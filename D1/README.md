# Reliable File Transfer Using UDP and SDN

## 1. Project Overview
This project implements file transfer between a client and a server using UDP sockets in C. The application is tested in a Mininet network controlled by the Ryu SDN controller.

## 2. Architecture
The network consists of:
- **h1:** UDP client
- **s1:** Open vSwitch
- **h2:** UDP server
- **Ryu:** SDN controller using OpenFlow 1.3

Data path: `h1 → s1 → h2`

## 3. Technologies Used
- C programming
- UDP sockets
- Mininet
- Open vSwitch
- Ryu SDN controller
- OpenFlow 1.3

## 4. Project Structure
- `client/udp_client.c` : UDP client
- `server/udp_server.c` : UDP server
- `sdn/` : SDN-related files
- `docs/` : Project screenshots
- `setup-commands.md` : Setup and execution instructions

## 5. Compilation
Compile the client and server from the repository root:

```bash
gcc client/udp_client.c -o udp_client
gcc server/udp_server.c -o udp_server
```

## 6. Running and Testing
Follow `setup-commands.md` for environment setup, starting the Ryu controller and Mininet, and running the UDP file transfer.

Verify the transferred file by comparing the original and received files using `cmp`.

## 7. Test Evidence
Screenshots and test results are included in the repository.

## 8. Current Scope
This submission covers the initial UDP file transfer implementation and its integration with the Mininet and SDN environment.
```
