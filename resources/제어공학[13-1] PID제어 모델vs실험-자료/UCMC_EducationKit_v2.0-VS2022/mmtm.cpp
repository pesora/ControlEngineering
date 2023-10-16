//////////////////////////////////////////////////////
// Common files for Robot applications
// 
// Copyright (c) 2002-2008. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://dart.kitech.re.kr
// Written by KwangWoong Yang<page365@gmail.com>
//
// Multimedia timer를 사용하기 위한 함수들 모음
#include "stdafx.h"

#include "mmtm.h"

#define TARGET_RESOLUTION	1         // 1-millisecond target resolution

static UINT _timerRes = 0;

void mmtm_init ()
{
	TIMECAPS tc;

	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	_timerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(_timerRes); 
}

void mmtm_deinit ()
{
	if (0 < _timerRes) {
		timeEndPeriod (_timerRes);
	}
}

static void CALLBACK OneShotTimer(UINT _timerId, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
} 

DWORD mmtm_start (UINT msInterval, LPTIMECALLBACK lpTimeCallback, DWORD dwUser)
{
	return timeSetEvent (msInterval, _timerRes, lpTimeCallback, (DWORD)dwUser, TIME_PERIODIC);
}

void mmtm_stop (DWORD _timerId)
{
	timeKillEvent (_timerId);  // cancel the event
}

DWORD mmtm_get_time ()
{
	return timeGetTime(); 
}

void mmtm_sleep (DWORD ms)
{
	Sleep (ms);
}
