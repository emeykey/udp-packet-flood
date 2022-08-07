# udp-packet-flood
Floods target IP and port with size specifiable UDP packets

Compile with g++ main.cpp -o packetspam

Usage: ./packetspam \<ip\> \<port\> \<bytes\> \[speed_in_mbps_default_max\]
  
Example: ./packetspam 192.168.0.1 80 4096 100
