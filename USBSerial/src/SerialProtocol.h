#include <DriverStation.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#ifndef SIR_USB
#define SIR_USB

class SerialUSB {
public:

	void test();
	void Write(char* data);
	const char* Read();
	void disable();
	enum Port{
			kUSB=0,
			kUSB1=1,
			kUSB2=2
	};
	SerialUSB(SerialUSB::Port port=SerialUSB::kUSB);
	~SerialUSB();
private:

	void restartAfterSuspendBit();
	void restartBit();
	void stopBit();
	void suspendBit();
	void FlushInput();
	void FlushOutput();
	void FlushIO();
	char* ports[3]={"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2"};
	struct termios config;
	int sPort;
	char initVal[1];
	char recVal[1];
	enum stat{
		none=0,
		initialize=1,
		data=2,
		end=3
	};

	DriverStation* ds;
	typedef stat st;
	st status;
	bool testEnabled;
	void init(char* byte, char* data);
	int PACKETSIZE=60;

};
#endif



