# CAN (Controller Area Network) Communication Protocol
-----------------------------------------------------------

## Table of Contents
1. [Introduction](#introduction)
2. [CAN High and CAN Low](#can-high-and-can-low)
3. [Termination Resistance](#termination-resistance)
4. [Additional Details](#additional-details)

----------------------------------------

## Introduction
CAN (Controller Area Network) is a widely used communication protocol in the automotive and industrial sectors for reliable and efficient data exchange between electronic control units (ECUs). It was initially developed by Bosch in the 1980s to address the increasing complexity of electrical systems in vehicles and has since become an industry standard. The CAN protocol allows multiple ECUs to communicate with each other over a shared bus, enabling real-time data transmission.

![poster](./image/Data-Transmission-In-CAN-Protocol.png)

## CAN High and CAN Low
CAN communication uses a differential pair of wires for transmission: CAN High (CAN-H) and CAN Low (CAN-L). The differential signaling means that the information is transmitted as the voltage difference between these two wires. When a logical '0' is transmitted, the voltage on CAN-H is higher than on CAN-L, and when a logical '1' is transmitted, the voltage on CAN-L is higher than on CAN-H.

## Termination Resistance
The CAN bus requires termination resistors at both ends of the network. The purpose of these termination resistors (typically 120 ohms) is to prevent signal reflections and ensure proper signal integrity. When a CAN signal reaches the end of the bus, without termination, the signal could reflect back and interfere with the original signal, leading to data corruption and communication errors.

## Additional Details
Here are some additional key points about the CAN protocol:

- Multi-Master Communication: CAN allows for multi-master communication, which means any node on the bus can transmit data. There is no central master controlling the communication.

- Message-Based Communication: CAN uses a message-based communication paradigm. Each node on the bus can send and receive messages. Messages are identified by unique identifiers (IDs) ranging from 0 to 2,047 (CAN 2.0A) or 0 to 2,103,551 (CAN 2.0B). Lower IDs have higher priority.

- Error Detection and Handling: CAN has built-in error detection mechanisms, including Cyclic Redundancy Check (CRC), to ensure data integrity. In case of errors, the protocol implements automatic retransmission and error handling.

- Data Frame Formats: CAN supports two types of data frames - Standard Data Frames (CAN 2.0A) and Extended Data Frames (CAN 2.0B). Standard frames have 11-bit IDs, while extended frames have 29-bit IDs, allowing for a larger number of unique messages.

- Speed and Reliability: CAN is known for its high reliability and fault tolerance. It can operate at various data rates, with common speeds like 125 kbps, 250 kbps, 500 kbps, and 1 Mbps.

- Applications: CAN is widely used in automotive applications for communication between ECUs, such as engine control modules, transmission control modules, airbag systems, and more. It is also used in industrial automation, robotics, and other fields where reliable and real-time communication is essential.

Overall, the CAN protocol's robustness, reliability, and efficiency have made it a preferred choice for a broad range of applications, particularly in the automotive industry, where multiple ECUs must exchange data seamlessly and efficiently.

# CAN Bus Communication with Ultrasonic Sensor and Arduino

This documentation explains how to establish CAN bus communication between two Arduino boards (Arduino Uno as the receiver and Arduino Nano as the transmitter) using MCP2515 CAN Bus Controller Modules. Additionally, an Ultrasonic Sensor is connected to the transmitter Arduino Nano to measure distance and transmit the data over the CAN bus to the receiver Arduino Uno.

![poster](./image/IMG_3832.JPG)

## Components Used
- Arduino Uno (Receiver) x 1
- Arduino Nano (Transmitter) x 1
- MCP2515 CAN Bus Controller Module x 2
![poster](./image/MCP2515-Parts.jpg)
- Ultrasonic Sensor x 1

## Wiring

### Transmitter (Arduino Nano) Wiring
- Connect the Ultrasonic Sensor:
  - VCC to 5V
  - GND to GND
  - TRIG_PIN to digital pin 5
  - ECHO_PIN to digital pin 6

- Connect the MCP2515 CAN Bus Controller Module:
  - VCC to 5V
  - GND to GND
  - CS (Slave Select) to digital pin 10
  - SO (MISO) to Arduino Nano's SPI MISO (Master-In Slave-Out) pin (D12)
  - SI (MOSI) to Arduino Nano's SPI MOSI (Master-Out Slave-In) pin (D11)
  - SCK (Serial Clock) to Arduino Nano's SPI SCK (Clock) pin (D13)

### Receiver (Arduino Uno) Wiring
- Connect the MCP2515 CAN Bus Controller Module:
  - VCC to 5V
  - GND to GND
  - CS (Slave Select) to digital pin 10
  - SO (MISO) to Arduino Uno's SPI MISO (Master-In Slave-Out) pin (D12)
  - SI (MOSI) to Arduino Uno's SPI MOSI (Master-Out Slave-In) pin (D11)
  - SCK (Serial Clock) to Arduino Uno's SPI SCK (Clock) pin (D13)

## Transmitter Code Explanation (Arduino Nano)

1. **Libraries and Constants:** The required libraries for the transmitter code are included: `SPI.h` for SPI communication and `mcp2515.h` for CAN communication.

2. **CAN Frame and MCP2515 Initialization:** The CAN message frame is defined as `can_frame` to store the data to be transmitted. The MCP2515 CAN Bus Controller module is initialized with the `mcp2515` object using `SPI` on digital pin 10 (CS pin).

3. **Ultrasonic Function:** The `ultrasonic()` function is defined to measure the distance using the ultrasonic sensor. It triggers the sensor, calculates the duration of the echo, and calculates the distance in centimeters. The measured distance is then printed to the serial monitor.

4. **Setup Function:**
   - The CAN message (`canMsg`) is initialized with a specific CAN ID (`0x036`) and data length (`8`).
   - The Ultrasonic Sensor pins (TRIG_PIN and ECHO_PIN) are set as OUTPUT and INPUT, respectively.
   - The SPI communication is started.
   - The MCP2515 module is reset, the CAN speed is set to `CAN_125KBPS`, and the normal mode is set using `mcp2515.setNormalMode()`.

5. **Loop Function:**
   - The `ultrasonic()` function is called to measure the distance.
   - The measured distance is updated in the `canMsg` data array.
   - The CAN message is sent using `mcp2515.sendMessage(&canMsg)`.

## Receiver Code Explanation (Arduino Uno)

1. **Libraries and Constants:** The required libraries for the receiver code are included: `Waveshare_LCD1602_RGB.h` for controlling the LCD screen, `SPI.h` for SPI communication, and `mcp2515.h` for CAN communication.

2. **LCD Initialization:** The `Waveshare_LCD1602_RGB` object (`lcd`) is initialized to control the RGB 16x2 LCD screen. The LCD is cleared and displays the "CANBUS TUTORIAL" message for 3 seconds.

3. **CAN Frame and MCP2515 Initialization:** The CAN message frame is defined as `can_frame` to store the received data. The MCP2515 CAN Bus Controller module is initialized with the `mcp2515` object using `SPI` on digital pin 10 (CS pin).

4. **Setup Function:**
   - Serial communication is started at a baud rate of `9600`.
   - SPI communication is started.
   - The LCD is initialized and cleared.
   - The MCP2515 module is reset, the CAN speed is set to `CAN_125KBPS`, and the normal mode is set using `mcp2515.setNormalMode()`.

5. **Loop Function:**
   - The `mcp2515.readMessage(&canMsg)` function is called to check if a CAN message has been received.
   - If a message with the specified `slaveID` is received, the distance data is extracted from the received CAN message and displayed on the LCD.
