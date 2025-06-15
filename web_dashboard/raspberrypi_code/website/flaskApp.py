from flask import Flask, render_template, Response
from flask_socketio import SocketIO
from picamera2 import Picamera2
import cv2
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
picam2.configure(picam2.create_preview_configuration(main={"size": (1920, 1080)}))

cameraFeedActive = False


def startThread():

    socketIoThread = threading.Thread(target=runSocketIo, daemon=True)
    socketIoThread.start()

    sendStatusDataThread = threading.Thread(target=sendStatusData, daemon=True)
    sendStatusDataThread.start()

    sendCameraFeedThread = threading.Thread(target=sendCameraFeed, daemon=True)
    sendCameraFeedThread.start()


def runSocketIo():
    socketio.run(app, host='0.0.0.0', port=5000, debug=False, allow_unsafe_werkzeug=True)

def sendStatusData():
    while True:
        socketio.emit('status_data_update', getStatusData())
        time.sleep(2)

def sendCameraFeed():
    while True:
        if cameraFeedActive:
            frame = picam2.capture_array()
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  #We convert BGR to RGB, to avoid color distortion
            _, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])  

            socketio.emit('camera_feed_update', buffer.tobytes())
            time.sleep(0.033)    # Send frame every 0.033 seconds, 30 FPS

def setStatusData(environmentalValuesArg, roverBatteryValuesArg, speedometerValuesArg, raspberryPiStatusValuesArg):
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

def getStatusData():
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

def getCameraFeedStatus():
    return cameraFeedActive


@app.route('/')
@app.route('/monitor')
def monitor():
    return render_template('monitor.html')

@app.route('/control')
def control():
    return render_template('control.html')

@socketio.on('connect')
def handleConnection():
    print("New client connected")

@socketio.on('camera_feed_btn_clicked')
def handleCameraFeedBtnClicked(feedStatus):

    global cameraFeedActive
    cameraFeedActive = feedStatus['active']
    
    if cameraFeedActive:
        picam2.start()
    else:
        picam2.stop()
