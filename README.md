# RCOM

# Virtual Port commands
sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777

# FTP test links
ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt  
ftp://anonymous:1@netlab1.fe.up.pt/pub.txt  
ftp://netlab1.fe.up.pt/pub.txt  
ftp://rcom:rcom@netlab1.fe.up.pt/files/pic1.jpg  
ftp://rcom:rcom@netlab1.fe.up.pt/files/pic2.png  
ftp://rcom:rcom@netlab1.fe.up.pt/files/crab.mp4  


### test big files (must uncomment code lines at open_socket_and_connect_server)
ftp://speedtest.tele2.net/1KB.zip