// fmservwDoc.h : interface of the CFmservwDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMSERVWDOC_H__841E3E06_8F6A_4F6B_8201_E28B850B627D__INCLUDED_)
#define AFX_FMSERVWDOC_H__841E3E06_8F6A_4F6B_8201_E28B850B627D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "fmservwSet.h"


class CFmservwDoc : public CDocument
{
protected: // create from serialization only
	CFmservwDoc();
	DECLARE_DYNCREATE(CFmservwDoc)

// Attributes
public:
	CFmservwSet m_fmservwSet;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmservwDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFmservwDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmservwDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMSERVWDOC_H__841E3E06_8F6A_4F6B_8201_E28B850B627D__INCLUDED_)
