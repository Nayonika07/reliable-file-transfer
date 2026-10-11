UDP File Transfer Test Results

Objective

To test UDP file transfer between two hosts in a Mininet network controlled by Ryu.

Network

- Sender: h1 (10.0.0.1)
- Receiver: h2 (10.0.0.2)
- Switch: s1
- Controller: Ryu
- Protocol: UDP

Test Procedure

1. Start the Ryu SDN controller.
2. Start Mininet with two hosts and an OpenFlow 1.3 switch.
3. Generate network traffic and verify connectivity using "pingall".
4. Run the UDP receiver on h2.
5. Send the text file from h1 to h2.
6. Check the received file contents.

Observed Result

- UDP sender reported 71 bytes sent.
- UDP receiver reported 71 bytes received.
- The received file contents were checked successfully.
- The Mininet connectivity test reported 0% packet loss.

Conclusion

The UDP file transfer between h1 and h2 completed successfully in the Mininet network.

Evidence

The final OpenFlow flow-table output is documented in "final-flows.txt". Setup commands are documented in "setup-commands.md".
