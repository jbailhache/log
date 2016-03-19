// visumfcView.h : interface of the CVisumfcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUMFCVIEW_H__0CDAEADF_EFF2_4C59_9C89_F18EC3701A07__INCLUDED_)
#define AFX_VISUMFCVIEW_H__0CDAEADF_EFF2_4C59_9C89_F18EC3701A07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVisumfcView : public CView
{
protected: // create from serialization only
	CVisumfcView();
	DECLARE_DYNCREATE(CVisumfcView)

// Attributes
public:
	CVisumfcDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisumfcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVisumfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVisumfcView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in visumfcView.cpp
inline CVisumfcDoc* CVisumfcView::GetDocument()
   { return (CVisumfcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUMFCVIEW_H__0CDAEADF_EFF2_4C59_9C89_F18EC3701A07__INCLUDED_)
