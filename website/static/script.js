var socket = io();
var tempData = [];
var humData = [];
var labelsTemperature = [];
var labelsHumidity = [];
var temperatureChart = null;
var humidityChart = null;

var feedActive = false;


function createTemperatureChart() {
    var ctx = document.getElementById('temperatureChart').getContext('2d');
    temperatureChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labelsTemperature,
            datasets: [{
                label: 'Temperature (°C)',
                data: tempData,
                borderColor: 'rgba(255, 99, 132, 1)',
                backgroundColor: 'rgba(255, 99, 132, 0.2)',
                borderWidth: 2,
                fill: true
            }]
        },
        options: {
            scales: {
                x: {
                    title: { display: true, text: 'Time' }
                },
                y: {
                    title: { display: true, text: 'Temperature (°C)' },
                    suggestedMin: 20,
                    suggestedMax: 30
                }
            }
        }
    });
}

function createHumidityChart() {
    var ctx = document.getElementById('humidityChart').getContext('2d');
    humidityChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labelsHumidity,
            datasets: [{
                label: 'Humidity (%)',
                data: humData,
                borderColor: 'rgb(57, 60, 255)',
                backgroundColor: 'hsla(224, 88.60%, 31.00%, 0.27)',
                borderWidth: 2,
                fill: true
            }]
        },
        options: {
            scales: {
                x: {
                    title: { display: true, text: 'Time' }
                },
                y: {
                    title: { display: true, text: 'Humidity (%)' },
                    suggestedMin: 0,
                    suggestedMax: 100
                }
            }
        }
    });
}

// Update the chart with new data
function updateTemperatureChart(newTemp) {
    var currentTime = new Date().toLocaleTimeString();
    labelsTemperature.push(currentTime);
    tempData.push(newTemp);

    // Keep only the last 30 readings (1 per 2 sec = 1 minute)
    if (labelsTemperature.length > 30) {
        labelsTemperature.shift();
        tempData.shift();
    }

    temperatureChart.update();
}

// Update the chart with new data
function updateHumidityChart(newHumidity) {
    var currentTime = new Date().toLocaleTimeString();
    labelsHumidity.push(currentTime);
    humData.push(newHumidity);

    // Keep only the last 30 readings (1 per 2 sec = 1 minute)
    if (labelsHumidity.length > 30) {
        labelsHumidity.shift();
        humData.shift();
    }

    humidityChart.update();
}

// Handle sensor updates from the server
socket.on('sensor_update', function(data) {
    document.getElementById('temperature').textContent = data.temperature + " °C";
    document.getElementById('humidity').textContent = data.humidity + " %";
    document.getElementById('pressure').textContent = data.pressure + " hPa";
    document.getElementById('altitude').textContent = data.altitude + " m";
    document.getElementById('speed').textContent = data.metersPerHour + " m/h";
    document.getElementById('revolutions').textContent = data.rpm + " rpm";
    document.getElementById('distance').textContent = data.distance + " m";
    document.getElementById('rpiCpuTemperature').textContent = data.rpiCpuTemperature + " ºC";
    document.getElementById('rpiCpuWorkload').textContent = data.rpiCpuWorkload + " %";
    document.getElementById('roverBatteryLevel').textContent = data.roverBatteryCellVoltage + " V " +  "(" + data.roverBatteryPercentage + "%)";

    // Update the chart with new temperature data
    updateTemperatureChart(data.temperature);
    updateHumidityChart(data.humidity);
});

socket.on('camera_update', function(data) {
    document.getElementById('cameraFeed').src = URL.createObjectURL(new Blob([data], {type: 'image/jpeg'}));
});

document.getElementById('toggleFeed').addEventListener('click', function () {
    feedActive = !feedActive;
    socket.emit('toggle_camera', { active: feedActive });

    if (feedActive) {
        this.textContent = "Stop Feed";
        document.getElementById('cameraFeed').src = "/video_feed";  // Start feed
    } else {
        this.textContent = "Start Feed";
        document.getElementById('cameraFeed').src = "";  // Stop feed
    }
});

// Initialize the chart when the page loads
window.onload = function() {
    createTemperatureChart();
    createHumidityChart();
};

