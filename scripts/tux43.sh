#!bin/bash

ifconfig eth0 172.16.40.1/24
route add -net 172.16.41.1/24 gw 172.16.40.254
route add default gw 172.16.40.254
route -n
echo "services.netlab.fe.up.pt 172.16.1.1" > /etc/resolv.conf