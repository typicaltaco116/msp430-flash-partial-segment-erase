echo "115200 BAUD 8|1"
stty -F /dev/ttyACM1 115200 cs8 -parenb -cstopb
echo "Press ctl + c to quit"
cat /dev/ttyACM1
