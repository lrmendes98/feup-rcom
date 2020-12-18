# tux22 on our workbench == tux2
# NOTA:
# tux23 E0: Porta 1
# tux24 E0: Porta 2
# tux24 E1: Porta 3
# tux22 E0: Porta 4

ifconfig eth0 down
ifconfig eth0 172.16.21.1/24

# Rotas
route add -net 172.16.20.0/24 gw 172.16.21.253
route add default gw 172.16.21.254

echo 0 > /proc/sys/net/ipv4/conf/eth0/accept_redirects and echo 0 > /proc/sys/net/ipv4/conf/all/accept_redirects