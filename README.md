# pipe2cap
Forward stdin network frames to a local adapter on Windows using pcap.

# How to:
- Add a MS loopback adapter to your Windows box ( let's image that its id will be '1' );
- Disable all network properties of this new adapter;
- Install winpcap again in order to add this new adapter to the pcap interfaces list;
- Connect to your linux remote box like this:
plink.exe -ssh -pw password login@192.168.1.1 "tcpdump -n -s 0 -i eth0 -w - not port ssh" | c:\users\user\Downloads\pipe2cap.exe 1
=> it will forward frames captured by tcpdump on 192.168.1.1 (except ssh) to your local MS loopback adapter identified by its id ( 1 ).
- Start your favorites tools and listen to your MS loopback adapter (ie: Wireshark, Ethergrouik, NetworkMiner...)

Enjoy!!!
