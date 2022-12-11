#!bin/bash

ifconfig eth0 172.16.41.1/24
route add -net 172.16.40.1/24 gw 172.16.41.253
route add default gw 172.16.41.254/24
route -n
echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects
echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects
echo "services.netlab.fe.up.pt 172.16.1.1" > /etc/resolv.conf