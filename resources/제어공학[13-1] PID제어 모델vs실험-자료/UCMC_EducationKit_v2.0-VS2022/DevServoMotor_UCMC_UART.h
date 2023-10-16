#ifndef	_DEV_SERVO_MOTOR_UCMC_UART_H_
#define	_DEV_SERVO_MOTOR_UCMC_UART_H_

#include "DevBusUART_RS232.h"

class DevServoMotor_UCMC_UART
{
public:
	DevServoMotor_UCMC_UART(char *port);
	virtual ~DevServoMotor_UCMC_UART();

private:
	long _id;
	BYTE _mode;
	DevBusUART_RS232 *m_uart;
	
	void StateChangeUcmc (int ret, unsigned short status);

	int SendPacket(unsigned char data[], int len);
	int RecvPacket(unsigned char data[], int len);

	int ReadByteObj(long id, unsigned short index, unsigned char subIndex, unsigned char *data);
	int ReadWordObj(long id, unsigned short index, unsigned char subIndex, unsigned short *data);
	int ReadDwordObj(long id, unsigned short index, unsigned char subIndex, unsigned long *data);
	int ReadMaxObj(long id, unsigned short index, unsigned char subIndex, unsigned long *data);
	
	int WriteByteObj(long id, unsigned short index, unsigned char subIndex, const unsigned char data);
	int WriteWordObj(long id, unsigned short index, unsigned char subIndex, const unsigned short data);
	int WriteDwordObj(long id, unsigned short index, unsigned char subIndex, const unsigned long data);
	int WriteMaxObj(long id, unsigned short index, unsigned char subIndex, unsigned long data);

public:
	//	Device Information
	virtual bool UCMC_GetDeviceType(DWORD *deviceType);
	virtual bool UCMC_GetDeviceVersion(double *softwareVersion, double *hardwareVersion);

	//	Command
	virtual bool UCMC_Initialize(void);
	virtual bool UCMC_ResetParameters(void);
	virtual bool UCMC_SetControlMode(DWORD controlMode);
	virtual bool UCMC_GetControlMode(DWORD *controlMode);
	virtual bool UCMC_SetMode(BYTE mode);
	virtual bool UCMC_GetMode(BYTE *mode);
	virtual bool UCMC_EnableMotorPower(void);
	virtual bool UCMC_DisableMotorPower(void);
	virtual bool UCMC_QuickStop(void);
	virtual bool UCMC_StartHoming(void);
	virtual bool UCMC_ClearError(void);
	virtual bool UCMC_SetNodeID(long id);
	virtual bool UCMC_SaveToEEPROM(void);
	virtual bool UCMC_LoadFromEEPROM(void);

	//	Status
	virtual bool UCMC_GetStatus(WORD *status);
	virtual bool UCMC_GetError(BYTE *err);
	virtual bool UCMC_GetDeviceError(WORD *devErr);

	//	Current Control
	virtual bool UCMC_SetContinuousCurrent(long continuousCurrent);
	virtual bool UCMC_GetContinuousCurrent(long *continuousCurrent);
	virtual bool UCMC_SetOutputCurrent(long outputCurrent);
	virtual bool UCMC_GetOutputCurrent(long *outputCurrent);
	
	virtual bool UCMC_SetCurrentPGain(short pGain);
	virtual bool UCMC_GetCurrentPGain(short *pGain);
	virtual bool UCMC_SetCurrentIGain(short iGain);
	virtual bool UCMC_GetCurrentIGain(short *iGain);

	virtual bool UCMC_SetTargetCurrent(long targetCurrent);
	virtual bool UCMC_GetTargetCurrent(long *targetCurrent);
	virtual bool UCMC_GetActualCurrent(long *actualCurrent);
	virtual bool UCMC_GetAveragedCurrent(long *averagedCurrent);

	//	Velocity Control
	virtual bool UCMC_SetEncoderCount(short encoderCount);
	virtual bool UCMC_GetEncoderCount(short *encoderCount);

	virtual bool UCMC_SetMaxVelocity(short maxVelocity);
	virtual bool UCMC_GetMaxVelocity(short *maxVelocity);
	virtual bool UCMC_SetProfileVelocity(short profileVelocity);
	virtual bool UCMC_GetProfileVelocity(short *profileVelocity);
	virtual bool UCMC_SetAcceleration(long acceleration);
	virtual bool UCMC_GetAcceleration(long *acceleration);
	virtual bool UCMC_SetDeceleration(long deceleration);
	virtual bool UCMC_GetDeceleration(long *deceleration);
	virtual bool UCMC_SetQuickStopDeceleration(long quickStopDeceleration);
	virtual bool UCMC_GetQuickStopDeceleration(long *quickStopDeceleration);

	virtual bool UCMC_SetVelPGain(short pGain);
	virtual bool UCMC_GetVelPGain(short *pGain);
	virtual bool UCMC_SetVelIGain(short iGain);
	virtual bool UCMC_GetVelIGain(short *iGain);
	virtual bool UCMC_SetVelDGain(short dGain);
	virtual bool UCMC_GetVelDGain(short *dGain);

	virtual bool UCMC_SetProfileTargetVelocity(short profileTargetVelocity);
	virtual bool UCMC_GetProfileTargetVelocity(short *profileTargetVelocity);
	virtual bool UCMC_SetTargetVelocity(short targetVelocity);
	virtual bool UCMC_GetTargetVelocity(short *targetVelocity);
	virtual bool UCMC_GetActualVelocity(short *actualVelocity);


	//	Position Control
	virtual bool UCMC_SetNegativePositionLimit(long negativePositionLimit);
	virtual bool UCMC_GetNegativePositionLimit(long *negativePositionLimit);
	virtual bool UCMC_SetPositivePositionLimit(long positivePositionLimit);
	virtual bool UCMC_GetPositivePositionLimit(long *positivePositionLimit);

	virtual bool UCMC_SetPosPGain(short pGain);
	virtual bool UCMC_GetPosPGain(short *pGain);
	virtual bool UCMC_SetPosIGain(short iGain);
	virtual bool UCMC_GetPosIGain(short *iGain);
	virtual bool UCMC_SetPosDGain(short dGain);
	virtual bool UCMC_GetPosDGain(short *dGain);
	virtual bool UCMC_SetPosVelocityFactor(short velocityFactor);
	virtual bool UCMC_GetPosVelocityFactor(short *velocityFactor);
	virtual bool UCMC_SetPosAccelerationFactor(short accelerationFactor);
	virtual bool UCMC_GetPosAccelerationFactor(short *accelerationFactor);

	virtual bool UCMC_SetProfilePosition(long profilePosition);
	virtual bool UCMC_GetProfilePosition(long *profilePosition);
	virtual bool UCMC_SetTargetPosition(long targetPosition);
	virtual bool UCMC_GetTargetPosition(long *targetPosition);
	virtual bool UCMC_SetActualPosition(long actualPosition);
	virtual bool UCMC_GetActualPosition(long *actualPosition);
	virtual bool UCMC_GetAbsolutePosition(long *absolutePosition);

	//	Homing
	virtual bool UCMC_SetHomingMethod(BYTE homingMethod);
	virtual bool UCMC_GetHomingMethod(BYTE *homingMethod);
	virtual bool UCMC_SetSwitchSearchVelocity(short switchHomingVel);
	virtual bool UCMC_GetSwitchSearchVelocity(short *switchHomingVel);
	virtual bool UCMC_SetZeroSearchVelocity(short zeroHomingVel);
	virtual bool UCMC_GetZeroSearchVelocity(short *zeroHomingVel);

	//	IO
	virtual bool UCMC_GetDigitalInput(DWORD *digitalInput);

	//	Sync Command
	virtual bool UCMC_SyncStatus(WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion);
	virtual bool UCMC_SyncCurrent(long targetCurrent, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion);
	virtual bool UCMC_SyncVelocity(short targetVelocity, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion);
	virtual bool UCMC_SyncPosition(long targetPosition, WORD *status, long *actualCurrent, short *actualVelocity, long *actualPosotion);

	//	Gain Test
	bool UCMC_TestPosition(long targetPosition, long *vel, int len);
	bool UCMC_TestVelocityPosition(long targetPosition, long *vel, int len);
	bool UCMC_TestVelocity(long targetVelocity, long *vel, int len);
	bool UCMC_SetTargetPositionBuf(int index, long targetPosition);
};

#endif	//	_DEV_SERVO_MOTOR_UCMC_UART_H_
