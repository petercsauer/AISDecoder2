# Serial Read using MAR lib
# J Mehlman 7/21/21
CC=g++
CFLAGS=-Wall

commsEx : main.o serialEx.o decoder.o udpEx.o SerialPort.o SocketUdpClient.o PacketHlcOcu.o HlcOcuStateVehicle.o PacketLittleEndian.o Packet.o
	$(CC) $(CFLAGS) -o commsEx main.o serialEx.o decoder.o udpEx.o SerialPort.o SocketUdpClient.o PacketHlcOcu.o HlcOcuStateVehicle.o PacketLittleEndian.o Packet.o

main.o : main.cpp 
	$(CC) $(CFLAGS) -c main.cpp 

decoder.o : decoder.cpp 
	$(CC) $(CFLAGS) -c decoder.cpp 

serialEx.o : serialEx.cpp
	$(CC) $(CFLAGS) -c serialEx.cpp

udpEx.o : udpEx.cpp
	$(CC) $(CFLAGS) -c udpEx.cpp

# Our serial port lib
SerialPort.o : ../legos/Serial/SerialPort.cpp
	$(CC) $(CFLAGS) -c ../legos/Serial/SerialPort.cpp

# Our udp/ip lib
SocketUdpClient.o : ../legos/Socket/SocketUdpClient.cpp
	$(CC) $(CFLAGS) -c ../legos/Socket/SocketUdpClient.cpp

# our packet lib
# The general packet and tools
PacketHlcOcu.o : ../HlcOcuMsgs/PacketHlcOcu.cpp
	$(CC) $(CFLAGS) -c ../HlcOcuMsgs/PacketHlcOcu.cpp
Packet.o : ../legos/Packet/Packet.cpp
	$(CC) $(CFLAGS) -c ../legos/Packet/Packet.cpp
PacketLittleEndian.o : ../legos/Packet/PacketLittleEndian.cpp
	$(CC) $(CFLAGS) -c ../legos/Packet/PacketLittleEndian.cpp

# The vehicle state packet (one of many)
HlcOcuStateVehicle.o : ../HlcOcuMsgs/HlcOcuStateVehicle.cpp
	$(CC) $(CFLAGS) -c ../HlcOcuMsgs/HlcOcuStateVehicle.cpp


clean : 
	rm commsEx main.o decoder.o serialEx.o udpEx.o SerialPort.o SocketUdpClient.o PacketHlcOcu.o HlcOcuStateVehicle.o Packet.o
