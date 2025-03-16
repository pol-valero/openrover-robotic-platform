var socket = io();

socket.on('sensor_update', function(data) {
    document.getElementById('temperature').innerText = data.temperature + " °C";
    document.getElementById('humidity').innerText = data.humidity + " %";
    document.getElementById('battery').innerText = data.battery + " %";
});