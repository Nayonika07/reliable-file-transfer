# Reliable File Transfer Using UDP and SDN

## 1. Project Overview

This project focuses on implementing reliable file transfer using UDP sockets in C. Since UDP does not guarantee packet delivery, the application incorporates sequence numbers, acknowledgements (ACKs), and retransmission mechanisms to improve reliability.

Software Defined Networking (SDN) is used to manage the network environment and provide centralized control over packet forwarding.

## 2. Network Architecture

The project uses Mininet to simulate a network consisting of a UDP client, an Open vSwitch and a UDP server.

The POX controller manages the network switch through the OpenFlow protocol.

**Network components:**
- **h1:** UDP client
- **s1:** Open vSwitch
- **h2:** UDP server
- **POX:** SDN controller

POX runs on the host MacBook, while Mininet runs inside an Ubuntu virtual machine.

## 3. Technologies Used

- C programming
- UDP socket programming
- Stop-and-Wait ARQ
- Sequence numbers and acknowledgements
- Mininet
- Open vSwitch
- POX SDN Controller
- OpenFlow 1.0
- Ubuntu Linux

## 4. Project Structure

- `client/` – UDP client implementation
- `server/` – UDP server implementation
- `sdn/` – SDN-related project files
- `D1/` – Initial deliverables and documentation
- `D2/` – Further implementation and testing

## 5. UDP Client-Server Implementation

The client reads the selected file and divides it into packets for transmission using UDP.

The server receives the packets, checks their sequence numbers, writes the received data to an output file and sends acknowledgements.

The implementation uses a Stop-and-Wait ARQ mechanism to support reliable packet delivery.

## 6. SDN Controller Integration

POX is used as the SDN controller for the project.

It provides centralized control of the Open vSwitch through OpenFlow 1.0. The controller runs on the MacBook, while the Mininet topology runs inside the Ubuntu virtual machine.

The remote controller connection was successfully established, and the switch reported an active connection.

## 7. Initial Testing and Results

The following tests have been performed:

- Mininet installation and topology initialization.
- Successful communication between Mininet hosts.
- Successful connection between the Mininet switch and POX controller.
- Successful local UDP file transfer.
- Acknowledgement reception for transmitted packets.
- File integrity verification between the original and received files.

The initial Mininet connectivity test reported 0% packet loss.

The local UDP file transfer test also completed successfully, with the received file matching the original.

## 8. D1 Deliverables

D1 establishes the initial UDP client-server implementation and the SDN testing environment.

The deliverables include:

- UDP client and server implementation.
- Mininet installation and network setup.
- POX controller setup and connection.
- Initial network connectivity testing.
- UDP file transfer testing.
- Supporting screenshots and documentation.

## 9. Further Work

Further development will focus on integrating the UDP client-server application into the POX-controlled Mininet topology and evaluating reliable file transfer under packet loss conditions.
