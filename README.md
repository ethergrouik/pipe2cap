# pipe2cap
Forwards stdin network frames to a local adapter on Windows using pcap.

You will be able to:
- use apps that cannot use stdin as a pcap capture source;
- use as many apps as you want in the same time with only one remote network frame capture flow.

## How to:
- Add a MS loopback adapter to your Windows box;
- Disable all network properties of this new adapter;
- Install winpcap again in order to add this new adapter to the pcap interfaces list;
- Start pipe2cap.exe without any argument to list adapters:

  Available interfaces:
  
  0 : \Device\NPF_{2AC8F83E-C9B9-45EF-AF30-46CAAA4991CE} (Intel(R) PRO/1000 MT)
  
  1 : \Device\NPF_{FC6032FA-D434-43A2-A34F-D792BB3EF7B4} (MS LoopBack Driver)

- Connect to your linux remote box like this:

  plink.exe -ssh -pw password login@192.168.1.1 "tcpdump -n -s 0 -i eth0 -w - not port ssh" | *path_to_file*\pipe2cap.exe 1

  **=>** it will forward frames captured by tcpdump on 192.168.1.1 (except ssh) to your local MS loopback adapter identified by its id ( 1 ).
- Start your favorites tools (ie: Wireshark, Ethergrouik, NetworkMiner...) and listen to your MS loopback adapter 

## How to add MS Loopback adapter:
https://technet.microsoft.com/en-us/library/cc708322%28v=ws.10%29.aspx

Enjoy!!!
