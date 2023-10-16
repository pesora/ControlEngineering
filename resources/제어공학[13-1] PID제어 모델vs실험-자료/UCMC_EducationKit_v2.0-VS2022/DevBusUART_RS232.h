#pragma once
#include "Windows.h"

class DevBusUART_RS232
{
public:
	DevBusUART_RS232(char *port);
	virtual ~DevBusUART_RS232();

private:
	char *m_port;
	HANDLE m_handle; 

public:
	virtual int Open(unsigned long timeOut = 10);
	virtual int Close();
	virtual int Read(long *id, unsigned char *data, int len);
	virtual int Write(long id, unsigned char *data, int len);
	virtual int Flush();
	virtual int CountRead();

	virtual int SetTimeOut(int readTimeout, int writeTimeout, int readIntervalTimeout);
	virtual int SetTimeOut(int timeOut);
	virtual int GetTimeOut(int *timeOut);
	virtual int SetBaudRate(int baudRate);
	virtual int GetBaudRate(int *baudRate);
};
