// visumfcDoc.h : interface of the CVisumfcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUMFCDOC_H__0996A4F8_41FA_433E_A52B_64630852665A__INCLUDED_)
#define AFX_VISUMFCDOC_H__0996A4F8_41FA_433E_A52B_64630852665A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVisumfcDoc : public CDocument
{
protected: // create from serialization only
	CVisumfcDoc();
	DECLARE_DYNCREATE(CVisumfcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisumfcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVisumfcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVisumfcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISUMFCDOC_H__0996A4F8_41FA_433E_A52B_64630852665A__INCLUDED_)
