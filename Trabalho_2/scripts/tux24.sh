# tux4 == tux24 in our workbench
# NOTA:
# tux23 E0: Porta 1
# tux24 E0: Porta 2
# tux24 E1: Porta 3
# tux22 E0: Porta 4

# Configurar eth0
ifconfig eth0 down
ifconfig eth0 172.16.20.254/24

# Configurar eth1
ifconfig eth1 down
ifconfig eth1 172.16.20.253/24
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

# Rotas
route add default gw 172.16.21.254