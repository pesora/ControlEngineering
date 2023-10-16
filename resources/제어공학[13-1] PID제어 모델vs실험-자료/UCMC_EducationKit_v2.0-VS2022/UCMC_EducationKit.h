// UCMC_EducationKit.h : main header file for the UCMC_EDUCATIONKIT application
//

#if !defined(AFX_UCMC_EDUCATIONKIT_H__2100F8A6_F173_4126_B46B_EEB0187EC5F9__INCLUDED_)
#define AFX_UCMC_EDUCATIONKIT_H__2100F8A6_F173_4126_B46B_EEB0187EC5F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUCMC_EducationKitApp:
// See UCMC_EducationKit.cpp for the implementation of this class
//

class CUCMC_EducationKitApp : public CWinApp
{
public:
	CUCMC_EducationKitApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUCMC_EducationKitApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUCMC_EducationKitApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UCMC_EDUCATIONKIT_H__2100F8A6_F173_4126_B46B_EEB0187EC5F9__INCLUDED_)
