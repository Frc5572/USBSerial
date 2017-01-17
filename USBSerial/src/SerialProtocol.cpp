#include "SerialProtocol.h"
#include <iostream>
#include <string.h>
#include <Timer.h>
#include <DriverStation.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace frc;


SerialUSB::SerialUSB(SerialUSB::Port port){
		//system("stty -F /dev/ttyUSB0 115200 intr ^C quit '^\\' erase ^? kill ^U eof ^D start ^Q stop ^S susp ^Z rprnt ^R werase ^W flush ^O");
		sPort=open(ports[port], O_RDWR | O_NOCTTY );
		config.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK |  ISTRIP | ICRNL | INLCR | IUCLC | IXANY | IMAXBEL | IUTF8 | ICANON | IEXTEN) | IXON | IXOFF | ISIG;
		config.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS) | CS8 | HUPCL | CREAD | CLOCAL;
		config.c_lflag &= ~( ECHO | ECHONL | NOFLSH | XCASE | TOSTOP | ECHOPRT ) | ECHOE | ECHOK | ECHOCTL | ECHOKE ;
		config.c_oflag &= ~(OLCUC | OCRNL | ONOCR | ONLRET | OFILL | OFDEL) | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0;
		config.c_cc[VMIN]=PACKETSIZE;
		config.c_cc[VTIME]=0;
		config.c_cc[VINTR]=(char)003;
		config.c_cc[VQUIT]=(char)034;
		config.c_cc[VERASE]=(char)177;
		config.c_cc[VKILL]=(char)025;
		config.c_cc[VEOF]=(char)004;
		config.c_cc[VSTART]=(char)021;
		config.c_cc[VSTOP]=(char)023;
		config.c_cc[VSUSP]=(char)032;
		config.c_cc[VREPRINT]=(char)022;
		config.c_cc[VWERASE]=(char)027;
		//config.c_cc[VFLUSH] = '^O';
		if(cfsetispeed(&config,B115200)<0){
			std::cout<<"error: cfsetispeed\n";
		}else{
			std::cout<<"Input Speed Set\n";
		}
		if(cfsetospeed(&config,B115200)<0){
					std::cout<<"error: cfsetispeed\n";
		}else{
			std::cout<<"Output Speed Set\n";
		}
		if(tcsetattr(sPort, TCSANOW, &config)<0){
			std::cout<<"error: tcsetattr\n";
		}else{
			std::cout<<"Setting pushed to file descriptor\n";
		}
		if(sPort!=-1){
			status=SerialUSB::none;
			initVal[0] = 0b01010101;
			recVal[0] = 0b10101010;
			ds=&DriverStation::GetInstance();
			testEnabled=true;

		}else{
			std::cout<<"Failed to open port\n";
		}
		system("tty");
		system("stty -F /dev/ttyUSB0 -a");
}
SerialUSB::~SerialUSB(){
	close(sPort);
}
void  SerialUSB::Write(char* data){
	init(initVal, data);
}
const char* SerialUSB::Read(){

	return "null";
}
void SerialUSB::init(char* byte, char* data){
	if(status==SerialUSB::none){
		status=SerialUSB::initialize;
		while(ds->IsEnabled()){
			char buffer[PACKETSIZE];
			int n = read(sPort, buffer, PACKETSIZE);
			if (n < 0)
				fputs("read failed!\n", stderr);
			std::cout<<buffer;
			//write(sPort,buf,sizeof(buf)+2); Write test= Success
		}
		std::cout << "eol" << std::endl;
	}
}
void SerialUSB::test(){
	init(initVal,"NO dawta");
}
void SerialUSB::disable(){
	testEnabled=false;
}
void SerialUSB::restartAfterSuspendBit(){
	tcflow(sPort,TCOON);//Output On
}
void SerialUSB::restartBit(){
	tcflow(sPort,TCION);//Input ON
}
void SerialUSB::stopBit(){
	tcflow(sPort,TCIOFF);//Input Off
}
void SerialUSB::suspendBit(){
	tcflow(sPort,TCOOFF);//Output Off
}
void SerialUSB::FlushInput(){
	tcflush(sPort,TCIFLUSH);
}
void SerialUSB::FlushOutput(){
	tcflush(sPort,TCOFLUSH);
}
void SerialUSB::FlushIO(){
	tcflush(sPort,TCIOFLUSH);
}
