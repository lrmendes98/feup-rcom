# NOTA :
# tux23 E0: Porta 1
# tux24 E0: Porta 2
# tux24 E1: Porta 3
# tux22 E0: Porta 4

# Criar vlan20
configure terminal
vlan 20
end
show vlan id 20 # confirmar

# Criar vlan21
configure terminal
vlan 21
end
show vlan id 21 # confirmar

# Configurar porta 1 (tux23) ligada a vlan20 no switch 
configure terminal
interface fastethernet 0/1
switchport mode access
switchport access vlan 20
end
show running-config interface fastethernet 0/1
# confirmar
show interfaces fastethernet 0/1 switchport 

# Configurar porta 2 (tux24) ligada a vlan20 no switch 
configure terminal
interface fastethernet 0/2
switchport mode access
switchport access vlan 20
end
show running-config interface fastethernet 0/2
# confirmar
show interfaces fastethernet 0/2 switchport 

# Configurar porta 4 (tux22) ligada a vlan21 no switch 
configure terminal
interface fastethernet 0/4
switchport mode access
switchport access vlan 21
end
show running-config interface fastethernet 0/4
# confirmar
show interfaces fastethernet 0/4 switchport