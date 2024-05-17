
    <h1>IoT Platform</h1>
    <p>This project is an IoT (Internet of Things) platform developed using Node.js and Express.js. It enables users to register, login, manage devices, and control them remotely using MQTT (Message Queuing Telemetry Transport) protocol.</p>

    <h2>Dependencies</h2>
    <ul>
        <li>express</li>
        <li>express-ws</li>
        <li>body-parser</li>
        <li>mongoose</li>
        <li>path</li>
        <li>mqtt</li>
    </ul>

    <h2>Installation</h2>
    <p>To run this project, you need to have Node.js and MongoDB installed on your system.</p>
    <ol>
        <li>Clone the repository:</li>
        <pre><code>git clone [repository_url]</code></pre>
        <li>Install dependencies:</li>
        <pre><code>npm install</code></pre>
        <li>Start the server:</li>
        <pre><code>npm start</code></pre>
    </ol>

    <h2>Usage</h2>
    <p>Once the server is running, you can access the following endpoints:</p>
    <ul>
        <li><strong>Register:</strong> POST /api/register - Register a new user</li>
        <li><strong>Login:</strong> POST /api/login - Log in with existing credentials</li>
        <li><strong>Save Devices:</strong> POST /api/saveDevices/:username - Save device information for a user</li>
        <li><strong>Fetch Devices:</strong> GET /api/fetchDevices/:username - Fetch devices information for a user</li>
        <li><strong>Control Device:</strong> POST /api/controlDevice - Send control commands to a device</li>
    </ul>

    <h2>WebSocket Support</h2>
    <p>The platform also supports WebSocket connections for real-time communication. WebSocket connection can be established at:</p>
    <pre><code>ws://localhost/api/controlSocket/:username</code></pre>

    <h2>MQTT Integration</h2>
    <p>The platform integrates with MQTT for device communication. It subscribes to topics related to DHT11 and potentiometer sensors and publishes control commands to specific device topics.</p>

    <h2>Database</h2>
    <p>The platform uses MongoDB for data storage. It connects to a MongoDB database named "iotplatform" by default.</p>

    <h2>Contributing</h2>
    <p>Contributions are welcome! Feel free to submit bug reports, feature requests, or pull requests.</p>

    <h2>License</h2>
    <p>This project is licensed under the MIT License.</p>
