// testprintView.h : interface of the CTestprintView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTPRINTVIEW_H__089F464E_5A26_4EB2_AFCF_457DBAB70495__INCLUDED_)
#define AFX_TESTPRINTVIEW_H__089F464E_5A26_4EB2_AFCF_457DBAB70495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestprintView : public CView
{
// protected: // create from serialization only
public:
	CTestprintView();
	DECLARE_DYNCREATE(CTestprintView)

// Attributes
public:
	CTestprintDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestprintView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint (CDC *pDC, CPrintInfo *pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestprintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// protected:

// Generated message map functions
// protected:
	//{{AFX_MSG(CTestprintView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in testprintView.cpp
inline CTestprintDoc* CTestprintView::GetDocument()
   { return (CTestprintDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPRINTVIEW_H__089F464E_5A26_4EB2_AFCF_457DBAB70495__INCLUDED_)
