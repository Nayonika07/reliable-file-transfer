Mininet and Ryu Setup Commands

1. Clean up Mininet

sudo mn -c

2. Start the Ryu SDN Controller

Activate the Python virtual environment:

source ~/ryu39-venv/bin/activate

Start the controller:

ryu-manager --verbose --ofp-tcp-listen-port 6633 ryu.app.simple_switch_13

Keep the controller terminal running.

3. Start Mininet

Open a separate Ubuntu terminal and run:

sudo mn --topo single,2 --mac --switch ovs,protocols=OpenFlow13 --controller remote,ip=127.0.0.1,port=6633

4. Test connectivity

At the "mininet>" prompt:

pingall

Expected result: 0% packet loss.

5. Capture the final OpenFlow table

After generating traffic:

sh ovs-ofctl -O OpenFlow13 dump-flows s1

Save the output:

sh ovs-ofctl -O OpenFlow13 dump-flows s1 > /tmp/final-flows.txt

Display the saved output:

sh cat /tmp/final-flows.txt

6. UDP File Transfer

A Python UDP sender on h1 sends a text file to a Python UDP receiver on h2.

Verified result: 71 bytes sent and received successfully, and the received file contents were checked.

Conclusion

The Mininet hosts communicated through an OpenFlow 1.3 switch controlled by Ryu. Connectivity, UDP file transfer, and the switch flow table were tested.
