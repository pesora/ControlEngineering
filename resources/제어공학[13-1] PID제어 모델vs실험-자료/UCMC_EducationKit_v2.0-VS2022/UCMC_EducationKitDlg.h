// UCMC_EducationKitDlg.h : header file
//

#if !defined(AFX_UCMC_EDUCATIONKITDLG_H__3734D1BB_9322_43F6_B776_21C02EC322C9__INCLUDED_)
#define AFX_UCMC_EDUCATIONKITDLG_H__3734D1BB_9322_43F6_B776_21C02EC322C9__INCLUDED_

#include "DevServoMotor_UCMC_UART.h"	// Added by ClassView
#include "GRAPHDLG.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUCMC_EducationKitDlg dialog
#define SOFT_MODE 1 //1: soft landing 0: no soft landing
#define SWITCH_MODE 1 //1:Koh method 0:Zanashi method
#define ENDING_MODE 0 //1:fs modifying 0:amax modifying 2: No Ending
#define FOLLOWING_MODE 1 //0:No PD 1:PD
#define FILE_NAME "out2sssss5int_soft_zeroVEL_Koh_PD.xls"
#define TARGET_POS 10000
#define TARGET_VEL 0
#define INITIAL_VEL 0
#define AMAX 1024*5
#define AMAX2 1024*5
#define VMAX 1024*5
#define SAMPLING_T 0.01
#define SAMPLES_MAX 500


class CUCMC_EducationKitDlg : public CDialog
{
// Construction
public:
	int ref[1500];
	int display_mode;
	int control_mode;
	void Callback_stop();
	void Callback_start();
	void callbackData();
	void DRAWGRAPH();
	void GetValue();
	unsigned long exam;
	int connectflag;
	GRAPHDLG dmk;
	int test;
	typedef struct Moter 
	{
	int p;
	int v;
	int a;
	int s;
	int ppt;
	int ee;
	int eedot;

	};
	Moter data[10000];
	CRect m_rect;
	unsigned char *buffer;
	BITMAPINFO   *bminfo; 


	long vel[1500];
	long posvelue[1500];
	BYTE *read_data;

	int callbackflag;


	int amax,amax2,vmax;
	double ts;

	int fs;
	int m;
	CUCMC_EducationKitDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUCMC_EducationKitDlg)
	enum { IDD = IDD_UCMC_EDUCATIONKIT_DIALOG };
	CButton	m_send;
	CButton	m_show_graph;
	CButton	m_exe_buffer;
	CStatic	m_image;
	CComboBox	m_uartCombo;
	short m_cur_cont;
	short m_cur_i_gain;
	int m_cur_output;
	short m_cur_p_gain;
	short m_cur_target;
	long m_pos_actual;
	short m_pos_d_gain;
	short m_pos_i_gain;
	long m_pos_negative;
	short m_pos_p_gain;
	long m_pos_positive;
	long m_pos_target;
	long m_vel_accel;
	int m_vel_encpulse;
	long m_vel_i_gain;
	short m_vel_maxvel;
	long m_vel_p_gain;
	short m_vel_target;
	int		m_ramp;
	double	m_ff1;
	double	m_ff2;
	int		m_smapling_time;
	int		m_step;
	int		m_freq;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUCMC_EducationKitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUCMC_EducationKitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUartOpenBtn();
	afx_msg void OnDestroy();
	afx_msg void OnReset();
	afx_msg void OnInit();
	afx_msg void OnEnable();
	afx_msg void OnDisable();
	afx_msg void OnStop();
	afx_msg void OnHome();
	afx_msg void OnClear();
	afx_msg void OnStatus();
	afx_msg void OnError();
	afx_msg void OnDevError();
	afx_msg void OnFileopen();
	afx_msg void OnFilesave();
	afx_msg void OnButtonCurCont();
	afx_msg void OnButtonCurOutput();
	afx_msg void OnButtonCurTarget();
	afx_msg void OnButtonCurIGain();
	afx_msg void OnButtonPosActual();
	afx_msg void OnButtonPosDGain();
	afx_msg void OnButtonPosIGain();
	afx_msg void OnButtonPosNegative();
	afx_msg void OnButtonPosPGain();
	afx_msg void OnButtonPosPositive();
	afx_msg void OnButtonPosTarget();
	afx_msg void OnButtonVelAccel();
	afx_msg void OnButtonVelEncpulse();
	afx_msg void OnButtonVelIGain();
	afx_msg void OnButtonVelMaxvel();
	afx_msg void OnButtonVelPGain();
	afx_msg void OnButtonVelTarget();
	afx_msg void OnButtonCurPGain();
	afx_msg void OnSendArray();
	afx_msg void OnMake();
	afx_msg void OnShowGraph();
	afx_msg void OnSend_Buffer();
	afx_msg void OnMakeStep();
	afx_msg void OnRadioVel();
	afx_msg void OnRadioPosition();
	afx_msg void OnMakePara();
	afx_msg void OnChangeEditPosPGain();
	afx_msg void OnRadioFb();
	afx_msg void OnRadioFf1();
	afx_msg void OnRadioFf2();
	afx_msg void OnRadioDispPos();
	afx_msg void OnRadioDispVel();
	afx_msg void OnRadioDispErr();
	afx_msg void OnRadioGraphPos();
	afx_msg void OnRadioGraphVel();
	afx_msg void OnRadioGraphErr();
	afx_msg void OnMakeSine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DevServoMotor_UCMC_UART * m_servoMotor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UCMC_EDUCATIONKITDLG_H__3734D1BB_9322_43F6_B776_21C02EC322C9__INCLUDED_)
