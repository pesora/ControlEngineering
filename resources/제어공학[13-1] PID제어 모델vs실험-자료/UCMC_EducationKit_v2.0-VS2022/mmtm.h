//////////////////////////////////////////////////////
// Common files for Robot applications
// 
// Copyright (c) 2002-2008. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://dart.kitech.re.kr
// Written by KwangWoong Yang<page365@gmail.com>
//
// Multimedia timer를 사용하기 위한 함수들 모음

#pragma once 
#pragma comment (lib,"winmm.lib")

#include <windows.h>
#include <Mmsystem.h>

extern void mmtm_init ();
extern void mmtm_deinit ();

extern DWORD mmtm_start (UINT msInterval, LPTIMECALLBACK lpTimeCallback, DWORD dwUser);
extern void mmtm_stop (DWORD _timerId);
extern DWORD mmtm_get_time ();
extern void mmtm_sleep (DWORD ms);
