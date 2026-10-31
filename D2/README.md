# D2 - Reliable File Transfer using UDP and SDN

## 1. Overview

The objective of this project is to develop a reliable file transfer system using UDP and Software Defined Networking (SDN).

UDP does not provide built-in reliability mechanisms such as acknowledgements or retransmissions. This project implements reliability at the application layer and integrates an SDN-controlled network using the POX controller and Mininet.

## 2. Network Architecture

The project uses the following network topology:

- **h1:** Sender (UDP client)
- **s1:** Open vSwitch
- **h2:** Receiver (UDP server)
- **c0:** SDN controller connection (POX)

The UDP client sends files from h1 to h2 through the Open vSwitch, which is controlled by POX.

## 3. Technologies Used

- C programming
- UDP socket programming
- Mininet
- Open vSwitch (OVS)
- POX SDN Controller
- OpenFlow 1.0
- Ubuntu

## 4. Implementation

### UDP Client and Server

The client and server are implemented in C using UDP sockets.

The implementation includes:
- File reading and transmission in chunks.
- Sequence numbers for packet identification.
- Acknowledgements (ACKs).
- Retransmission mechanism for reliable delivery.
- Binary file handling.

### SDN Integration

POX is used as the SDN controller.

Mininet is configured with an Open vSwitch connected to the POX controller. The network topology consists of two hosts communicating through the switch.

## 5. Testing and Results

The following tests have been performed:

| Test | Status |
|---|---|
| POX controller startup | Successful |
| Mininet topology creation | Successful |
| Switch connection to POX | Successful |
| Host connectivity using pingall | Successful |
| UDP file transfer through Mininet | Successful |
| Image file integrity verification using cmp | Successful |
| Audio file transfer | In progress |
| Video file transfer | Pending |
| Packet-loss and retransmission testing | Pending |

### Successful Image Transfer

An image file was transferred from h1 to h2 through the POX-controlled Mininet topology.

The received file was verified against the original using the `cmp` command, confirming that both files matched.

## 6. Evidence

Screenshots and test results are maintained in this directory to document:

- POX controller execution.
- Mininet topology and switch connectivity.
- Successful host communication.
- UDP file transfer.
- File integrity verification.

## 7. Future Work

- Test audio and video file transfers.
- Perform controlled packet-loss experiments.
- Evaluate retransmission behaviour.
- Record transfer performance and packet statistics.
- Improve the user interface for file selection and transfer monitoring.

## 8. Conclusion

The D2 implementation demonstrates UDP-based file transfer through a Mininet network controlled by the POX SDN controller. Initial image transfer and file integrity verification have been completed successfully. Further testing will evaluate different file formats and reliability under packet-loss conditions.
