// Get userName from local storage
const username = localStorage.getItem('username');
document.getElementById('username').innerText = username;

// Go to control page
function goToControlPage() {
    const selectedUsername = localStorage.getItem('username');
    window.location.href = `/control?username=${selectedUsername}`;
}

// Function to add a new device to the list
function addDevice() {
    const deviceName = document.getElementById('deviceName').value;
    const deviceList = document.getElementById('deviceList');
    const selectedDevice = deviceList.options[deviceList.selectedIndex].value;

    // Create a new line for the device
    const deviceLine = document.createElement('div');
    // Updated line to include device ID
    deviceLine.innerHTML = `<p>${selectedDevice} - Name: ${deviceName} </p>`;
    
    // Append the new line to the device container
    document.getElementById('deviceContainer').appendChild(deviceLine);
}

// Function to save devices to the server
async function saveDevices() {
    const deviceContainer = document.getElementById('deviceContainer');
    const devices = deviceContainer.getElementsByTagName('p');

    const username = localStorage.getItem('username'); // Get the user ID from local storage

    const deviceData = Array.from(devices).map(device => {
        const parts = device.innerText.split(' - ');
        return {
            deviceType: parts[0], 
            deviceName: parts[1].split(': ')[1]
        };
    });

    // Send a request to the server to save device information
    const response = await fetch(`/api/saveDevices/${username}`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(deviceData),
    });

    const data = await response.json();
    alert(data.message);

    // Clear the device container after saving
    deviceContainer.innerHTML = '';
}
