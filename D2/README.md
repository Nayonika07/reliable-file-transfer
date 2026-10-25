# D2: Reliable File Transfer Using UDP and SDN

## 1. Overview

The objective of D2 is to improve the reliability of file transfer over UDP and evaluate the application in an SDN-controlled network environment.

UDP provides connectionless communication but does not guarantee packet delivery, ordering, or retransmission. This project introduces a reliability mechanism at the application layer and will subsequently evaluate its behaviour under different network conditions.

## 2. Objectives

- Improve reliable file delivery using UDP sockets.
- Implement packet sequence numbers and acknowledgements (ACKs).
- Introduce timeout-based retransmission for unacknowledged packets.
- Detect duplicate packets to prevent duplicate data from being written to the output file.
- Verify file integrity after transfer.
- Integrate and evaluate the application in a Mininet topology controlled by an SDN controller.
- Compare performance under different network conditions.

## 3. Implementation Progress

### Component 1: Application Reliability

**Status: Initial implementation and basic testing completed.**

The client and server applications have been updated to use a simple Stop-and-Wait ARQ approach.

Implemented features:
- Packet sequence numbers.
- ACK messages from the receiver.
- A timeout mechanism for waiting for ACKs.
- Retransmission attempts when an ACK is not received.
- Duplicate packet detection at the receiver.
- File transfer using UDP sockets.
- Basic validation of packet headers and payload lengths.

### Basic Testing

A local file transfer was tested using the UDP client and server.

The received file was compared with the original using the `cmp` command. The command returned exit status `0`, indicating that the two files matched exactly.

**Note:** Packet-loss and retransmission behaviour still require dedicated testing. Additional protocol edge cases will also be checked before the reliability component is considered complete.

## 4. Testing Evidence

Screenshots and test evidence will be maintained in the `D2/screenshots/` directory.

Planned evidence includes:
- Successful compilation of the updated client and server.
- Client output showing packet transmission and ACK reception.
- Server output showing packet reception.
- File integrity verification.
- Retransmission behaviour under simulated packet loss.
- Further tests under Mininet and SDN control.

## 5. Remaining Work

- Test transfers involving larger files and different file sizes.
- Simulate packet loss and verify retransmission behaviour.
- Test timeout and retry-limit handling.
- Verify correct handling of duplicate packets and end-of-file acknowledgement.
- Integrate the updated application with the Mininet topology and SDN controller.
- Evaluate performance under multiple network conditions.
- Document the experimental results and compare the relevant measurements.

## 6. Project Structure

- `client/`: UDP client implementation.
- `server/`: UDP server implementation.
- `sdn/`: SDN controller code and related configuration.
- `tests/`: Test files and testing scripts, as applicable.
- `screenshots/`: Existing project evidence.
- `D2/`: D2 progress documentation and supporting evidence.

## 7. Reproduction

Detailed setup and execution instructions, test commands, screenshots, and final results will be added as the implementation progresses.

## 8. Final D2 Summary

This section will be updated after all D2 components have been implemented and tested. It will summarise the final functionality, SDN integration, challenging network scenarios, performance evaluation, and conclusions.
