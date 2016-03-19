// fmclientView.h : interface of the CFmclientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMCLIENTVIEW_H__32918C20_8DA1_466D_BE4F_3CB477107202__INCLUDED_)
#define AFX_FMCLIENTVIEW_H__32918C20_8DA1_466D_BE4F_3CB477107202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFmclientSet;

class CFmclientView : public CRecordView
{
protected: // create from serialization only
	CFmclientView();
	DECLARE_DYNCREATE(CFmclientView)

public:
	//{{AFX_DATA(CFmclientView)
	enum{ IDD = IDD_FMCLIENT_FORM };
	CFmclientSet* m_pSet;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CFmclientDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmclientView)
	public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFmclientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmclientView)
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in fmclientView.cpp
inline CFmclientDoc* CFmclientView::GetDocument()
   { return (CFmclientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCLIENTVIEW_H__32918C20_8DA1_466D_BE4F_3CB477107202__INCLUDED_)
