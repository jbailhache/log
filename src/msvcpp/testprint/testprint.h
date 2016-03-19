// testprint.h : main header file for the TESTPRINT application
//

#if !defined(AFX_TESTPRINT_H__9ABDE8E9_1252_437E_A96F_38ECAD77035E__INCLUDED_)
#define AFX_TESTPRINT_H__9ABDE8E9_1252_437E_A96F_38ECAD77035E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestprintApp:
// See testprint.cpp for the implementation of this class
//

class CTestprintApp : public CWinApp
{
public:
	CTestprintApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestprintApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CTestprintApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPRINT_H__9ABDE8E9_1252_437E_A96F_38ECAD77035E__INCLUDED_)
