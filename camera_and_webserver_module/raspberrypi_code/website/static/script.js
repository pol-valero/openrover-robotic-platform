var socket = io();

var tempData = [];
var humData = [];
var labelsTemperature = [];
var labelsHumidity = [];
var temperatureChart = null;
var humidityChart = null;

var feedActive = false;


function createTemperatureChart() {
    var temperatureChartCanvas = document.getElementById('temperatureChart');
    temperatureChart = new Chart(temperatureChartCanvas, {
        type: 'line',
        data: {
            labels: labelsTemperature,
            datasets: [{
                label: 'Temperature (°C)',
                data: tempData,
                borderColor: 'rgb(249, 95, 128)',
                backgroundColor: 'rgba(247, 94, 127, 0.2)',
                fill: true,
                borderWidth: 2
            }]
        },
        options: {
            plugins: {
                legend: { display: false }  //We hide the legend (label and color box)
            },
            scales: {
                x: {
                    title: { display: true, text: 'Time' }
                },
                y: {
                    title: { display: true, text: 'Temperature (°C)' },
                    suggestedMin: 15,
                    suggestedMax: 30
                }
            }
        }
    });
}

function createHumidityChart() {
    var humidityChartCanvas = document.getElementById('humidityChart');
    humidityChart = new Chart(humidityChartCanvas, {
        type: 'line',
        data: {
            labels: labelsHumidity,
            datasets: [{
                label: 'Humidity (%)',
                data: humData,
                borderColor: 'rgb(56, 59, 251)',
                backgroundColor: 'hsla(224, 89.70%, 30.40%, 0.27)',
                fill: true,
                borderWidth: 2
            }]
        },
        options: {
            plugins: {
                legend: { display: false }  //We hide the legend (label and color box)
            },
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

function updateTemperatureChart(newTemp) {
    var currentTime = new Date().toLocaleTimeString();

    labelsTemperature.push(currentTime);
    tempData.push(newTemp);

    // We only keep the last 30 readings (every 2 seconds the Rover should send the temp/hum data, therefore the last minute of data should be displayed)
    if (labelsTemperature.length > 30) {
        labelsTemperature.shift();      // By shifting, we remove the first element from the array, and shift the other elements to the left one position
        tempData.shift();
    }

    temperatureChart.update();
}

function updateHumidityChart(newHumidity) {
    var currentTime = new Date().toLocaleTimeString();

    labelsHumidity.push(currentTime);
    humData.push(newHumidity);

    // We only keep the last 30 readings (every 2 seconds the Rover should send the temp/hum data, therefore the last minute of data should be displayed)
    if (labelsHumidity.length > 30) {
        labelsHumidity.shift();     // By shifting, we remove the first element from the array, and shift the other elements to the left one position
        humData.shift();
    }

    humidityChart.update();
}


socket.on('status_data_update', function(data) {
    document.getElementById('temperatureValue').textContent = data.temperature + " °C";
    document.getElementById('humidityValue').textContent = data.humidity + " %";
    document.getElementById('pressureValue').textContent = data.pressure + " hPa";
    document.getElementById('altitudeValue').textContent = data.altitude + " m";
    document.getElementById('speedValue').textContent = data.metersPerHour + " m/h";
    document.getElementById('revolutionsValue').textContent = data.rpm + " rpm";
    document.getElementById('distanceValue').textContent = data.distance + " m";
    document.getElementById('rpiCpuTemperatureValue').textContent = data.rpiCpuTemperature + " ºC";
    document.getElementById('rpiCpuWorkloadValue').textContent = data.rpiCpuWorkload + " %";
    document.getElementById('roverBatteryLevelValue').textContent = data.roverBatteryCellVoltage + " V " +  "(" + data.roverBatteryPercentage + "%)";

    //We update the charts with the new temp/hum data
    updateTemperatureChart(data.temperature);
    updateHumidityChart(data.humidity);
});

socket.on('camera_feed_update', function(data) {
    document.getElementById('cameraFeed').src = URL.createObjectURL(new Blob([data], {type: 'image/jpeg'}));
});

document.getElementById('cameraFeedBtn').addEventListener('click', function () {
    feedActive = !feedActive;
    socket.emit('camera_feed_btn_clicked', { active: feedActive });

    if (feedActive) {
        this.textContent = "Stop Feed";
    } else {
        this.textContent = "Start Feed";
    }
});

window.onload = function() {
    createTemperatureChart();
    createHumidityChart();
};

