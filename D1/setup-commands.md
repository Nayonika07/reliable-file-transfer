# Setup and Run Instructions

1. Requirements

The project uses the following environment:

MacBook running the POX SDN controller

Ubuntu virtual machine running Mininet

Python 3

GCC compiler

Mininet

Open vSwitch

POX SDN Controller

OpenFlow 1.0

## 2. Compile the C Programs

From the repository root, run:

```bash
gcc client/udp_client.c -o udp_client
gcc server/udp_server.c -o udp_server
```

Make sure the compiled executables and test file are available inside the appropriate Mininet hosts.

3. POX Controller Setup

POX is used as the SDN controller in the current project setup.

The controller runs on the MacBook, independently of the Ubuntu virtual machine running Mininet.

The tested configuration uses:

Controller: POX 0.7.0 (gar)

Controller IP: 192.168.29.177

Controller port: 6633

OpenFlow version: 1.0

Controller component: forwarding.l2_learning

The controller was successfully started on the MacBook.

Python compatibility note: POX started successfully in the tested environment using Python 3.14. However, a compatibility warning was displayed because the version used recommends Python 3.6–3.9. This should be considered if compatibility issues arise.

Keep this terminal open. If your virtual environment has a different path, use that path instead.

## 4. Start Mininet

In a separate terminal:

```bash
sudo mn -c
sudo mn --topo single,2 --mac --switch ovs,protocols=OpenFlow13 --controller remote,ip=127.0.0.1,port=6633
```

## 5. Test Connectivity

At the `mininet>` prompt:

```text
nodes
net
pingall
```

Expected result: all hosts can reach each other with 0% packet loss.

## 6. Run the UDP File Transfer

Start the server on h2:

```text
h2 ./udp_server 5000 /tmp/received.txt
```

In another Mininet prompt, send the file from h1:

```text
h1 ./udp_client 10.0.0.2 5000 test.txt
```

Ensure the executables and `test.txt` are accessible from those hosts.

## 7. Verify the Switch Flow Table

After generating traffic:

```text
sh ovs-ofctl -O OpenFlow13 dump-flows s1
```

## 8. Verify the Received File

Compare the original and received files after making both accessible from the same shell:

```bash
cmp test.txt received.txt
```

If `cmp` prints nothing and exits successfully, the files match.

## Notes
The commands above document the project's C UDP client/server and Mininet/Ryu setup. Follow the actual file locations and virtual-environment path used in your tested environment.
