from flask import Flask, render_template, Response
from flask_socketio import SocketIO
from picamera2 import Picamera2
import base64
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

send_camera_feed_Thread = None

def send_camera_feed():
    while True:
        frame = picam2.capture_array()  # Capture frame as numpy array
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert BGR to RGB
        _, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])  

        socketio.emit('camera_update', buffer.tobytes())  # Send frame to clients
        time.sleep(0.03)

@socketio.on('toggle_camera')
def handle_toggle_camera(data):

    global send_camera_feed_Thread
    
    if data['active']:
        picam2.start()
        send_camera_feed_Thread = threading.Thread(target=send_camera_feed, daemon=True)
        send_camera_feed_Thread.start()
    else:
        picam2.stop()
        send_camera_feed_Thread.stop_event.set()




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
