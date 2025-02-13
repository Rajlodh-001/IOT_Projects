# ESPIF IoT Device

## Overview

The **ESPIF IoT Device** is an advanced smart controller designed for **Atomburge fans**. It features a built-in web server and WebSocket communication, allowing users to control fan speed, set sleep timers, and manage fan lights remotely. The device uses **infrared (IR) light** to send commands, ensuring seamless and reliable communication.

## Features

- **Web Server Integration**: Hosts a user-friendly interface for controlling the fan.
- **WebSockets**: Enables real-time, low-latency communication between the device and the user.
- **Infrared Communication**: Sends IR signals to control fan speed, sleep timer, and lighting.
- **Remote Access**: Control your fan from any device connected to the local network.
- **Sleep Timer**: Schedule automatic shutdown to save energy.
- **Fan Light Control**: Turn fan lights on/off or adjust brightness (if supported).
- **Compact & Efficient**: Designed for low power consumption and high reliability.

## Technologies Used

- **ESPIF Microcontroller**
- **Infrared (IR) Transmission**
- **Embedded Web Server (HTML, CSS, JavaScript)**
- **WebSockets for Real-Time Communication**
- **JSON-Based Command Processing**

## Installation & Setup

1. **Hardware Setup**:
   - Connect the ESPIF IoT device to a power source.
   - Ensure the IR transmitter is positioned towards the Atomburge fan for optimal performance.

2. **Software Configuration**:
   - Flash the firmware onto the ESPIF device.
   - Connect to the device’s Wi-Fi network and access the web interface at `Assigned IP`.

3. **Pairing with Fan**:
   - Point IR signals at fan.
   - Test basic controls such as fan speed adjustment and light toggling.

## Usage

- Open the web interface from your browser.
- Adjust fan speed using the interactive UI.
- Set a sleep timer to automate fan shutdown.
- Toggle fan lights with a single tap.
- Monitor real-time updates through WebSocket communication.

## Future Enhancements

- **Integration with Smart Assistants (Alexa, Google Home)**
- **Mobile App for Remote Access**
- **Customizable IR Codes for Multiple Devices**
- **Cloud Connectivity for Global Access**

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for more details.

## Contact

For inquiries or contributions, please reach out to **[Rajlodh911@.com](mailto:Rajlodh911@gmail.com)**.

