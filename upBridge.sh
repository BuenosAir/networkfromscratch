ip link add name nn type bridge
ip link set nn up
ip link set eth0 master nn 
ip link set tap0 master nn
