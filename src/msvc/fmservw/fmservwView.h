// fmservwView.h : interface of the CFmservwView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMSERVWVIEW_H__3AF017BE_FB26_4A76_BA11_8D6B976D2106__INCLUDED_)
#define AFX_FMSERVWVIEW_H__3AF017BE_FB26_4A76_BA11_8D6B976D2106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFmservwSet;

class CFmservwView : public CRecordView
{
protected: // create from serialization only
	CFmservwView();
	DECLARE_DYNCREATE(CFmservwView)

public:
	//{{AFX_DATA(CFmservwView)
	enum{ IDD = IDD_FMSERVW_FORM };
	CFmservwSet* m_pSet;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CFmservwDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmservwView)
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
	virtual ~CFmservwView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmservwView)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in fmservwView.cpp
inline CFmservwDoc* CFmservwView::GetDocument()
   { return (CFmservwDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMSERVWVIEW_H__3AF017BE_FB26_4A76_BA11_8D6B976D2106__INCLUDED_)
