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
from logic.sharedStructs import RaspberryPiStatusValues

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

environmentalValues = EnvironmentalValues()
roverBatteryValues = RoverBatteryValues()
speedometerValues = SpeedometerValues()
raspberryPiStatusValues = RaspberryPiStatusValues()

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (1280, 720)}))

send_camera_feed_Thread = None

def send_camera_feed():
    while True:
        frame = picam2.capture_array()  # Capture frame as numpy array
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert BGR to RGB
        _, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])  

        socketio.emit('camera_update', buffer.tobytes())  # Send frame to clients
        time.sleep(0.05)    # Send frame every 0.05 seconds, 20 FPS

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




def set_sensor_data(environmentalValuesArg, roverBatteryValuesArg, speedometerValuesArg, raspberryPiStatusValuesArg):
    global environmentalValues
    global roverBatteryValues
    global speedometerValues
    global raspberryPiStatusValues

    if environmentalValuesArg is not None:
        environmentalValues = environmentalValuesArg
    if roverBatteryValuesArg is not None:
        roverBatteryValues = roverBatteryValuesArg
    if speedometerValuesArg is not None:
        speedometerValues = speedometerValuesArg
    if raspberryPiStatusValuesArg is not None:
        raspberryPiStatusValues = raspberryPiStatusValuesArg


def get_monitoring_data():
    return {
        "temperature": environmentalValues.temperature,
        "humidity": environmentalValues.humidity,
        "pressure": environmentalValues.pressure,
        "altitude": environmentalValues.altitude,
        "rpm": speedometerValues.rpm,
        "metersPerHour": speedometerValues.metersPerHour,
        "distance": speedometerValues.distance,
        "roverBatteryCellVoltage": roverBatteryValues.cellVoltage,
        "roverBatteryPercentage": roverBatteryValues.percentage,
        "rpiCpuTemperature": raspberryPiStatusValues.cpuTemperature,
        "rpiCpuWorkload": raspberryPiStatusValues.cpuWorkload
    }

def send_sensor_data():
    """Continuously send sensor data to all connected clients."""
    while True:
        socketio.emit('sensor_update', get_monitoring_data())
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
