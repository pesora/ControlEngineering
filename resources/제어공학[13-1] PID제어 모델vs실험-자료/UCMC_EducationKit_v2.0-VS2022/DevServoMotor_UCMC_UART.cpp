#include "stdafx.h"

#include <math.h>
#include <stdio.h>
#include "DevServoMotor_UCMC_UART.h"

/*
************************************************************************
*
*	Device Information
*
************************************************************************
*/
////////////////////////////////////////////////////////////////////////
//	Device Type
////////////////////////////////////////////////////////////////////////
#define	CAN_OPEN_DEV_TYPE				0x1000		//	Device type
	#define	CAN_OPEN_SERVO_CONTROLLER		0x00020192	//	CiA Draft Standard Proposal 402

////////////////////////////////////////////////////////////////////////
//	Device ID
////////////////////////////////////////////////////////////////////////
#define	CAN_OPEN_ID_OBJ					0x1018
	#define	CAN_OPEN_VENDOR_ID				0x01
	#define	CAN_OPEN_PRODUCT_CODE			0x02

/*
************************************************************************
*
*	Servo Controller
* 
************************************************************************
*/
////////////////////////////////////////////////////////////////////////
//	Mode, Command, Status and Error Code
////////////////////////////////////////////////////////////////////////
#define	CAN_OPEN_CTRL_MODE					0x6502	//	Control Mode
	#define	UCMC_CURRENT_CTRL_MODE			0x01
	#define	UCMC_VELOCITY_CTRL_MODE			0x02
	#define	UCMC_POSITION_CTRL_MODE			0x04

#define	CAN_OPEN_MODE					0x6060		//	Mode
	#define	CAN_OPEN_MODE_HOMING			0x06
	#define	CAN_OPEN_MODE_PRO_VEL			0x03
	#define	CAN_OPEN_MODE_PRO_POS			0x01
	#define	CAN_OPEN_MODE_POS				0xFF
	#define	CAN_OPEN_MODE_VEL				0xFE
	#define	CAN_OPEN_MODE_CUR				0xFC

#define	CAN_OPEN_COMMAND				0x6040		//	Command
	#define	UCMC_INIT						0x0001
	#define	UCMC_RESET						0x0002
	#define	UCMC_CLEAR_ERROR				0x0003
	#define	UCMC_EN_SERVO					0x0004
	#define	UCMC_DI_SERVO					0x0005
	#define	UCMC_QUICK_STOP					0x0006
	#define	UCMC_START_HOMING				0x0007
	#define	UCMC_SAVE_TO_EEPROM				0x00FE		//	Save parameters to EEPROM
	#define	UCMC_LOAD_FROM_EEPROM			0x00FF		//	Load parameters from EEPROM

#define	CAN_OPEN_STATUS					0x6041		//	Status
	#define	UCMC_STATUS_CUR					0x0001		//	The current mode
	#define	UCMC_STATUS_VEL					0x0002		//	The velocity mode
	#define	UCMC_STATUS_PRO_VEL				0x0004		//	The profile velocity mode
	#define	UCMC_STATUS_POS					0x0008		//	The position mode
	#define	UCMC_STATUS_PRO_POS				0x0010		//	The profile position mode
	#define	UCMC_STATUS_HOMING				0x0020		//	The homing mode
	#define	UCMC_SERVO_ON_OFF				0x0080		//	The servo on/off
	#define	UCMC_COMP_HOME					0x0100		//	Complete homing
	#define	UCMC_OCCUR_ERR					0x0200		//	Occur a error
	#define	UCMC_OCCUR_WARN					0x0400		//	Occur a warning
	#define	UCMC_RUNNING					0x8000		//	Is Running

#define	CAN_OPEN_ERROR					0x1001
	#define	CAN_OPEN_GENERIC_ERROR			0x01
	#define	CAN_OPEN_CURRENT_ERROR			0x02
	#define	CAN_OPEN_VOLTAGE_ERROR			0x04
	#define	CAN_OPEN_TEMP_ERROR				0x08
	#define	CAN_OPEN_COMM_ERROR				0x10
	#define	CAN_OPEN_DEV_PROFILE_ERROR		0x20
	#define	CAN_OPEN_MOTION_ERROR			0x80

#define	CAN_OPEN_DEV_ERROR				0x603F
	#define	UCMC_ERROR_GEN_ERR				0x0001		//	Generic Error -> Unspecific Error
	#define	UCMC_ERROR_SW_RESET				0x0002		//	Software Reset (WatchDog)
	#define	UCMC_ERROR_MOTOR_ERR			0x1001		//	Motor Error(No Motor Current)
	#define	UCMC_ERROR_ENC_ERR				0x1002		//	Encoder Error
	#define	UCMC_ERROR_HALL_ERR				0x1003		//	Hall Sensor Error
	#define	UCMC_ERROR_OVER_LOAD			0x1004		//	Overload Error
	#define	UCMC_ERROR_OVER_CUR				0x1005		//	Over Current Error
	#define	UCMC_ERROR_PICK_CUR_ERR			0x1006		//	Pick Current Error
	#define	UCMC_ERROR_NSW_LIMIT_ERR		0x2001		//	Negative SW Limit Error
	#define	UCMC_ERROR_PSW_LIMIT_ERR		0x2002		//	Positive SW Limit Error
	#define	UCMC_ERROR_VEL_ERR				0x2003		//	Velocity Error (abs(profile - actual) > xx pulse/ms)
	#define	UCMC_ERROR_POS_ERR				0x2004		//	Position Error (abs(profile - actual) > xx pulse)


///////////////////////////////////////////////////////////////////////////
//	Parameters for current control
///////////////////////////////////////////////////////////////////////////
#define	CAN_OPEN_MOTOR_DAT				0x6410		//	The motor data							(unsigned 8bit)
	#define	CAN_OPEN_CONT_CUR_LIMIT			0x0001		//	The continuous current limit		(signed 32bit)
	#define	CAN_OPEN_OUTPUT_CUR_LIMIT		0x0002		//	The output current limit			(signed 32bit)
#define	CAN_OPEN_CUR_PARAM				0x60F6		//	The PID parameters for current control	(unsigned 8bit)
	#define	CAN_OPEN_CUR_P_GAIN				0x0001		//	The P gain for current control		(signed 16bit)
	#define	CAN_OPEN_CUR_I_GAIN				0x0002		//	The I gain for current control		(signed 16bit)
#define	UCMC_TARGET_CUR					0x2100		//	The current mode setting value			(signed 32bit)
#define	CAN_OPEN_ACTUAL_CUR				0x6078		//	The actual current (RO)					(signed 32bit)
#define	UCMC_AVERAGED_CUR				0x2101		//	The average current	(RO)				(signed 32bit)


///////////////////////////////////////////////////////////////////////////
//	Parameters for velocity control
///////////////////////////////////////////////////////////////////////////
#define	UCMC_ENC_PULSE_NUM				0x2200		//	The encoder pulse number				(signed 16bit)
#define	CAN_OPEN_VEL_PARAM				0x60F9		//	The PID parameters for velocity control	(unsigned 8bit)
	#define	CAN_OPEN_VEL_P_GAIN				0x0001		//	The velocity P gain					(signed 16bit)
	#define	CAN_OPEN_VEL_I_GAIN				0x0002		//	The velocity I gain					(signed 16bit)
	#define	CAN_OPEN_VEL_D_GAIN				0x0003		//	The velocity D gain					(signed 16bit)

#define	CAN_OPEN_MAX_VEL				0x607F		//	The maximum velocity					(signed 16bit)
#define	CAN_OPEN_PROFILE_VEL			0x6081		//	The profile velocity					(signed 16bit)
#define	CAN_OPEN_ACCEL					0x6083		//	The acceleration						(signed 32bit)
#define	CAN_OPEN_DECEL					0x6084		//	The deceleration						(signed 32bit)
#define	CAN_OPEN_QUICK_STOP_DECEL		0x6085		//	The quick stop deceleration				(signed 32bit)

#define	CAN_OPEN_PROFILE_TARGET_VEL		0x60FF		//	The target velocity						(signed 16bit)
#define	UCMC_TARGET_VEL					0x2201		//	The reference velocity					(signed 16bit)
#define	CAN_OPEN_ACTUAL_VEL				0x606C		//	The actual velocity						(signed 16bit)

///////////////////////////////////////////////////////////////////////////
//	Parameters for velocity control
///////////////////////////////////////////////////////////////////////////
#define CAN_OPEN_POS_LIMIT				0x607D		//	The position limit						(unsigned 8bit)
	#define	CAN_OPEN_NEGATIVE_POS_LIMIT		0x0001		//	The minimal position limit	(RW)	(signed 32bit)
	#define	CAN_OPEN_POSITIVE_POS_LIMIT		0x0002		//	The maximum position limit	(RW)	(signed 32bit)
#define	CAN_OPEN_POS_PARAM				0x60FB		//	The PID parameters for position control	(unsigned 8bit)
	#define	CAN_OPEN_POS_P_GAIN				0x0001		//	The position P gain					(signed 16bit)
	#define	CAN_OPEN_POS_I_GAIN				0x0002		//	The position I gain					(signed 16bit)
	#define	CAN_OPEN_POS_D_GAIN				0x0003		//	The position D gain					(signed 16bit)
	#define	CAN_OPEN_POS_V_FACTOR			0x0004		//	The position Velocity Feed Forward Factor		(signed 16bit)
	#define	CAN_OPEN_POS_A_FACTOR			0x0005		//	The position Acceleration Feed Forward Factor	(signed 16bit)

#define	CAN_OPEN_PROFILE_TARGET_POS		0x607A		//	The target position						(signed 32bit)
#define	UCMC_TARGET_POS					0x2300		//	The reference position					(signed 32bit)
#define	CAN_OPEN_ACTUAL_POS				0x6064		//	The actual position						(signed 32bit)
#define	UCMC_ABSOLUTE_POS				0x2301		//	The absolute position					(signed 32bit)

///////////////////////////////////////////////////////////////////////////
//	Parameters for velocity control
///////////////////////////////////////////////////////////////////////////
#define	CAN_OPEN_HOMING_METHOD			0x6098		//	The homing method
#define	CAN_OPEN_HOMING_VEL				0x6099		//	The homing velocity
	#define	CAN_OPEN_SWITCH_SEARCH_VEL		0x0001		//	The velocity for switch search		(signed 16bit)
	#define	CAN_OPEN_ZERO_SEARCH_VEL		0x0002		//	The velocity for zero search		(signed 16bit)

#define	RS232_WRITE_MAX			0x22
#define	RS232_WRITE_DWORD		0x23
#define	RS232_WRITE_WORD		0x2B
#define	RS232_WRITE_BYTE		0x2F
#define	RS232_WRITE_RESPONSE	0x60
#define	RS232_WRITE_FLOAT		0x33

#define	RS232_READ_REQUEST		0x40
#define	RS232_READ_MAX			0x42
#define	RS232_READ_DWORD		0x43
#define	RS232_READ_WORD			0x4B
#define	RS232_READ_BYTE			0x4F
#define	RS232_READ_FLOAT		0x53

#define	RS232_ABORT_MSG			0xC0
	#define	RS232_NO_OBJ_ERR		0x06020000
	#define	RS232_SUB_INDEX_ERR		0x06090011
	#define	RS232_ACCESS_ERR		0x06010000
	#define	RS232_WR_ONLY_ERR		0x06010001
	#define	RS232_RD_ONLY_ERR		0x06010002
	#define	RS232_SERVICE_PARAM_ERR	0x06070010
	#define	RS232_VALUE_HIGH_ERR	0x06090031
	#define	RS232_VALUE_LOW_ERR		0x06090032
	#define	RS232_LESS_ERR			0x06090036
	#define	RS232_MORE_ERR			0x06090037
	#define	RS232_GEN_ERR			0x08000000
	#define	RS232_ID_ERR			0x0F00FFB9
	#define	RS232_SERVICE_MODE_ERR	0x0f00FFBC

#define	UCMC_DELAY			(50)

DevServoMotor_UCMC_UART::DevServoMotor_UCMC_UART(char *port)
{
	_id = 0;
	m_uart = new DevBusUART_RS232(port);
	if(m_uart->Open() < 0) 
	{
		AfxMessageBox("Can't open COM Port");
	}
}

DevServoMotor_UCMC_UART::~DevServoMotor_UCMC_UART(void)
{
	if(m_uart != NULL) {
		delete m_uart;
		m_uart = NULL;
	}
}

int DevServoMotor_UCMC_UART::SendPacket(unsigned char data[], int len)
{
	int ret;
	unsigned long timeTick;

	m_uart->Flush();

	timeTick = GetTickCount();

	for(int i = 0; i < len; i += ret) {
		ret = m_uart->Write(NULL, &data[i], (len - i));
		if(ret < 0) {
			return -1;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			return -1;
		}
	}

	return len;
}

int DevServoMotor_UCMC_UART::RecvPacket(unsigned char data[], int len)
{
	int ret;
	int i;
	unsigned long timeTick;

	timeTick = GetTickCount();

	for(i = 0; i < 1; i += ret) {
		ret = m_uart->Read(NULL, data, 1);
		if(ret < 0) {
			m_uart->Flush();	return -1;
		}
		else if(ret == 1) {
			if(data[0] == 0xFF) {
				break;
			}
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			m_uart->Flush();	return -1;
		}
	}

	for(i = 0; i < 1; i += ret) {
		ret = m_uart->Read(NULL, &data[1], 1);
		if(ret < 0) {
			m_uart->Flush();	return -1;
		}
		else if(ret == 1) {
			if(data[1] == 0xFF) {
				break;
			}
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			m_uart->Flush();	return -1;
		}
	}

	for(i = 2; i < 14; i += ret) {
		ret = m_uart->Read(NULL, &data[i], 16 - i);
		if(ret < 0) {
			m_uart->Flush();	return -1;
		}
		
		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			m_uart->Flush();	return -1;
		}
	}
	m_uart->Flush();

	return 0;
}

int DevServoMotor_UCMC_UART::ReadByteObj(long id, unsigned short index, unsigned char subIndex, unsigned char *data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_READ_REQUEST, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	*data = buf[10];

	return 0;
}

int DevServoMotor_UCMC_UART::ReadWordObj(long id, unsigned short index, unsigned char subIndex, unsigned short *data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_READ_REQUEST, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	memcpy(data, &buf[10], 2);

	return 0;
}

int DevServoMotor_UCMC_UART::ReadDwordObj(long id, unsigned short index, unsigned char subIndex, unsigned long *data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_READ_REQUEST, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	memcpy(data, &buf[10], 4);

	return 0;
}

int DevServoMotor_UCMC_UART::ReadMaxObj(long id, unsigned short index, unsigned char subIndex, unsigned long *data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_READ_REQUEST, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	memcpy(data, &buf[10], 4);

	return 0;
}

int DevServoMotor_UCMC_UART::WriteByteObj(long id, unsigned short index, unsigned char subIndex, const unsigned char data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_WRITE_BYTE, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	memcpy(&buf[10], &data, 1);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	return 0;
}

int DevServoMotor_UCMC_UART::WriteWordObj(long id, unsigned short index, unsigned char subIndex, const unsigned short data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_WRITE_WORD, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	memcpy(&buf[10], &data, 2);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	return 0;
}

int DevServoMotor_UCMC_UART::WriteDwordObj(long id, unsigned short index, unsigned char subIndex, const unsigned long data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_WRITE_DWORD, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	memcpy(&buf[10], &data, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	return 0;
}

int DevServoMotor_UCMC_UART::WriteMaxObj(long id, unsigned short index, unsigned char subIndex, unsigned long data)
{
	unsigned char buf[16] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, RS232_WRITE_MAX, (unsigned char)(index & 0xFF), (unsigned char)(index >> 8), subIndex, 0x00, 0x00, 0x00, 0x00};
	memcpy(&buf[2], &id, 4);
	memcpy(&buf[10], &data, 4);
	
	if(SendPacket(buf, 14) < 0) {
		return -1;
	}

	if(RecvPacket(buf, sizeof(buf)) < 0) {
		return -1;
	}

	return 0;
}

bool DevServoMotor_UCMC_UART::UCMC_GetDeviceType(DWORD *deviceType)
{
	if(ReadDwordObj(_id, CAN_OPEN_DEV_TYPE, 0, deviceType) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetDeviceVersion(double *softwareVersion, double *hardwareVersion)
{
	DWORD version;

	if(ReadDwordObj(_id, CAN_OPEN_ID_OBJ, CAN_OPEN_PRODUCT_CODE, &version) < 0) {
		return false;
	}

	*softwareVersion = (double)((0x0000FF00 & version) >> 8) + (double)(0x000000FF & version) * 0.1;
	*hardwareVersion = (double)((0xFF000000 & version) >> 24) + (double)((0x00FF0000 & version) >> 16) * 0.1;
	
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_Initialize(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_ResetParameters(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetControlMode(DWORD controlMode)
{
	if(WriteDwordObj(_id, CAN_OPEN_CTRL_MODE, 0, controlMode) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetControlMode(DWORD *controlMode)
{
	if(ReadDwordObj(_id, CAN_OPEN_CTRL_MODE, 0, controlMode) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetMode(BYTE mode)
{
	if(WriteByteObj(_id, CAN_OPEN_MODE, 0, mode) < 0) {
		return false;
	}

	_mode = mode;

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetMode(BYTE *mode)
{
	if(ReadByteObj(_id, CAN_OPEN_MODE, 0, mode) < 0) {
		return false;
	}

	_mode = *mode;

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_EnableMotorPower(void)
{
	if(WriteByteObj(_id, CAN_OPEN_COMMAND, 0, UCMC_EN_SERVO) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_DisableMotorPower(void)
{
	if(WriteByteObj(_id, CAN_OPEN_COMMAND, 0, UCMC_DI_SERVO) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_QuickStop(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_StartHoming(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_ClearError(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetNodeID(long id)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SaveToEEPROM(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_LoadFromEEPROM(void)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetStatus(WORD *status)
{
	if(ReadWordObj(_id, CAN_OPEN_STATUS, 0, status) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetError(BYTE *err)
{
	if(ReadByteObj(_id, CAN_OPEN_ERROR, 0, err) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetDeviceError(WORD *devErr)
{
	if(ReadWordObj(_id, CAN_OPEN_DEV_ERROR, 0, devErr) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetContinuousCurrent(long continuousCurrent)
{
	if(WriteDwordObj(_id, CAN_OPEN_MOTOR_DAT, CAN_OPEN_CONT_CUR_LIMIT, continuousCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetContinuousCurrent(long *continuousCurrent)
{
	if(ReadDwordObj(_id, CAN_OPEN_MOTOR_DAT, CAN_OPEN_CONT_CUR_LIMIT, (unsigned long *)continuousCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetOutputCurrent(long outputCurrent)
{
	if(WriteDwordObj(_id, CAN_OPEN_MOTOR_DAT, CAN_OPEN_OUTPUT_CUR_LIMIT, outputCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetOutputCurrent(long *outputCurrent)
{
	if(ReadDwordObj(_id, CAN_OPEN_MOTOR_DAT, CAN_OPEN_OUTPUT_CUR_LIMIT, (unsigned long *)outputCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetCurrentPGain(short pGain)
{
	if(WriteWordObj(_id, CAN_OPEN_CUR_PARAM, CAN_OPEN_CUR_P_GAIN, pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetCurrentPGain(short *pGain)
{
	if(ReadWordObj(_id, CAN_OPEN_CUR_PARAM, CAN_OPEN_CUR_P_GAIN, (unsigned short *)pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetCurrentIGain(short iGain)
{
	if(WriteWordObj(_id, CAN_OPEN_CUR_PARAM, CAN_OPEN_CUR_I_GAIN, iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetCurrentIGain(short *iGain)
{
	if(ReadWordObj(_id, CAN_OPEN_CUR_PARAM, CAN_OPEN_CUR_I_GAIN, (unsigned short *)iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetTargetCurrent(long targetCurrent)
{
	if(_mode != CAN_OPEN_MODE_CUR) {
		if(UCMC_SetMode(CAN_OPEN_MODE_CUR) == false) {
			return false;
		}
		
		if(UCMC_EnableMotorPower() == false) {
			return false;
		}
	}

	if(WriteDwordObj(_id, UCMC_TARGET_CUR, 0, targetCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetTargetCurrent(long *targetCurrent)
{
	if(ReadDwordObj(_id, UCMC_TARGET_CUR, 0, (unsigned long *)targetCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetActualCurrent(long *actualCurrent)
{
	if(ReadDwordObj(_id, CAN_OPEN_ACTUAL_CUR, 0, (unsigned long *)actualCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetAveragedCurrent(long *averagedCurrent)
{
	if(ReadDwordObj(_id, UCMC_AVERAGED_CUR, 0, (unsigned long *)averagedCurrent) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetEncoderCount(short encoderCount)
{
	if(WriteWordObj(_id, UCMC_ENC_PULSE_NUM, 0, encoderCount) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetEncoderCount(short *encoderCount)
{
	if(ReadWordObj(_id, UCMC_ENC_PULSE_NUM, 0, (unsigned short *)encoderCount) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetMaxVelocity(short maxVelocity)
{
	if(WriteWordObj(_id, CAN_OPEN_MAX_VEL, 0, maxVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetMaxVelocity(short *maxVelocity)
{
	if(ReadWordObj(_id, CAN_OPEN_MAX_VEL, 0, (unsigned short *)maxVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetProfileVelocity(short profileVelocity)
{
	if(WriteWordObj(_id, CAN_OPEN_PROFILE_VEL, 0, profileVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetProfileVelocity(short *profileVelocity)
{
	if(ReadWordObj(_id, CAN_OPEN_PROFILE_VEL, 0, (unsigned short *)profileVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetAcceleration(long acceleration)
{
	if(WriteDwordObj(_id, CAN_OPEN_ACCEL, 0, acceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetAcceleration(long *acceleration)
{
	if(ReadDwordObj(_id, CAN_OPEN_ACCEL, 0, (unsigned long *)acceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetDeceleration(long deceleration)
{
	if(WriteDwordObj(_id, CAN_OPEN_DECEL, 0, deceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetDeceleration(long *deceleration)
{
	if(ReadDwordObj(_id, CAN_OPEN_DECEL, 0, (unsigned long *)deceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetQuickStopDeceleration(long quickStopDeceleration)
{
	if(WriteDwordObj(_id, CAN_OPEN_QUICK_STOP_DECEL, 0, quickStopDeceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetQuickStopDeceleration(long *quickStopDeceleration)
{
	if(ReadDwordObj(_id, CAN_OPEN_QUICK_STOP_DECEL, 0, (unsigned long *)quickStopDeceleration) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetVelPGain(short pGain)
{
	if(WriteWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_P_GAIN, pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetVelPGain(short *pGain)
{
	if(ReadWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_P_GAIN, (unsigned short *)pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetVelIGain(short iGain)
{
	if(WriteWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_I_GAIN, iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetVelIGain(short *iGain)
{
	if(ReadWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_I_GAIN, (unsigned short *)iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetVelDGain(short dGain)
{
	if(WriteWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_D_GAIN, dGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetVelDGain(short *dGain)
{
	if(ReadWordObj(_id, CAN_OPEN_VEL_PARAM, CAN_OPEN_VEL_D_GAIN, (unsigned short *)dGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetProfileTargetVelocity(short profileTargetVelocity)
{
	if(_mode != CAN_OPEN_MODE_PRO_VEL) {
		if(UCMC_SetMode(CAN_OPEN_MODE_PRO_VEL) == false) {
			return false;
		}

		if(UCMC_EnableMotorPower() == false) {
			return false;
		}
	}

	if(WriteWordObj(_id, CAN_OPEN_PROFILE_TARGET_VEL, 0, profileTargetVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetProfileTargetVelocity(short *profileTargetVelocity)
{
	if(ReadWordObj(_id, CAN_OPEN_PROFILE_TARGET_VEL, 0, (unsigned short *)profileTargetVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetTargetVelocity(short targetVelocity)
{
	if(_mode != CAN_OPEN_MODE_VEL) {
		if(UCMC_SetMode(CAN_OPEN_MODE_VEL) == false) {
			return false;
		}

		if(UCMC_EnableMotorPower() == false) {
			return false;
		}
	}

	if(WriteWordObj(_id, UCMC_TARGET_VEL, 0, targetVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetTargetVelocity(short *targetVelocity)
{
	if(ReadWordObj(_id, UCMC_TARGET_VEL, 0, (unsigned short *)targetVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetActualVelocity(short *actualVelocity)
{
	if(ReadWordObj(_id, CAN_OPEN_ACTUAL_VEL, 0, (unsigned short *)actualVelocity) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetNegativePositionLimit(long negativePositionLimit)
{
	if(WriteDwordObj(_id, CAN_OPEN_POS_LIMIT, CAN_OPEN_NEGATIVE_POS_LIMIT, negativePositionLimit) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetNegativePositionLimit(long *negativePositionLimit)
{
	if(ReadDwordObj(_id, CAN_OPEN_POS_LIMIT, CAN_OPEN_NEGATIVE_POS_LIMIT, (unsigned long *)negativePositionLimit) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPositivePositionLimit(long positivePositionLimit)
{
	if(WriteDwordObj(_id, CAN_OPEN_POS_LIMIT, CAN_OPEN_POSITIVE_POS_LIMIT, positivePositionLimit) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPositivePositionLimit(long *positivePositionLimit)
{
	if(ReadDwordObj(_id, CAN_OPEN_POS_LIMIT, CAN_OPEN_POSITIVE_POS_LIMIT, (unsigned long *)positivePositionLimit) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPosPGain(short pGain)
{
	if(WriteWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_P_GAIN, pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPosPGain(short *pGain)
{
	if(ReadWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_P_GAIN, (unsigned short *)pGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPosIGain(short iGain)
{
	if(WriteWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_I_GAIN, iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPosIGain(short *iGain)
{
	if(ReadWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_I_GAIN, (unsigned short *)iGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPosDGain(short dGain)
{
	if(WriteWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_D_GAIN, dGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPosDGain(short *dGain)
{
	if(ReadWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_D_GAIN, (unsigned short *)dGain) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPosVelocityFactor(short velocityFactor)
{
	if(WriteWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_V_FACTOR, velocityFactor) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPosVelocityFactor(short *velocityFactor)
{
	if(ReadWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_V_FACTOR, (unsigned short *)velocityFactor) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetPosAccelerationFactor(short accelerationFactor)
{
	if(WriteWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_A_FACTOR, accelerationFactor) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetPosAccelerationFactor(short *accelerationFactor)
{
	if(ReadWordObj(_id, CAN_OPEN_POS_PARAM, CAN_OPEN_POS_A_FACTOR, (unsigned short *)accelerationFactor) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetProfilePosition(long profilePosition)
{
	if(_mode != CAN_OPEN_MODE_PRO_POS) {
		if(UCMC_SetMode(CAN_OPEN_MODE_PRO_POS) == false) {
			return false;
		}

		if(UCMC_EnableMotorPower() == false) {
			return false;
		}
	}
	
	if(WriteDwordObj(_id, CAN_OPEN_PROFILE_TARGET_POS, 0, profilePosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetProfilePosition(long *profilePosition)
{
	if(ReadDwordObj(_id, CAN_OPEN_PROFILE_TARGET_POS, 0, (unsigned long *)profilePosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetTargetPosition(long targetPosition)
{
	if(_mode != CAN_OPEN_MODE_POS) {
		if(UCMC_SetMode(CAN_OPEN_MODE_POS) == false) {
			return false;
		}

		if(UCMC_EnableMotorPower() == false) {
			return false;
		}
	}

	if(WriteDwordObj(_id, UCMC_TARGET_POS, 0, targetPosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetTargetPosition(long *targetPosition)
{
	if(ReadDwordObj(_id, UCMC_TARGET_POS, 0, (unsigned long *)targetPosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetActualPosition(long actualPosition)
{
	if(WriteDwordObj(_id, CAN_OPEN_ACTUAL_POS, 0, actualPosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetActualPosition(long *actualPosition)
{
	if(ReadDwordObj(_id, CAN_OPEN_ACTUAL_POS, 0, (unsigned long *)actualPosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetAbsolutePosition(long *absolutePosition)
{
	if(ReadDwordObj(_id, UCMC_ABSOLUTE_POS, 0, (unsigned long *)absolutePosition) < 0) {
		return false;
	}

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetHomingMethod(BYTE homingMethod)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetHomingMethod(BYTE *homingMethod)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetSwitchSearchVelocity(short switchHomingVel)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetSwitchSearchVelocity(short *switchHomingVel)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetZeroSearchVelocity(short zeroHomingVel)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetZeroSearchVelocity(short *zeroHomingVel)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_GetDigitalInput(DWORD *digitalInput)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SyncStatus(WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SyncCurrent(long targetCurrent, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SyncVelocity(short targetVelocity, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SyncPosition(long targetPosition, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion)
{
	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_TestPosition(long targetPosition, long *vel, int len)
{
	unsigned char data[6000] = {0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
	int ret;
	unsigned long timeTick;

	memcpy(&data[2], &_id, 4);
	memcpy(&data[10], &targetPosition, 4);

	m_uart->Flush();

	timeTick = GetTickCount();

	for(int i = 0; i < 14; i += ret) {
		ret = m_uart->Write(NULL, &data[i], (14 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	if(len > 1500) {
		len = 1500;
	}

	for(int i = 0; i < len * 4; i += ret) {
		ret = m_uart->Read(NULL, &data[i], (len * 4 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	memcpy(vel, data, len * 4);

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_TestVelocityPosition(long targetPosition, long *vel, int len)
{
	unsigned char data[6000] = {0xFF, 0xFF, 0, 0, 0, 0, 0xFD, 0xFF, 0xFF, 0xFF};
	int ret;
	unsigned long timeTick;

	memcpy(&data[2], &_id, 4);
	memcpy(&data[10], &targetPosition, 4);

	m_uart->Flush();

	timeTick = GetTickCount();

	for(int i = 0; i < 14; i += ret) {
		ret = m_uart->Write(NULL, &data[i], (14 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	if(len > 1500) {
		len = 1500;
	}

	for(int i = 0; i < len * 4; i += ret) {
		ret = m_uart->Read(NULL, &data[i], (len * 4 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	memcpy(vel, data, len * 4);

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_TestVelocity(long targetVelocity, long *vel, int len)
{
	unsigned char data[6000] = {0xFF, 0xFF, 0, 0, 0, 0, 0xFE, 0xFF, 0xFF, 0xFF};
	int ret;
	unsigned long timeTick;

	memcpy(&data[2], &_id, 4);
	memcpy(&data[10], &targetVelocity, 4);

	m_uart->Flush();

	timeTick = GetTickCount();

	for(int i = 0; i < 14; i += ret) {
		ret = m_uart->Write(NULL, &data[i], (14 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	if(len > 1500) {
		len = 1500;
	}

	for(int i = 0; i < len * 4; i += ret) {
		ret = m_uart->Read(NULL, &data[i], (len * 4 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	memcpy(vel, data, len * 4);

	return true;
}

bool DevServoMotor_UCMC_UART::UCMC_SetTargetPositionBuf(int index, long targetPosition)
{
	unsigned char data[16] = {0xFF, 0xFF, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF};
	int ret;
	unsigned long timeTick;

	if(index >= 1500) {
		return false;
	}

	memcpy(&data[2], &_id, 4);
	data[6] = (unsigned char)(index & 0xFF);
	data[7] = (unsigned char)((index >> 8) & 0xFF);
	memcpy(&data[10], &targetPosition, 4);

	m_uart->Flush();

	timeTick = GetTickCount();

	for(int i = 0; i < 14; i += ret) {
		ret = m_uart->Write(NULL, &data[i], (14 - i));
		if(ret < 0) {
			false;
		}

		if((GetTickCount() - timeTick) > UCMC_DELAY) {
			false;
		}
	}

	return true;
}
