#!bin/bash

ifconfig eth0 172.16.40.254/24
ifconfig eth1 172.16.41.253/24
route add default gw 172.16.41.254/24
route -n
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
echo "services.netlab.fe.up.pt 172.16.1.1" > /etc/resolv.conf