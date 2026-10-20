# UDP File Transfer Test Results

## 1. Objective
To test file transfer between a UDP client and server written in C.

## 2. Test Environment
- C UDP client: `client/udp_client.c`
- C UDP server: `server/udp_server.c`
- Network: Mininet with two hosts, h1 and h2
- SDN controller: Ryu using OpenFlow 1.3

## 3. Test Procedure
1. Compile the C client and server.
2. Start the Ryu controller.
3. Start the Mininet topology.
4. Check connectivity using `pingall`.
5. Run the UDP server on h2.
6. Send a test file from h1 to h2 using the UDP client.
7. Compare the original and received files using `cmp`.

## 4. Expected Results
- Mininet hosts can communicate successfully.
- The UDP client sends the test file to the UDP server.
- The server saves the received data to an output file.
- If `cmp` exits successfully without printing differences, the original and received files match.

## 5. Evidence
Screenshots of Mininet connectivity, SDN flow-table output, and file-transfer testing are included in the repository.

## 6. Conclusion
The test verifies UDP file transfer and its operation within the Mininet network when the documented test completes successfully.
