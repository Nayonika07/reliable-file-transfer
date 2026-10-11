SDN-Based UDP File Transfer Using Mininet and Ryu

Objective

To test host connectivity in Mininet, transfer a file using UDP, and inspect the OpenFlow flow table using the Ryu SDN controller.

Tools Used

- Ubuntu
- Mininet
- Open vSwitch
- Ryu SDN Controller
- OpenFlow 1.3
- Python 3

Network Topology

- One switch: s1
- Two hosts: h1 and h2
- h1 IP address: 10.0.0.1
- h2 IP address: 10.0.0.2
- Controller: Ryu on port 6633

Experiments and Results

1. Connectivity Test

Command: "pingall"

Result: 0% packet loss (2/2 hosts received).

2. UDP File Transfer

A text file was sent from h1 to h2 using Python UDP sockets.

Result: 71 bytes sent and received successfully. The received file contents were verified.

3. OpenFlow Flow Table

Command: "ovs-ofctl -O OpenFlow13 dump-flows s1"

The final flow-table output is saved in "final-flows.txt".

Conclusion

The Mininet hosts communicated successfully under the Ryu controller. A UDP file transfer from h1 to h2 was completed, and the switch flow table was inspected using OpenFlow 1.3.
