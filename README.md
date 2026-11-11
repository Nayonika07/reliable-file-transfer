# Reliable File Transfer using UDP and Software-Defined Networking (SDN)

**A reliable file-transfer application using UDP socket programming, Stop-and-Wait ARQ, and Software-Defined Networking.**

---

## 1. Project Overview

UDP is a connectionless transport protocol that does not guarantee packet delivery, ordering, or retransmission. This project aims to overcome these limitations by implementing a reliable file-transfer mechanism over UDP and integrating it with a Software-Defined Networking (SDN) environment.

The application enables users to transfer files between network hosts while monitoring packet transmission, acknowledgements, retransmissions, and file integrity.

## 2. Project Objectives

- Implement reliable file transfer using UDP sockets in C.
- Handle packet loss using acknowledgements and retransmissions.
- Maintain packet ordering using sequence numbers.
- Support the transfer of different file types, including images, audio, and video.
- Integrate the application with an SDN-controlled network.
- Explore dynamic routing and recovery during network failures.
- Evaluate performance using measurable network statistics.

## 3. Technologies Used

| Technology | Purpose |
|---|---|
| C | UDP client-server implementation |
| Python | Graphical user interface |
| UDP Sockets | Network communication |
| Stop-and-Wait ARQ | Reliable packet delivery |
| Mininet | Network emulation |
| POX Controller | SDN control |
| OpenFlow | Communication between controller and switches |
| Ubuntu | Development and testing |
| Git and GitHub | Version control and collaboration |

## 4. System Architecture

The project consists of the following components:

### 4.1 UDP Client
- Reads the selected file.
- Divides the file into packets.
- Assigns sequence numbers.
- Transmits packets to the receiver.
- Waits for acknowledgements.
- Retransmits packets when acknowledgements are not received within the timeout period.

### 4.2 UDP Server
- Receives incoming packets.
- Checks packet sequence numbers.
- Writes received data into the output file.
- Sends acknowledgements to the client.
- Handles duplicate packets.

### 4.3 Graphical User Interface
- Allows users to browse and select files.
- Initiates file transfers.
- Displays transfer logs and progress.
- Shows transfer status and file-integrity verification.

### 4.4 SDN Controller
- Communicates with OpenFlow switches.
- Installs and manages forwarding rules.
- Supports network-path management.
- Provides the foundation for dynamic rerouting during link failures.

## 5. Key Features

- UDP-based client-server communication.
- Stop-and-Wait ARQ reliability mechanism.
- Sequence numbers and acknowledgements.
- Timeout-based packet retransmission.
- Duplicate packet handling.
- Graphical file selection.
- Live transfer logs.
- Transfer progress monitoring.
- File-integrity verification.
- Mininet-based network emulation.
- POX-based SDN control.

## 6. Project Requirements and Evaluation

| Requirement | Description |
|---|---|
| Complete functionality | Reliable file transfer between network hosts |
| Dynamic SDN | Controller-based forwarding and dynamic path changes |
| Challenging scenario | Demonstration under packet loss or link failure |
| Performance evaluation | Transfer time, throughput, packet loss and retransmissions |
| Documentation and demonstration | GitHub documentation, screenshots, demo and viva |

## 7. Testing and Performance Evaluation

The application is tested using different file types, including images, audio and video.

Testing includes:

- File transmission between client and server.
- Packet transmission and acknowledgement monitoring.
- Retransmission behaviour.
- Received-file size comparison.
- File-integrity verification.
- Transfer-time measurement.
- Mininet network testing.
- OpenFlow flow-table inspection.

### Performance Metrics

- Total packets transmitted.
- Total packets received.
- Acknowledgements sent and received.
- Number of retransmissions.
- Packet loss percentage.
- Total transfer time.
- Throughput.

Detailed test results and screenshots are maintained in the D2 documentation.

## 8. Repository Structure

```text
reliable-file-transfer/
│
├── README.md
├── D2_commands.txt
│
├── client/
│   └── udp_client.c
│
├── server/
│   └── udp_server.c
│
├── sdn/
│
├── reliability/
│
├── tests/
│
├── d2_gui.py
│
└── docs/
    ├── D1/
    │
    └── D2/
        ├── UI_Screenshots/
        ├── final-flow-table.png
        ├── mininet-test-results.png
        └── mininet_test_results1.png
```

*The structure may be updated as additional implementation files and documentation are added.*

## 9. How to Run

### Compile the UDP Client

```bash
gcc -Wall -Wextra -O2 client/udp_client.c -o udp_client
```

### Compile the UDP Server

```bash
gcc -Wall -Wextra -O2 server/udp_server.c -o udp_server
```

### Start the Server

```bash
./udp_server 5000 received_file
```

### Start the Client

Open another terminal:

```bash
./udp_client 127.0.0.1 5000 /path/to/input_file
```

Replace `/path/to/input_file` with the actual file path.

### Launch the GUI

```bash
python3 d2_gui.py
```

The local GUI mode uses a local test receiver. Mininet-based execution requires the corresponding network topology and controller configuration.

## 10. Documentation and Evidence

All project documentation and testing evidence are maintained in the `docs/` directory.

### D1 Documentation
- Initial project implementation.
- UDP client-server implementation.
- Mininet and SDN setup evidence.
- D1 testing screenshots and documentation.

### D2 Documentation

**GUI Screenshots:** `docs/D2/UI_Screenshots/`

- GUI home screen.
- File selection.
- Transfer progress.
- Successful file transfer.
- File-integrity verification.

**Mininet and SDN Evidence:**

- `final-flow-table.png` – Final OpenFlow flow-table output.
- `mininet-test-results.png` – Mininet network testing results.
- `mininet_test_results1.png` – Additional Mininet testing evidence.

### Additional Documentation
- `D2_commands.txt` – Project setup and execution commands.
- `README.md` – Main project overview and documentation.

## Team Contributions

| Team Member | Contributions |
|---|---|
| **Member 1 (Nayonika V)** | Project planning and coordination, UDP client implementation, GUI development, file-transfer testing, SDN integration, Mininet topology setup and testing, and overall D2 implementation. |
| **Member 2 (Pooja)** | D1 and D2 documentation, organisation of screenshots and test evidence, documentation of Mininet results and OpenFlow flow-table outputs, and preparation of project deliverables. |
| **Member 3 (Prajna)** | Implementation of the UDP server-side code during D1. |

*Contributions are listed based on the work completed by each team member.*

## 12. Future Enhancements

- Complete dynamic SDN-based rerouting.
- Improve network recovery during link failures.
- Extend testing across different network topologies.
- Add detailed performance comparisons.
- Improve the GUI with live network-path visualisation.

## 13. Conclusion

This project demonstrates how reliability mechanisms can be implemented over UDP and combined with Software-Defined Networking to improve file-transfer behaviour.

It brings together socket programming, network emulation, SDN control, graphical interfaces, and performance evaluation.

---

**Project:** Reliable File Transfer using UDP and SDN  
**Department:** Computer Science and Engineering  
**Institution:** PES University  
**Academic Year:** 2026–2027
