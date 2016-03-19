// visumfc.h : main header file for the VISUMFC application
//

#if !defined(AFX_VISUMFC_H__2AD88AE6_8288_4892_AA90_39F7B90922CC__INCLUDED_)
#define AFX_VISUMFC_H__2AD88AE6_8288_4892_AA90_39F7B90922CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVisumfcApp:
// See visumfc.cpp for the implementation of this class
//

class CVisumfcApp : public CWinApp
{
public:
	CVisumfcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisumfcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVisumfcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUMFC_H__2AD88AE6_8288_4892_AA90_39F7B90922CC__INCLUDED_)
