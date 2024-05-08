const express = require('express');
const expressWs = require('express-ws');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const path = require('path'); // Import the path module
const mqtt = require('mqtt'); // Import the mqtt module
const mqttClient = mqtt.connect('mqtt://192.168.87.246');


const app = express();
expressWs(app); // Enable WebSocket support
const PORT = 80;

//MQTT connection
mqttClient.on('connect', () => {
    console.log('Connected to MQTT Broker');
});

mqttClient.on('error', (error) => {
    console.error('MQTT Client Error:', error);
});

// Subscribe to topics related to DHT11 and potentiometer
const dht11TopicPattern = '+/dht11/+/+';
const potentiometerTopicPattern = '+/potentiometer/+/+';

mqttClient.subscribe([dht11TopicPattern, potentiometerTopicPattern], (err) => {
    if (err) {
        console.error('Error subscribing to MQTT topics:', err);
    } else {
        console.log('Subscribed to topics related to DHT11 and potentiometer');
    }
});

// Log received messages to the terminal
mqttClient.on('message', (topic, message) => {
    console.log(`${message.toString()} ${topic}`);
});

// WebSocket connection handler
app.ws('/api/controlSocket/:username', (ws, req) => {
    const username = req.params.username;

    // Log WebSocket connection
    console.log(`WebSocket connected for ${username}`);

    // Forward MQTT messages to the WebSocket
    mqttClient.on('message', (topic, message) => {
        const [mqttUsername, deviceType, deviceName] = topic.split('/');
        if (mqttUsername === username) {
            ws.send(JSON.stringify({ topic, message: message.toString() }));
        }
    });

    // Handle WebSocket closure
    ws.on('close', () => {
        console.log(`WebSocket closed for ${username}`);
    });
});

// Kết nối đến MongoDB
mongoose.connect('mongodb://localhost:27017/iotplatform', {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    
});

// Định nghĩa schema và model cho người dùng
const userSchema = new mongoose.Schema({
    username: String,
    password: String, 
    devices: [{ deviceName: String, deviceType: String }], 
});

const User = mongoose.model('User', userSchema);

// Sử dụng body-parser middleware để đọc dữ liệu từ request body
app.use(bodyParser.json());

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Serve the homepage
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/dashboard', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'dashboard.html'));
});

app.get('/control', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'control.html'));
});


// Đăng ký người dùng
app.post('/api/register', async (req, res) => {
    const { username, password } = req.body;

    try {
        // Kiểm tra xem người dùng đã tồn tại chưa
        const existingUser = await User.findOne({ username });
        if (existingUser) {
            return res.status(400).json({ message: 'Username already exists' });
        }

        // Nếu không, tạo người dùng mới
        const newUser = new User({ username, password });
        await newUser.save();
        res.json({ message: 'User registered successfully' });
    } catch (error) {
        res.status(500).json({ message: 'Error registering user' });
    }
});


// Login
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


// Endpoint API để lưu thông tin thiết bị vào MongoDB với ID người dùng
app.post('/api/saveDevices/:username', async (req, res) => {
    const deviceData = req.body;
    const username = req.params.username;

    try {
        const user = await User.findOne({ username: username });
        if (!user) {
            return res.status(404).json({ message: 'User not found' });
        }

        user.devices.push(...deviceData.map(d => ({
            deviceType: d.deviceType,
            deviceName: d.deviceName  
        })));

        await user.save();
        res.json({ message: 'Devices information saved successfully' });
    } catch (error) {
        console.error(error);
        res.status(500).json({ message: 'Error saving devices information' });
    }
});

// Endpoint API to fetch devices information from MongoDB by username
app.get('/api/fetchDevices/:username', async (req, res) => {
    const username = req.params.username;

    try {
        const user = await User.findOne({ username: username });
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
//Publish Message on specific topic
app.post('/api/controlDevice', async (req, res) => {
    const { username, deviceName, deviceType, action } = req.body;

    const topic = `${username}/${deviceType}/${deviceName}`;
    mqttClient.publish(topic, action);

    res.json({ message: 'Device control command sent' });
});

// Khởi động server
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
