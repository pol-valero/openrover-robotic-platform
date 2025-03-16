from flask import Flask, render_template, Response
from flask_socketio import SocketIO
from picamera2 import Picamera2
import cv2
import random
import time
import threading

from logic.sharedStructs import RoverBatteryValues
from logic.sharedStructs import EnvironmentalValues
from logic.sharedStructs import SpeedometerValues

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

environmentalValues = EnvironmentalValues()

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (1280, 720)}))
picam2.start()

def generate_frames():
    """Continuously capture frames and send as a video stream."""
    while True:
        frame = picam2.capture_array()  # Capture frame as numpy array
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert BGR to RGB
        _, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 90])  # Increase quality
        frame_bytes = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')
        time.sleep(0.03)  # 30 FPS

@app.route('/video_feed')
def video_feed():
    """Route to provide the video stream."""
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


def set_sensor_data(environmentalValuesArg):
    global environmentalValues
    environmentalValues = environmentalValuesArg
    print('Environmental values set1')
    print('Temperature1: {}'.format(environmentalValues.temperature))


def get_sensor_data():
    print('Environmental values set2')
    print('Temperature2: {}'.format(environmentalValues.temperature))
    return {
        "temperature": environmentalValues.temperature,
        "humidity": environmentalValues.humidity,
        "battery": 0
    }

def send_sensor_data():
    """Continuously send sensor data to all connected clients."""
    while True:
        socketio.emit('sensor_update', get_sensor_data())
        time.sleep(2)  # Send data every 2 seconds

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('connect')
def handle_connect():
    print("Client connected")

def runSocketIo():
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)

def startThread():
    # Start the background thread
    sensor_thread = threading.Thread(target=send_sensor_data, daemon=True)
    sensor_thread.start()

    # Start the Flask web server
    socketIoThread = threading.Thread(target=runSocketIo)
    socketIoThread.start()


#if __name__ == '__main__':
    #socketio.run(app, host='0.0.0.0', port=5000, debug=True)
