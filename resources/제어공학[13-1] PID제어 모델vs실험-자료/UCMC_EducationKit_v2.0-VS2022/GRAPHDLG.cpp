// GRAPHDLG.cpp : implementation file
//

#include "stdafx.h"
#include "UCMC_EducationKit.h"
#include "GRAPHDLG.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GRAPHDLG dialog
#define  PI 3.141592

GRAPHDLG::GRAPHDLG(CWnd* pParent /*=NULL*/)
	: CDialog(GRAPHDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(GRAPHDLG)
	m_position = 0;
	m_sampling = 0;
	//}}AFX_DATA_INIT

sam_num=1500;

	int i;

	for (i=0; i<sam_num; i++)
	{
		real_data[i]=127.5*sin((2*PI/sam_num)*i);
	}

    Click_flag=0;
}


void GRAPHDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GRAPHDLG)
	DDX_Control(pDX, IDC_DLG_IMAGE, m_rect);
	DDX_Text(pDX, IDC_POSITION_EDIT, m_position);
	DDX_Text(pDX, IDC_SAMPLING_EIDT, m_sampling);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GRAPHDLG, CDialog)
	//{{AFX_MSG_MAP(GRAPHDLG)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_UP_BOTTON, OnUpBotton)
	ON_BN_CLICKED(IDC_DOWN_BUTTON, OnDownButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GRAPHDLG message handlers

BOOL GRAPHDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString k;
	int i;
	m_rect.GetClientRect(&rect);
	rect.NormalizeRect();
	m_rect.ClientToScreen(&rect);
	ScreenToClient(&rect);	

	max_data=-90000000;
	min_data=900000000;


	for (i=0; i<sam_num; i++)
	{
		if (real_data[i]>max_data)
		{
			max_data=real_data[i];
		}
		if (real_data[i]<min_data)
		{
			min_data=real_data[i];
		}
	}
/*
	if (abs(max_data)>abs(min_data))
	{
		data_puls=max_data;
	}
	else
	{
		data_puls=min_data;
	}
*/
	data_puls=(max_data-min_data);


	int line;
	line= data_puls;
m=1;
	while (line>0)
	{

		line=line/m;
		m=m*10;
	}

	

	m=m/10;

	if (min_data<0)
	{
		zero_off= -min_data+(data_puls/20);
	}
	else
	{
		zero_off=data_puls/20;
	}

	data_puls=data_puls+data_puls/10;

	for (i=0; i<sam_num; i++)
	{
		graph_data[i].x=rect.left+(rect.Width()*i)/sam_num;
		graph_data[i].y=rect.bottom-(rect.Height()*(real_data[i]+zero_off))/data_puls;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GRAPHDLG::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));

	dc.FillRect(rect,&brush );	
	int i, j, k;
	CPen pen, pen2, pen3, pen4,*pOldpen;

	pen.CreatePen(PS_SOLID, 1,RGB(0,255,255));

	pOldpen=dc.SelectObject(&pen);

//	dc.MoveTo(rect.left, rect.top+rect.Height()/2);
//	dc.LineTo(rect.right, rect.top+rect.Height()/2);
//	dc.MoveTo(rect.left+rect.Width()/2, rect.top);
//	dc.LineTo(rect.left+rect.Width()/2, rect.bottom);




	pen3.CreatePen(PS_DOT, 1,RGB(100,100,100));
	pOldpen=dc.SelectObject(&pen3);


	CString x_val;
	CRect x_pos;

	CString y_val;
	CRect y_pos;


	y_pos.left= rect.left- 40;
	y_pos.right= rect.left;


	x_pos.top =rect.bottom;
	x_pos.bottom =x_pos.top+20;
	


/////  y=0;
	k=rect.bottom-(rect.Height()*(zero_off))/data_puls;
	y_val.Format("%d", 0);
	y_pos.top=k;
	y_pos.bottom=k+20;
	dc.MoveTo(rect.left, k);
	dc.LineTo(rect.right, k);
	dc.DrawText(y_val,y_pos, DT_RIGHT);
/////
	

/*

////////minor///////////////////////

	for (i=m/2; i<max_data; i=i+m/2)
	{
		k=rect.bottom-(rect.Height()*(i+zero_off))/data_puls;
		y_pos.top=k;
		y_pos.bottom=k+20;
		y_val.Format("%d", i);
		dc.MoveTo(rect.left, k);
		dc.LineTo(rect.right, k);
		dc.DrawText(y_val,y_pos, DT_RIGHT);
	}
	for (i=-m/2; i>min_data; i=i-m/2)
	{
		k=rect.bottom-(rect.Height()*(i+zero_off))/data_puls;
		y_pos.top=k;
		y_pos.bottom=k+20;
		y_val.Format("%d", i);
		dc.MoveTo(rect.left, k);
		dc.LineTo(rect.right, k);
		dc.DrawText(y_val,y_pos, DT_RIGHT);
	}
*/
////// major//////



	for (i=m; i<max_data+(data_puls/20); i=i+m)
	{
		k=rect.bottom-(rect.Height()*(i+zero_off))/data_puls;
		y_pos.top=k;
		y_pos.bottom=k+20;
		y_val.Format("%d", i);
		dc.MoveTo(rect.left, k);
		dc.LineTo(rect.right, k);
		dc.DrawText(y_val,y_pos, DT_RIGHT);
	}
	for (i=-m; i>min_data-(data_puls/20); i=i-m)
	{
		k=rect.bottom-(rect.Height()*(i+zero_off))/data_puls;
		y_pos.top=k;
		y_pos.bottom=k+20;
		y_val.Format("%d", i);
		dc.MoveTo(rect.left, k);
		dc.LineTo(rect.right, k);
		dc.DrawText(y_val,y_pos, DT_RIGHT);
	}


///////////////////////////////////////

	for (j=sam_num/10; j<sam_num; j=j+(sam_num/10))
	{
		k=rect.left+(rect.Width()*j)/sam_num;
		x_pos.left=k-20;
		x_pos.right=x_pos.left+40;
		x_val.Format("%d", j);
		dc.MoveTo(k, rect.top);
		dc.LineTo(k, rect.bottom);
		dc.DrawText(x_val,x_pos, DT_CENTER);
	}
	

	pen4.CreatePen(PS_SOLID, 1,RGB(0,0,255));
	
	pOldpen=dc.SelectObject(&pen4);

	k=rect.bottom-(rect.Height()*(max_data+zero_off))/data_puls;
	y_val.Format("%d", max_data);
	y_pos.top=k;
	y_pos.bottom=k+20;
	dc.MoveTo(rect.left, k);
	dc.LineTo(rect.right, k);
	dc.SetTextColor(RGB(0,0,255));
	dc.DrawText(y_val,y_pos, DT_RIGHT);

	k=rect.bottom-(rect.Height()*(min_data+zero_off))/data_puls;
	y_val.Format("%d", min_data);
	y_pos.top=k;
	y_pos.bottom=k+20;
	dc.MoveTo(rect.left, k);
	dc.LineTo(rect.right, k);
	dc.SetTextColor(RGB(0,0,255));
	dc.DrawText(y_val,y_pos, DT_RIGHT);



	pen2.CreatePen(PS_SOLID, 1,RGB(255,0,0));

	pOldpen=dc.SelectObject(&pen2);

	for (i=0; i<sam_num-1; i++)
	{
		dc.MoveTo(graph_data[i]);
		dc.LineTo(graph_data[i+1]);
	}

}

void GRAPHDLG::OnDestroy() 
{
	CDialog::OnDestroy();
	

}



void GRAPHDLG::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (Click_flag==1 && point.y>rect.top && point.y<rect.bottom && point.x>rect.left && point.x<rect.right)
	{
		Draw_Trace_Line(point.x);
		Find_real_data(point.x);
		SetValueOut(point.x);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


void GRAPHDLG::Find_real_data(int x)
{
  int real_x;
  real_x=((x-rect.left)*sam_num)/rect.Width();

  m_sampling=real_x;
  m_position=real_data[m_sampling];
  UpdateData(FALSE);
}

void GRAPHDLG::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Click_flag=0;
	CDialog::OnLButtonUp(nFlags, point);
}

void GRAPHDLG::OnUpBotton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_sampling<sam_num)
	{
		m_sampling++;
		m_position=real_data[m_sampling];
		Find_Graph_data(m_sampling);
	}
	UpdateData(FALSE);
}

void GRAPHDLG::OnDownButton() 
{
	UpdateData(TRUE);
	if (m_sampling>0)
	{
		m_sampling--;
		m_position=real_data[m_sampling];
		Find_Graph_data(m_sampling);
	}
	UpdateData(FALSE);
	
}

void GRAPHDLG::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Click_flag=1;
	CDialog::OnLButtonDown(nFlags, point);
}

void GRAPHDLG::Draw_Trace_Line(int x)
{
		CClientDC dc(this);
		dc.SetROP2(R2_XORPEN);
		dc.SelectStockObject(WHITE_PEN);
		dc.MoveTo(mouse_point1.x, rect.top);
		dc.LineTo(mouse_point1.x, rect.bottom);
		dc.MoveTo(x, rect.top);
		dc.LineTo(x, rect.bottom);
		mouse_point1.x=x;
}

void GRAPHDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		CClientDC dc(this);
		dc.SetROP2(R2_XORPEN);
		dc.SelectStockObject(WHITE_PEN);
		dc.MoveTo(mouse_point1.x, rect.top);
		dc.LineTo(mouse_point1.x, rect.bottom);
		dc.MoveTo(0, rect.top);
		dc.LineTo(0, rect.bottom);
		mouse_point1.x=0;

	
	CDialog::OnClose();
}

void GRAPHDLG::Find_Graph_data(int real_x)
{
	int Graph_x;
	Graph_x= rect.left+(rect.Width()*real_x)/sam_num;
	Draw_Trace_Line(Graph_x);

}

void GRAPHDLG::OnOK() 
{
		CClientDC dc(this);
		dc.SetROP2(R2_XORPEN);
		dc.SelectStockObject(WHITE_PEN);
		dc.MoveTo(mouse_point1.x, rect.top);
		dc.LineTo(mouse_point1.x, rect.bottom);
		dc.MoveTo(0, rect.top);
		dc.LineTo(0, rect.bottom);
		mouse_point1.x=0;
	
	CDialog::OnOK();
}

void GRAPHDLG::OnCancel() 
{
		CClientDC dc(this);
		dc.SetROP2(R2_XORPEN);
		dc.SelectStockObject(WHITE_PEN);
		dc.MoveTo(mouse_point1.x, rect.top);
		dc.LineTo(mouse_point1.x, rect.bottom);
		dc.MoveTo(0, rect.top);
		dc.LineTo(0, rect.bottom);
		mouse_point1.x=0;
	
	CDialog::OnCancel();
}

void GRAPHDLG::SetValueOut(int x)
{

/*
		CString val;
		CRect pos;

		int real_x, k;
		real_x=((x-rect.left)*sam_num)/rect.Width();
		k=rect.bottom-(rect.Height()*(real_data[real_x]+zero_off))/data_puls;

		pos.left= x+10;
		pos.right= x+80;
		pos.top= k;
		pos.bottom=k+20;

		val.Format("%d, %d",real_x,real_data[real_x]);
		CClientDC dc(this);
		dc.DrawText(val, pos, DT_LEFT);

*/
	
	
}
