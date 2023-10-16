#if !defined(AFX_GRAPHDLG_H__D5C2CF91_1D00_4F93_BD72_C45C8727AA33__INCLUDED_)
#define AFX_GRAPHDLG_H__D5C2CF91_1D00_4F93_BD72_C45C8727AA33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GRAPHDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GRAPHDLG dialog

class GRAPHDLG : public CDialog
{
// Construction
public:
	void SetValueOut(int x);
	void Find_Graph_data(int real_x);
	void Draw_Trace_Line(int x);
	GRAPHDLG(CWnd* pParent = NULL);   // standard constructor
    CRect rect;

	long real_data[1500];
	CPoint graph_data[1500];
	CPoint mouse_point1, mouse_point2;
	void Find_real_data(int x);
	int Click_flag;
	int max_data;
	int min_data;

	int data_puls;	
	int zero_off;
	int m;

	int sam_num;

// Dialog Data
	//{{AFX_DATA(GRAPHDLG)
	enum { IDD = IDD_DIALOG1 };
	CStatic	m_rect;
	int		m_position;
	int		m_sampling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GRAPHDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GRAPHDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpBotton();
	afx_msg void OnDownButton();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDLG_H__D5C2CF91_1D00_4F93_BD72_C45C8727AA33__INCLUDED_)
