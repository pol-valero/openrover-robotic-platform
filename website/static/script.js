var socket = io();
var tempData = [];
var labels = [];
var chart = null;

var feedActive = false;


// Create the temperature chart
function createChart() {
    var ctx = document.getElementById('temperatureChart').getContext('2d');
    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
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

// Update the chart with new data
function updateChart(newTemp) {
    var currentTime = new Date().toLocaleTimeString();
    labels.push(currentTime);
    tempData.push(newTemp);

    // Keep only the last 30 readings (1 per 2 sec = 1 minute)
    if (labels.length > 30) {
        labels.shift();
        tempData.shift();
    }

    chart.update();
}

// Handle sensor updates from the server
socket.on('sensor_update', function(data) {
    document.getElementById('temperature').textContent = data.temperature + " °C";
    document.getElementById('humidity').textContent = data.humidity + " %";
    document.getElementById('battery').textContent = data.battery + " %";

    // Update the chart with new temperature data
    updateChart(data.temperature);
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
    createChart();
};
