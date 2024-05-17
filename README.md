<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Platform Project</title>
</head>
<body>
    <h1>Platform Project</h1>
    <p>This project provides a platform for IoT (Internet of Things) applications. It includes a server implemented in Node.js using Express.js framework, WebSocket support, and MongoDB for data storage.</p>

    <h2>Getting Started</h2>
    <p>To get started with the project, follow these steps:</p>
    <ol>
        <li>Clone the repository to your local machine.</li>
        <li>Install the dependencies by running <code>npm install</code>.</li>
        <li>Ensure MongoDB is running locally on port 27017.</li>
        <li>Start the server by running <code>node server.js</code>.</li>
        <li>You can now access the platform through a web browser.</li>
    </ol>

    <h2>Dependencies</h2>
    <p>The following dependencies are used in this project:</p>
    <ul>
        <li>Express.js - for building the web server and RESTful APIs.</li>
        <li>express-ws - for WebSocket support.</li>
        <li>body-parser - for parsing request bodies.</li>
        <li>mongoose - for MongoDB object modeling.</li>
        <li>mqtt - for MQTT protocol support.</li>
    </ul>

    <h2>File Structure</h2>
    <p>The main file of the project is <code>server.js</code>. Other important files include:</p>
    <ul>
        <li><code>public/index.html</code> - Homepage of the platform.</li>
        <li><code>public/dashboard.html</code> - Dashboard page.</li>
        <li><code>public/control.html</code> - Control page.</li>
    </ul>

    <h2>Endpoints</h2>
    <p>The following endpoints are available:</p>
    <ul>
        <li><code>POST /api/register</code> - Register a new user.</li>
        <li><code>POST /api/login</code> - Login for existing users.</li>
        <li><code>POST /api/saveDevices/:username</code> - Save device information for a user.</li>
        <li><code>GET /api/fetchDevices/:username</code> - Fetch devices information for a user.</li>
        <li><code>POST /api/controlDevice</code> - Publish message on a specific topic to control a device.</li>
    </ul>

    <h2>MQTT Integration</h2>
    <p>The server integrates with an MQTT broker to communicate with IoT devices. It subscribes to topics related to DHT11 and potentiometer sensors and can publish messages to control devices.</p>

    <h2>WebSocket Support</h2>
    <p>The server provides WebSocket support for real-time communication with clients. Clients can establish WebSocket connections to receive updates and control IoT devices.</p>

    <h2>Contributing</h2>
    <p>Contributions to the project are welcome. Fork the repository, make your changes, and submit a pull request.</p>

    <h2>License</h2>
    <p>This project is licensed under the MIT License. See the <code>LICENSE</code> file for details.</p>

    <h2>Authors</h2>
    <p>This project is maintained by [Your Name].</p>

    <h2>Contact</h2>
    <p>If you have any questions or suggestions, feel free to contact us at [your email address].</p>
</body>
</html>
