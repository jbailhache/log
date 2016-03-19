// fmclientSet.h : interface of the CFmclientSet class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMCLIENTSET_H__D7E357FA_6037_4404_975C_FFF0C7743066__INCLUDED_)
#define AFX_FMCLIENTSET_H__D7E357FA_6037_4404_975C_FFF0C7743066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFmclientSet : public CRecordset
{
public:
	CFmclientSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CFmclientSet)

// Field/Param Data
	//{{AFX_FIELD(CFmclientSet, CRecordset)
	CString	m_ID;
	CString	m_FILE;
	CString	m_CNX;
	CString	m_DCNX;
	double	m_TIME;
	double	m_PGENB;
	double	m_HGN;
	CString	m_REM;
	BOOL	m_READ;
	double	m_RTR;
	double	m_LINE;
	double	m_SPEED;
	double	m_NOISE;
	double	m_SIGNALS;
	double	m_SIGNALQ;
	BOOL	m_PRINTED;
	BOOL	m_OCR;
	BOOL	m_SAVED;
	double	m_FRS_REF;
	BOOL	m_FRS_SAV;
	BOOL	m_FRS_OCR;
	BOOL	m_FRS_EXP;
	BOOL	m_FRS_PRT;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmclientSet)
	public:
	virtual CString GetDefaultConnect();	// Default connection string
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCLIENTSET_H__D7E357FA_6037_4404_975C_FFF0C7743066__INCLUDED_)

