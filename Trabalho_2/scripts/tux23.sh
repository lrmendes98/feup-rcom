# tux23 on our workbench == tux1 
# NOTA:
# tux23 E0: Porta 1
# tux24 E0: Porta 2
# tux24 E1: Porta 3
# tux22 E0: Porta 4

ifconfig eth0 down
ifconfig eth0 172.16.20.1/24
arp -d <ipaddress>

# Rotas
route add -net 172.16.21.0/24 gw 172.16.20.254
route add default gw 172.16.20.254