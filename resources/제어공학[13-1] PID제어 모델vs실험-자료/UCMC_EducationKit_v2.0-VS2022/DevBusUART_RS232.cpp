#include "stdafx.h"

#include "DevBusUART_RS232.h"

DevBusUART_RS232::DevBusUART_RS232(char *port)
{
	m_port = port;
	m_handle = INVALID_HANDLE_VALUE;
}

DevBusUART_RS232::~DevBusUART_RS232()
{
	if(m_handle != INVALID_HANDLE_VALUE) {
		CloseHandle(m_handle);
	}
	m_handle = INVALID_HANDLE_VALUE;
}

int DevBusUART_RS232::Open(unsigned long timeOut)
{
	m_handle = CreateFile (m_port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(m_handle == INVALID_HANDLE_VALUE){
		return -1;
	}

	DCB m_dcb;
	BOOL bRet = GetCommState(m_handle, &m_dcb);
	if(bRet == 0){
		goto RET_ERROR;
	}
	m_dcb.DCBlength = sizeof(DCB);
	m_dcb.BaudRate = 115200;
	m_dcb.ByteSize = 8;
	m_dcb.Parity = 0;
	m_dcb.StopBits = 0;
	m_dcb.fBinary = TRUE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fParity = 0;
	m_dcb.fOutX = false;
	m_dcb.fInX = false;
	m_dcb.fNull = false;
	m_dcb.fAbortOnError = false;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxDsrFlow = false;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxCtsFlow = false;

	bRet = SetCommState(m_handle, &m_dcb);
	if(bRet == 0){
		goto RET_ERROR;
	}

	SetTimeOut(timeOut);
	return 0;

RET_ERROR:
	CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;

	return -1;
}

int DevBusUART_RS232::Close()
{
	if(m_handle != INVALID_HANDLE_VALUE) {
		CloseHandle(m_handle);
	}

	m_handle = INVALID_HANDLE_VALUE;

	return 0;
}

int DevBusUART_RS232::Read(long *id, unsigned char *data, int len)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	DWORD dwBytesRead = 0;

	if (ReadFile (m_handle, data, len, &dwBytesRead, NULL)) {
		return (int)dwBytesRead;
	}

	return -1;
}

int DevBusUART_RS232::Write(long id, unsigned char *data, int len)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	DWORD dwBytesWritten = 0;

	if(WriteFile(m_handle, data, len, &dwBytesWritten, NULL)) {
		return (int)dwBytesWritten;
	}

	return -1;
}

int DevBusUART_RS232::Flush()
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}
	COMSTAT Stat;
	DWORD iError;

	ClearCommError (m_handle, &iError, &Stat);
	PurgeComm (m_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	return 0;
}

int DevBusUART_RS232::CountRead ()
{
	if (m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	COMSTAT Stat;
	DWORD iError;
	int ret = ClearCommError(m_handle, &iError, &Stat);
	if (ret ) {
		return Stat.cbInQue;
	}
	return 0;
}

int DevBusUART_RS232::SetTimeOut(int readTimeout, int writeTimeout, int readIntervalTimeout)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	COMMTIMEOUTS m_CommTimeouts;

	BOOL bRet = GetCommTimeouts (m_handle, &m_CommTimeouts);
	if(!bRet) {
		return -1;
	}
	m_CommTimeouts.ReadIntervalTimeout = readIntervalTimeout;
	m_CommTimeouts.ReadTotalTimeoutConstant = readTimeout;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeouts.WriteTotalTimeoutConstant = writeTimeout;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;

	bRet = SetCommTimeouts (m_handle, &m_CommTimeouts);
	if (!bRet){
		return -1;
	}
	return 0;
}

int DevBusUART_RS232::SetTimeOut(int timeOut)
{
	return SetTimeOut (timeOut, timeOut, 1);
}

int DevBusUART_RS232::GetTimeOut(int *timeOut)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	return 0;
}

int DevBusUART_RS232::SetBaudRate(int baudRate)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	DCB dcb;
	BOOL bRet = GetCommState(m_handle, &dcb);
	dcb.BaudRate = baudRate;
	bRet = SetCommState(m_handle, &dcb);

	Flush ();

	return 0;
}

int DevBusUART_RS232::GetBaudRate(int *baudRate)
{
	if(m_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	DCB dcb;
	BOOL bRet = GetCommState(m_handle, &dcb);
	*baudRate = dcb.BaudRate;

	return 0;
}