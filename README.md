# pipe2cap
Transfers remote network frames to a local adapter on Windows (using winpcap) or Linux.

You will be able to:
- use apps that cannot use stdin as a pcap capture source;
- use as many apps as you want at the same time with only one remote network frame capture flow.

## How to:
### For Windows boxes:
- Add a MS loopback adapter to your Windows box;
- Disable all network properties of this new adapter;
- Reinstall winpcap or reboot your computer in order to add this new adapter to the pcap interface list;
- Start pipe2cap.exe without any argument to list adapters:

  Available interfaces:
  
  0 : \Device\NPF_{2AC8F83E-C9B9-45EF-AF30-46CAAA4991CE} (Intel(R) PRO/1000 MT)
  
  1 : \Device\NPF_{FC6032FA-D434-43A2-A34F-D792BB3EF7B4} (MS LoopBack Driver)

- Connect to your linux remote box like this:

  plink.exe -ssh -pw password login@192.168.1.1 "tcpdump -n -s 0 -i eth0 -w - not port ssh" | *path_to_file*\pipe2cap.exe 1

  **=>** it will forward frames captured by tcpdump on 192.168.1.1 (except ssh) to your local MS loopback adapter identified by its id ( 1 ).
- Start your favorite tools (ie: Wireshark, Ethergrouik, NetworkMiner...) and listen to your MS loopback adapter 

### For Linux boxes:
- Add a dummy iface: sudo modprobe dummy; (Thanks Max!)
- Start the iface: sudo ifconfig dummy0 up;
- Start pipe2cap without any argument to list adapters: sudo ./pipe2cap;

  Available interfaces:
  
 0 : enp6s0	((null))
 
 1 : dummy0	((null))
 
 2 : any	(Pseudo-device that captures on all interfaces)
 
 3 : lo	((null))
 
 4 : vboxnet0	((null))
 
 5 : bluetooth-monitor	(Bluetooth Linux Monitor)
 
 6 : nflog	(Linux netfilter log (NFLOG) interface)
 
 7 : nfqueue	(Linux netfilter queue (NFQUEUE) interface)
 
 8 : usbmon1	(USB bus number 1)
 
 9 : usbmon2	(USB bus number 2)
 
 10 : usbmon3	(USB bus number 3)
 
 11 : usbmon4	(USB bus number 4)
 
 12 : enx0050b60be189	((null))

- Connect to the remote linux box like this:

  sshpass -p password ssh login@192.168.1.1 "tcpdump -n -s 0 -i eth0 -w - not port ssh" | sudo ./pipe2cap 1
  
  **=>** it will forward frames captured by tcpdump on 192.168.1.1 (except ssh) to your dummy0 iface identified by its id ( 1 ).

## How to add a MS Loopback adapter:
https://technet.microsoft.com/en-us/library/cc708322%28v=ws.10%29.aspx

## How to build the project:
### On Windows boxes:
- Download and install [Codeblocks](http://www.codeblocks.org/downloads) (codeblocks-16.01mingw-setup.exe was used to realize this project);
- Load the Codeblocks project file pipe2cap.cbp
- Winpcap dependencies are already present in the project.
 
### On Linux boxes:
- sudo apt-get install libpcap-dev
- compile: gcc -o pipe2cap main.c -lpcap

Enjoy!!!
