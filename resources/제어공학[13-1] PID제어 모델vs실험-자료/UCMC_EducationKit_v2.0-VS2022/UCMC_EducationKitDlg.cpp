// UCMC_EducationKitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UCMC_EducationKit.h"
#include "UCMC_EducationKitDlg.h"
#include "mmtm.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#define PHI 3.141592
#define T 0.001
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool isVelocityMode = false;

extern CUCMC_EducationKitApp theApp;

unsigned long hMultiMediaTimer;
int kmk;
void CALLBACK MultiMediaTimerFunc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	((CUCMC_EducationKitDlg *)theApp.m_pMainWnd)->callbackData();

	
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUCMC_EducationKitDlg dialog

CUCMC_EducationKitDlg::CUCMC_EducationKitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUCMC_EducationKitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUCMC_EducationKitDlg)
	m_cur_cont = 0;
	m_cur_i_gain = 0;
	m_cur_output = 0;
	m_cur_p_gain = 0;
	m_cur_target = 0;
	m_pos_actual = 0;
	m_pos_d_gain = 0;
	m_pos_i_gain = 0;
	m_pos_negative = 0;
	m_pos_p_gain = 0;
	m_pos_positive = 0;
	m_pos_target = 0;
	m_vel_accel = 0;
	m_vel_encpulse = 0;
	m_vel_i_gain = 0;
	m_vel_maxvel = 0;
	m_vel_p_gain = 0;
	m_vel_target = 0;
	m_ramp = 0;
	m_ff1 = 0.0;
	m_ff2 = 0.0;
	m_smapling_time = 1500;
	m_step = 0;
	m_freq = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUCMC_EducationKitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUCMC_EducationKitDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_send);
	DDX_Control(pDX, IDC_SHOW_GRAPH, m_show_graph);
	DDX_Control(pDX, IDC_SEND_ARRAY, m_exe_buffer);
	DDX_Control(pDX, IDC_UART_COMBO, m_uartCombo);
	DDX_Text(pDX, IDC_EDIT_CUR_CONT, m_cur_cont);
	DDX_Text(pDX, IDC_EDIT_CUR_I_GAIN, m_cur_i_gain);
	DDX_Text(pDX, IDC_EDIT_CUR_OUTPUT, m_cur_output);
	DDX_Text(pDX, IDC_EDIT_CUR_P_GAIN, m_cur_p_gain);
	DDX_Text(pDX, IDC_EDIT_CUR_TARGET, m_cur_target);
	DDX_Text(pDX, IDC_EDIT_POS_ACTUAL, m_pos_actual);
	DDX_Text(pDX, IDC_EDIT_POS_D_GAIN, m_pos_d_gain);
	DDX_Text(pDX, IDC_EDIT_POS_I_GAIN, m_pos_i_gain);
	DDX_Text(pDX, IDC_EDIT_POS_NEGATIVE, m_pos_negative);
	DDX_Text(pDX, IDC_EDIT_POS_P_GAIN, m_pos_p_gain);
	DDX_Text(pDX, IDC_EDIT_POS_POSITIVE, m_pos_positive);
	DDX_Text(pDX, IDC_EDIT_POS_TRAGET, m_pos_target);
	DDX_Text(pDX, IDC_EDIT_VEL_ACCEL, m_vel_accel);
	DDX_Text(pDX, IDC_EDIT_VEL_ENCPULSE, m_vel_encpulse);
	DDX_Text(pDX, IDC_EDIT_VEL_I_GAIN, m_vel_i_gain);
	DDX_Text(pDX, IDC_EDIT_VEL_MAXVEL, m_vel_maxvel);
	DDX_Text(pDX, IDC_EDIT_VEL_P_GAIN, m_vel_p_gain);
	DDX_Text(pDX, IDC_EDIT_VEL_TARGET, m_vel_target);
	DDX_Text(pDX, IDC_EDIT_RAMP, m_ramp);
	DDX_Text(pDX, IDC_EDIT_FF1, m_ff1);
	DDX_Text(pDX, IDC_EDIT_FF2, m_ff2);
	DDX_Text(pDX, IDC_SAMPLINGNUM, m_smapling_time);
	DDX_Text(pDX, IDC_EDIT_STEP, m_step);
	DDX_Text(pDX, IDC_EDIT_SINE, m_freq);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUCMC_EducationKitDlg, CDialog)
	//{{AFX_MSG_MAP(CUCMC_EducationKitDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_UART_OPEN_BTN, OnUartOpenBtn)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_DISABLE, OnDisable)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_HOME, OnHome)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_STATUS, OnStatus)
	ON_BN_CLICKED(IDC_ERROR, OnError)
	ON_BN_CLICKED(IDC_DEV_ERROR, OnDevError)
	ON_BN_CLICKED(IDC_FILEOPEN, OnFileopen)
	ON_BN_CLICKED(IDC_FILESAVE, OnFilesave)
	ON_BN_CLICKED(IDC_BUTTON_CUR_CONT, OnButtonCurCont)
	ON_BN_CLICKED(IDC_BUTTON_CUR_OUTPUT, OnButtonCurOutput)
	ON_BN_CLICKED(IDC_BUTTON_CUR_TARGET, OnButtonCurTarget)
	ON_BN_CLICKED(IDC_BUTTON_CUR_I_GAIN, OnButtonCurIGain)
	ON_BN_CLICKED(IDC_BUTTON_POS_ACTUAL, OnButtonPosActual)
	ON_BN_CLICKED(IDC_BUTTON_POS_D_GAIN, OnButtonPosDGain)
	ON_BN_CLICKED(IDC_BUTTON_POS_I_GAIN, OnButtonPosIGain)
	ON_BN_CLICKED(IDC_BUTTON_POS_NEGATIVE, OnButtonPosNegative)
	ON_BN_CLICKED(IDC_BUTTON_POS_P_GAIN, OnButtonPosPGain)
	ON_BN_CLICKED(IDC_BUTTON_POS_POSITIVE, OnButtonPosPositive)
	ON_BN_CLICKED(IDC_BUTTON_POS_TARGET, OnButtonPosTarget)
	ON_BN_CLICKED(IDC_BUTTON_VEL_ACCEL, OnButtonVelAccel)
	ON_BN_CLICKED(IDC_BUTTON_VEL_ENCPULSE, OnButtonVelEncpulse)
	ON_BN_CLICKED(IDC_BUTTON_VEL_I_GAIN, OnButtonVelIGain)
	ON_BN_CLICKED(IDC_BUTTON_VEL_MAXVEL, OnButtonVelMaxvel)
	ON_BN_CLICKED(IDC_BUTTON_VEL_P_GAIN, OnButtonVelPGain)
	ON_BN_CLICKED(IDC_BUTTON_VEL_TARGET, OnButtonVelTarget)
	ON_BN_CLICKED(IDC_BUTTON_CUR_P_GAIN, OnButtonCurPGain)
	ON_BN_CLICKED(IDC_SEND_ARRAY, OnSendArray)
	ON_BN_CLICKED(IDC_MAKE, OnMake)
	ON_BN_CLICKED(IDC_SHOW_GRAPH, OnShowGraph)
	ON_BN_CLICKED(IDC_BUTTON1, OnSend_Buffer)
	ON_BN_CLICKED(IDC_MAKE_STEP, OnMakeStep)
	ON_BN_CLICKED(IDC_RADIO_VEL, OnRadioVel)
	ON_BN_CLICKED(IDC_RADIO_POSITION, OnRadioPosition)
	ON_BN_CLICKED(IDC_MAKE_PARA, OnMakePara)
	ON_BN_CLICKED(IDC_RADIO_FB, OnRadioFb)
	ON_BN_CLICKED(IDC_RADIO_FF1, OnRadioFf1)
	ON_BN_CLICKED(IDC_RADIO_FF2, OnRadioFf2)
	ON_BN_CLICKED(IDC_RADIO_GRAPH_POS, OnRadioGraphPos)
	ON_BN_CLICKED(IDC_RADIO_GRAPH_VEL, OnRadioGraphVel)
	ON_BN_CLICKED(IDC_RADIO_GRAPH_ERR, OnRadioGraphErr)
	ON_BN_CLICKED(IDC_MAKE_SINE, OnMakeSine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUCMC_EducationKitDlg message handlers

BOOL CUCMC_EducationKitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
//	ShowWindow(SW_MAXIMIZE);
	
	// TODO: Add extra initialization here
	CString strTmp;
	for(int i = 0; i <= 9; i++)
	{
		strTmp.Format("COM%d", i + 1);
		m_uartCombo.InsertString(i, strTmp);
	}
	m_uartCombo.SetCurSel(0);

	m_servoMotor = NULL;
	connectflag=0;


/////////////////////////////////////////
	amax=AMAX;
	amax2=AMAX2;
	vmax=VMAX;
	ts=SAMPLING_T;

	fs=(int)(1/ts);
	m=0;
/////////////////////////////////////////////

	test=0;
	mmtm_init();
	callbackflag =0;
	int i;

	for (i=0; i<1500; i++)
	{
		posvelue[i]=512;
	}

	for (i=0; i<1500; i++)
	{
		vel[i]=-1000000;
	}

// ////  Edit 초기화 부분
	m_ramp=4;
	m_step=512;
	m_freq=1;
	m_ff1 = 100.0;
	m_ff2 = 100.0;
	m_smapling_time=150;
//////



	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUCMC_EducationKitDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	Callback_stop();
	// TODO: Add your message handler code here
	if (callbackflag==1)
	{
		callbackflag=0;
		mmtm_stop(exam);
	}

	if(m_servoMotor != NULL) 
	{
		delete m_servoMotor;
		m_servoMotor = NULL;
	}
	dmk.DestroyWindow();

}

void CUCMC_EducationKitDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUCMC_EducationKitDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon

		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{

		CDialog::OnPaint();
	}


}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUCMC_EducationKitDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUCMC_EducationKitDlg::OnUartOpenBtn() 
{
	// TODO: Add your control notification handler code here
	if(m_servoMotor != NULL) 
	{
	//	AfxMessageBox("시리얼 포트가 이미 열려있습니다.");
	//	return;
	}

	CString port;
	int index = m_uartCombo.GetCurSel();
	m_uartCombo.GetLBText(index, port);

	m_servoMotor = new DevServoMotor_UCMC_UART((LPSTR)(LPCTSTR)port);

	double swVer, hwVer;
	m_servoMotor->UCMC_GetDeviceVersion(&swVer, &hwVer);

	port.Format("VERSION - SW : %f, HW : %f", swVer, hwVer);
	SetDlgItemText(IDC_STATIC_VERSION, port);
	m_servoMotor->UCMC_SetControlMode(0x02);
	connectflag=1;
	OnStatus() ;


//// radio 버튼 초기화 부분
	CButton *pCheck;
	pCheck=(CButton *)GetDlgItem(IDC_RADIO_FB);
	pCheck->SetCheck(TRUE);
	OnRadioFb();

	pCheck=(CButton *)GetDlgItem(IDC_RADIO_VEL);
	pCheck->SetCheck(TRUE);
	OnRadioVel();

	pCheck=(CButton *)GetDlgItem(IDC_RADIO_GRAPH_POS);
	pCheck->SetCheck(TRUE);
	OnRadioGraphPos();

	UpdateData(FALSE);
/////'

	Callback_start();
}


void CUCMC_EducationKitDlg::OnReset() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_ResetParameters();	
	
}

void CUCMC_EducationKitDlg::OnInit() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_Initialize();
}

void CUCMC_EducationKitDlg::OnEnable() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_EnableMotorPower();
}

void CUCMC_EducationKitDlg::OnDisable() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_DisableMotorPower();
}

void CUCMC_EducationKitDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_QuickStop();	
}

void CUCMC_EducationKitDlg::OnHome() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_StartHoming();	
}

void CUCMC_EducationKitDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_ClearError();	
}

void CUCMC_EducationKitDlg::OnStatus() 
{
	// TODO: Add your control notification handler code here
	unsigned short status;
	CString temp; 
	DWORD mk;

	m_servoMotor->UCMC_GetStatus(&status);
	m_servoMotor->UCMC_GetControlMode(&mk);

	temp.Format("STATUS : %d ", status);
	SetDlgItemText(IDC_STATIC_STATUS, temp);
	GetValue();
}

void CUCMC_EducationKitDlg::OnError() 
{
	// TODO: Add your control notification handler code here
	unsigned char err;
	CString temp; 

	m_servoMotor->UCMC_GetError(&err);
	temp.Format("ERROR  : %d ", err);
	SetDlgItemText(IDC_STATIC_ERROR, temp);
	
}

void CUCMC_EducationKitDlg::OnDevError() 
{
	// TODO: Add your control notification handler code here
	unsigned short d_err;
	CString temp; 

	m_servoMotor->UCMC_GetDeviceError(&d_err);
	temp.Format("DEVICE ERROR  : %d ", d_err);
	SetDlgItemText(IDC_STATIC_DEVICE_ERROR, temp);	
}

void CUCMC_EducationKitDlg::GetValue()
{
	short temp_short;
	long temp_long;

	CString display;



  ///////////////////CURRENT////////////////////
  m_servoMotor->UCMC_GetContinuousCurrent(&temp_long);


  m_cur_cont = (short)temp_long;



  m_servoMotor->UCMC_GetOutputCurrent(&temp_long);

  m_cur_output = (short)temp_long;

  m_servoMotor->UCMC_GetCurrentPGain(&temp_short);
	
  m_cur_p_gain = temp_short;

  m_servoMotor->UCMC_GetCurrentIGain(&temp_short);

  m_cur_i_gain = temp_short;

  m_servoMotor->UCMC_GetTargetCurrent(&temp_long);

  m_cur_target = (short)temp_long;
  
m_servoMotor->UCMC_GetActualCurrent(&temp_long);



 ////////////////////VELOCITY////////////////////////



  m_servoMotor->UCMC_GetEncoderCount(&temp_short);
 
	m_vel_encpulse = temp_short;


  m_servoMotor->UCMC_GetAcceleration(&temp_long);
  
	m_vel_accel = temp_long;

  m_servoMotor->UCMC_GetVelPGain(&temp_short);


	m_vel_p_gain = temp_short;


  m_servoMotor->UCMC_GetVelIGain(&temp_short);
 
	m_vel_i_gain = temp_short;

  m_servoMotor->UCMC_GetTargetVelocity(&temp_short);

  m_vel_target = temp_short;


  m_servoMotor->UCMC_GetActualVelocity(&temp_short);
 

  m_servoMotor->UCMC_GetMaxVelocity(&m_vel_maxvel);
 



  /////////////////////////POSITION/////////////////////


  m_servoMotor->UCMC_GetPosPGain(&temp_short);

  m_pos_p_gain = temp_short;

  m_servoMotor->UCMC_GetPosIGain(&temp_short);
 
  m_pos_i_gain = temp_short;


  m_servoMotor->UCMC_GetPosDGain(&temp_short);
 
  m_pos_d_gain = temp_short;

  m_servoMotor->UCMC_GetNegativePositionLimit(&temp_long);
 
  m_pos_negative = temp_long;

  m_servoMotor->UCMC_GetPositivePositionLimit(&temp_long);

  m_pos_positive = temp_long;

  m_servoMotor->UCMC_GetTargetPosition(&temp_long);

  m_pos_target = temp_long;

  m_servoMotor->UCMC_GetActualPosition(&temp_long);

  m_pos_actual = temp_long;

  m_servoMotor->UCMC_GetActualPosition(&temp_long);
 
  m_servoMotor->UCMC_GetAbsolutePosition(&temp_long);

 
  UpdateData(FALSE);
}



void CUCMC_EducationKitDlg::OnFileopen() 
{


	// TODO: Add your control notification handler code here
	CString filename= "txt파일(*.txt)|*.txt|모든파일|*.*||";
	CFileDialog dlg(TRUE, ".txt", NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, filename);
		CFile file;
		FILE *fp;
		int size;
	char *buf;
	int line_count=0;
	CString mk;
	int i=0, k, count=0;int delpos[1500];

		
	char temp[100];

	if (dlg.DoModal()==IDOK)
	{

		filename=dlg.GetPathName();		

		fp=fopen(filename,"rt");
		

		for (i=0; i<1500; i++)
		{
			fscanf(fp,"%d",&posvelue[i]);
		}
		 fclose(fp);
	}
	for (count=0;count<1500; count++)
	{
	
			ref[count]=posvelue[count];
			if(control_mode==1&&count>0)
			{
				delpos[count]=ref[count]-ref[count-1];
				posvelue[count]=ref[count]+3.00*(ref[count]-ref[count-1])+5.52;
			}
			else if(control_mode==2 && count>1)
			{
				posvelue[count]=ref[count]+3.00*(ref[count]-ref[count-1])
				+10.00*(ref[count]-2*ref[count-1]+ref[count-2])+5.52;
			}
	}

}

void CUCMC_EducationKitDlg::OnFilesave() 
{


//if (vel[0]!=-1000000)
//{

	CString filename= "txt파일(*.txt)|*.txt|모든파일|*.*||";
	CFileDialog dlg(FALSE, ".txt", NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, filename);

	if (dlg.DoModal()==IDOK)
	{

		FILE *fp;
		filename=dlg.GetFileName();
			fp=fopen(filename, "w");

		int i;
		long position = 0;
		for (i=0; i<1500; i++)
		{
			position += vel[i];
			posvelue[i]=position;
			fprintf(fp, "%d\t%d\n", vel[i], position);
			//fprintf(fp, "%d\n", i);
		}
		fclose(fp);	
	}
//}
//else
//{
//	MessageBox("데이터가 생성되지 않았습니다");
//}

}

void CUCMC_EducationKitDlg::OnButtonCurCont() 
{
	// TODO: Add your control notification hndler code here

	UpdateData(TRUE);

	m_servoMotor->UCMC_SetContinuousCurrent(m_cur_cont);

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonCurOutput() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetOutputCurrent(m_cur_output);//

	UpdateData(FALSE);			
}



void CUCMC_EducationKitDlg::OnButtonCurTarget() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetTargetCurrent(m_cur_target);//

	UpdateData(FALSE);		
}
void CUCMC_EducationKitDlg::OnButtonCurPGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetCurrentPGain(m_cur_p_gain);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonCurIGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetCurrentIGain(m_cur_i_gain);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosActual() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetActualPosition(m_pos_actual);//

	UpdateData(FALSE);	
}

void CUCMC_EducationKitDlg::OnButtonPosDGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetPosDGain(m_pos_d_gain);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosIGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetPosIGain(m_pos_i_gain);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosNegative() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetNegativePositionLimit(m_pos_negative);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosPGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetPosPGain(m_pos_p_gain);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosPositive() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetPositivePositionLimit(m_pos_positive);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonPosTarget() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetTargetPosition(m_pos_target);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonVelAccel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetAcceleration(m_vel_accel);//

	UpdateData(FALSE);		
}

void CUCMC_EducationKitDlg::OnButtonVelEncpulse() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetEncoderCount(m_vel_encpulse);//

	UpdateData(FALSE);			
}

void CUCMC_EducationKitDlg::OnButtonVelIGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetVelIGain((short)m_vel_i_gain);//

	UpdateData(FALSE);			
}

void CUCMC_EducationKitDlg::OnButtonVelMaxvel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetMaxVelocity(m_vel_maxvel);//

	UpdateData(FALSE);			
}

void CUCMC_EducationKitDlg::OnButtonVelPGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_servoMotor->UCMC_SetVelPGain((short)m_vel_p_gain);//

	UpdateData(FALSE);			
}

void CUCMC_EducationKitDlg::OnButtonVelTarget() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_servoMotor->UCMC_SetTargetVelocity(m_vel_target);

	UpdateData(FALSE);			
}

//DEL void CUCMC_EducationKitDlg::OnSendAllValue() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 
//DEL 	UpdateData(FALSE);
//DEL }

void CUCMC_EducationKitDlg::OnSendArray() 
{
	// TODO: Add your control notification handler code here
	m_exe_buffer.EnableWindow(false);
	m_show_graph.EnableWindow(false);
	UpdateData(TRUE);

	if(isVelocityMode == true) {
		m_servoMotor->UCMC_TestVelocity(m_pos_target, vel, 1500);
	}
	else {
		m_servoMotor->UCMC_TestPosition(m_pos_target, vel, 1500);
	}

	FILE *fp;
	fp=fopen("data.txt", "w");
	int i;
	long position = 0;
	for (i=0; i<1500; i++)
	{
		position += vel[i];
		posvelue[i]=position;
		fprintf(fp, "%d\t%d\n", vel[i], position);
	}
	fclose(fp);
	m_exe_buffer.EnableWindow(TRUE);
	m_show_graph.EnableWindow(TRUE);
	UpdateData(FALSE);
}




void CUCMC_EducationKitDlg::DRAWGRAPH()
{

}


void CUCMC_EducationKitDlg::OnMake() 
{
	UpdateData(TRUE);
	int i;
	ref[0]=0;
	for (i=1; i<1500; i++)
	{
		ref[i]=i*m_ramp;
		if(control_mode==0)
		{
			posvelue[i]=i*m_ramp;
		}
		else
		{
			//posvelue[i]=(i+2.89)*m_ramp+6.52;
			posvelue[i]=(i+3.00)*m_ramp+5.52;
		}
	}
	UpdateData(FALSE);
}



void CUCMC_EducationKitDlg::callbackData()
{
	short avel;

	long apos;
	m_servoMotor->UCMC_GetActualVelocity(&avel);
	m_servoMotor->UCMC_GetActualPosition(&apos);
	
	CString dis;
	dis.Format("%d", avel);
	SetDlgItemText(IDC_TEST, dis);

	dis.Format("%d", apos );
	SetDlgItemText(IDC_TEST3, dis);

}

void CUCMC_EducationKitDlg::OnShowGraph() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i;
	int pos1=0;
//	if (vel[0]!=-1000000)
//	{
		dmk.sam_num=m_smapling_time;
		if(display_mode==0)
			for (i=0; i<1500;i++)
			{
				dmk.real_data[i]=posvelue[i];
			}
		else if(display_mode==1)
			for (i=0; i<1500;i++)
			{
				dmk.real_data[i]=posvelue[i]-pos1;
				pos1=posvelue[i];
			}
		else if(display_mode==2)
			for (i=0; i<1500;i++)
			{
				if(i==0)
					dmk.real_data[i]=0;
				else
					dmk.real_data[i]=ref[i-1]-posvelue[i];
			}
//	}
	dmk.DoModal();
	UpdateData(FALSE);
}

void CUCMC_EducationKitDlg::Callback_start()
{
	if (callbackflag==0)
 	{
 		callbackflag=1;
 		exam = mmtm_start(1, MultiMediaTimerFunc, 0);
 	}
 	else
 	{
 		return;
 	}
}

void CUCMC_EducationKitDlg::Callback_stop()
{
 	if (callbackflag==1)
 	{
 		callbackflag=0;
 		mmtm_stop(exam);
 	}
 	else
 	{
 		return;
 	}
}

void CUCMC_EducationKitDlg::OnSend_Buffer() 
{
	// TODO: Add your control notification handler code here
	m_send.EnableWindow(FALSE);
	m_exe_buffer.EnableWindow(FALSE);
	m_show_graph.EnableWindow(FALSE);
	UpdateData(TRUE);
	for(int i=0; i<1500; i++)
 	{
		m_servoMotor->UCMC_SetTargetPositionBuf(i,posvelue[i]);		
 	}
	
	m_send.EnableWindow(TRUE);
	m_exe_buffer.EnableWindow(TRUE);
	m_show_graph.EnableWindow(TRUE);
	UpdateData(FALSE);	
}

void CUCMC_EducationKitDlg::OnMakeStep() 
{
	// TODO: Add your control notification handler code here
	int i;
	UpdateData(TRUE);
	for (i=0; i<1500; i++)
	{
		posvelue[i]=m_step;//2048
		ref[i]=m_step; //2048;
	}	
}

void CUCMC_EducationKitDlg::OnRadioVel() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_SetControlMode(0x06);	
	isVelocityMode = true;
}

void CUCMC_EducationKitDlg::OnRadioPosition() 
{
	// TODO: Add your control notification handler code here
	m_servoMotor->UCMC_SetControlMode(0x04);	
	isVelocityMode = false;
}

void CUCMC_EducationKitDlg::OnMakePara() 
{
	// TODO: Add your control notification handler code here
	int i,delta_pos=0,pos=0,delta_pos1;
	posvelue[0]=0;
	ref[0]=0;
	if(control_mode!=0)
		pos=5.52;
	for (i=1; i<1500; i++)
	{
			delta_pos1=delta_pos;
			if(i<=150)
				delta_pos=i;
			else
				delta_pos=150;
			ref[i]=ref[i-1]+delta_pos;
			pos=pos+delta_pos;
			if(control_mode==0)
				posvelue[i]=pos;
			else if(control_mode==1)
				posvelue[i]=pos+3.00*delta_pos;
			else if(control_mode==2)
			{
				if(i<2||i>150)
					posvelue[i]=pos+3.00*delta_pos;
				else
					posvelue[i]=pos+3.00*delta_pos+10.00*(delta_pos-delta_pos1);
			}
			//posvelue[i]=pos;//+2.96*delta_pos;//+7.91*(delta_pos-delta_pos1);
	}
}


void CUCMC_EducationKitDlg::OnRadioFb() 
{
	// TODO: Add your control notification handler code here
	control_mode=0;
}

void CUCMC_EducationKitDlg::OnRadioFf1() 
{
	// TODO: Add your control notification handler code here
	control_mode=1;	
}

void CUCMC_EducationKitDlg::OnRadioFf2() 
{
	// TODO: Add your control notification handler code here
	control_mode=2;
}

void CUCMC_EducationKitDlg::OnRadioGraphPos() 
{
	// TODO: Add your control notification handler code here
	display_mode=0;
}

void CUCMC_EducationKitDlg::OnRadioGraphVel() 
{
	// TODO: Add your control notification handler code here
	display_mode=1;
}

void CUCMC_EducationKitDlg::OnRadioGraphErr() 
{
	// TODO: Add your control notification handler code here
	display_mode=2;
}

void CUCMC_EducationKitDlg::OnMakeSine() 
{
	// TODO: Add your control notification handler code here
	int i;
	UpdateData(TRUE);
	for (i=0; i<1500; i++)
	{
		ref[i]=(int)(m_step*sin(2.0*PHI*i*m_freq*T));
		posvelue[i]=ref[i];
	}	
}
