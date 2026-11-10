# Multi-Host Network Implementation

## 1. Overview

The project uses a multi-host network topology created using Mininet and managed by a Software Defined Networking (SDN) controller.

The topology connects multiple hosts through OpenFlow-enabled switches, allowing communication across alternative network paths.

## 2. Network Topology

The network consists of:

- **h1:** Source host
- **h2:** Destination host
- **s1, s2, s3, s4:** OpenFlow switches
- **POX Controller:** Controls forwarding decisions and monitors network links.

## 3. Network Architecture

Two alternative paths connect the source and destination:

**Primary path:**
h1 → s1 → s2 → s4 → h2

**Backup path:**
h1 → s1 → s3 → s4 → h2

## 4. SDN Controller Functionality

The custom POX controller (`reliable_sdn.py`) implements:

- Switch connection handling.
- Link discovery and monitoring.
- Host learning.
- Shortest-path calculation using Breadth-First Search (BFS).
- Forwarding rule installation.
- Network topology updates when links fail.

## 5. Multi-Host Communication

Mininet provides the virtual hosts used to test communication across the network.

Host-to-host connectivity is tested using ICMP ping commands.

Example:

```bash
h1 ping -c 10 h2
```

## 6. Dynamic Routing

The topology provides two alternative paths between the source and destination.

When a link failure occurs, the controller is designed to detect the topology change, clear outdated forwarding rules, and calculate an alternative route.

## 7. Testing and Validation

The initial connectivity test achieved:

- Source: h1
- Destination: h2
- Packets transmitted: 5
- Packets received: 5
- Packet loss: 0%

Automatic rerouting after link failure is subject to separate validation.

## 8. Files Used

| File | Purpose |
|---|---|
| `d2_topology.py` | Creates the Mininet network |
| `ext/reliable_sdn.py` | Implements the POX SDN controller |
| UDP client/server source files | Implement file transfer |

## 9. Conclusion

The multi-host Mininet environment provides a test platform for evaluating reliable communication and dynamic SDN routing under normal and link-failure conditions.
