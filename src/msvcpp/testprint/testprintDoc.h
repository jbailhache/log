// testprintDoc.h : interface of the CTestprintDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTPRINTDOC_H__04ACC0A3_6E9F_4FC2_B13F_C11F0D83D61E__INCLUDED_)
#define AFX_TESTPRINTDOC_H__04ACC0A3_6E9F_4FC2_B13F_C11F0D83D61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestprintDoc : public CDocument
{
protected: // create from serialization only
	CTestprintDoc();
	DECLARE_DYNCREATE(CTestprintDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestprintDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestprintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestprintDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CTestprintDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPRINTDOC_H__04ACC0A3_6E9F_4FC2_B13F_C11F0D83D61E__INCLUDED_)
