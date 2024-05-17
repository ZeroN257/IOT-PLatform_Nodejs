<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>README - IoT Platform</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
            line-height: 1.6;
        }
        code {
            background-color: #f4f4f4;
            padding: 2px 4px;
            border-radius: 4px;
        }
        pre {
            background-color: #f4f4f4;
            padding: 10px;
            border-radius: 4px;
            overflow-x: auto;
        }
    </style>
</head>
<body>
    <h1>IoT Platform</h1>
    <p>This project is a simple IoT platform built using Node.js, Express, WebSockets, MongoDB, and MQTT. It allows users to register, login, save device information, and control devices via WebSockets and MQTT.</p>

    <h2>Features</h2>
    <ul>
        <li>User registration and login</li>
        <li>Save and retrieve device information</li>
        <li>WebSocket support for real-time communication</li>
        <li>MQTT integration for device communication</li>
        <li>Serve static files and HTML pages</li>
    </ul>

    <h2>Project Structure</h2>
    <pre>
    .
    ├── public
    │   ├── index.html
    │   ├── dashboard.html
    │   └── control.html
    ├── server.js (main server file)
    └── README.html (this file)
    </pre>

    <h2>Installation</h2>
    <p>To run this project, follow these steps:</p>
    <ol>
        <li>Clone the repository:</li>
        <pre><code>git clone https://github.com/yourusername/iot-platform.git</code></pre>
        <li>Navigate to the project directory:</li>
        <pre><code>cd iot-platform</code></pre>
        <li>Install the dependencies:</li>
        <pre><code>npm install</code></pre>
        <li>Start the server:</li>
        <pre><code>node server.js</code></pre>
    </ol>

    <h2>Usage</h2>
    <h3>MQTT Broker Configuration</h3>
    <p>The server connects to an MQTT broker at <code>mqtt://192.168.87.246</code>. Ensure your MQTT broker is running and accessible at this address.</p>

    <h3>Endpoints</h3>
    <p>The following endpoints are available:</p>
    <ul>
        <li><code>GET /</code> - Serve the homepage.</li>
        <li><code>GET /dashboard</code> - Serve the dashboard page.</li>
        <li><code>GET /control</code> - Serve the control page.</li>
        <li><code>POST /api/register</code> - Register a new user.</li>
        <li><code>POST /api/login</code> - Login a user.</li>
        <li><code>POST /api/saveDevices/:username</code> - Save device information for a user.</li>
        <li><code>GET /api/fetchDevices/:username</code> - Fetch device information for a user.</li>
        <li><code>POST /api/controlDevice</code> - Send a control command to a device.</li>
        <li><code>WS /api/controlSocket/:username</code> - WebSocket endpoint for real-time control and monitoring.</li>
    </ul>

    <h2>Database Configuration</h2>
    <p>The project uses MongoDB for storing user and device information. Ensure MongoDB is running and accessible at <code>mongodb://localhost:27017/iotplatform</code>.</p>

    <h2>Code Overview</h2>
    <pre><code class="language-js">
const express = require('express');
const expressWs = require('express-ws');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const path = require('path');
const mqtt = require('mqtt');

const mqttClient = mqtt.connect('mqtt://192.168.87.246');
const app = express();
expressWs(app);
const PORT = 80;

// MQTT connection and message handling
mqttClient.on('connect', () => {
    console.log('Connected to MQTT Broker');
});

mqttClient.on('error', (error) => {
    console.error('MQTT Client Error:', error);
});

const dht11TopicPattern = '+/dht11/+/+';
const potentiometerTopicPattern = '+/potentiometer/+/+';

mqttClient.subscribe([dht11TopicPattern, potentiometerTopicPattern], (err) => {
    if (err) {
        console.error('Error subscribing to MQTT topics:', err);
    } else {
        console.log('Subscribed to topics related to DHT11 and potentiometer');
    }
});

mqttClient.on('message', (topic, message) => {
    console.log(`${message.toString()} ${topic}`);
});

// WebSocket connection handler
app.ws('/api/controlSocket/:username', (ws, req) => {
    const username = req.params.username;

    console.log(`WebSocket connected for ${username}`);

    mqttClient.on('message', (topic, message) => {
        const [mqttUsername, deviceType, deviceName] = topic.split('/');
        if (mqttUsername === username) {
            ws.send(JSON.stringify({ topic, message: message.toString() }));
        }
    });

    ws.on('close', () => {
        console.log(`WebSocket closed for ${username}`);
    });
});

// MongoDB connection and schemas
mongoose.connect('mongodb://localhost:27017/iotplatform', {
    useNewUrlParser: true,
    useUnifiedTopology: true,
});

const userSchema = new mongoose.Schema({
    username: String,
    password: String,
    devices: [{ deviceName: String, deviceType: String }],
});

const User = mongoose.model('User', userSchema);

// Middleware and static file serving
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/dashboard', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'dashboard.html'));
});

app.get('/control', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'control.html'));
});

// User registration and login endpoints
app.post('/api/register', async (req, res) => {
    const { username, password } = req.body;

    try {
        const existingUser = await User.findOne({ username });
        if (existingUser) {
            return res.status(400).json({ message: 'Username already exists' });
        }

        const newUser = new User({ username, password });
        await newUser.save();
        res.json({ message: 'User registered successfully' });
    } catch (error) {
        res.status(500).json({ message: 'Error registering user' });
    }
});

app.post('/api/login', async (req, res) => {
    const { username, password } = req.body;

    try {
        const user = await User.findOne({ username, password });
        if (user) {
            res.json({ message: 'Login successful', username: user.username });
        } else {
            return res.status(401).json({ message: 'Invalid username or password' });
        }
    } catch (error) {
        return res.status(500).json({ message: 'Error logging in' });
    }
});

// Device information endpoints
app.post('/api/saveDevices/:username', async (req, res) => {
    const deviceData = req.body;
    const username = req.params.username;

    try {
        const user = await User.findOne({ username });
        if (!user) {
            return res.status(404).json({ message: 'User not found' });
        }

        user.devices.push(...deviceData.map(d => ({
            deviceType: d.deviceType,
            deviceName: d.deviceName,
        })));

        await user.save();
        res.json({ message: 'Devices information saved successfully' });
    } catch (error) {
        console.error(error);
        res.status(500).json({ message: 'Error saving devices information' });
    }
});

app.get('/api/fetchDevices/:username', async (req, res) => {
    const username = req.params.username;

    try {
        const user = await User.findOne({ username });
        if (!user) {
            return res.status(404).json({ message: 'User not found' });
        }

        const devices = user.devices.map(device => ({
            deviceType: device.deviceType,
            deviceName: device.deviceName,
            deviceId: device._id,
        }));

        res.json({ devices });
    } catch (error) {
        console.error(error);
        res.status(500).json({ message: 'Error fetching devices information' });
    }
});

app.post('/api/controlDevice', async (req, res) => {
    const { username, deviceName, deviceType, action } = req.body;

    const topic = `${username}/${deviceType}/${deviceName}`;
    mqttClient.publish(topic, action);

    res.json({ message: 'Device control command sent' });
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
    </code></pre>

    <h2>License</h2>
    <p>This project is licensed under the MIT License.</p>

    <h2>Contributing</h2>
    <p>Contributions are welcome! Please open an issue or submit a pull request.</p>
</body>
</html>
