// fmclientDoc.h : interface of the CFmclientDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMCLIENTDOC_H__28B8F8A4_679A_4458_87CA_A5DC76D4EE96__INCLUDED_)
#define AFX_FMCLIENTDOC_H__28B8F8A4_679A_4458_87CA_A5DC76D4EE96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "fmclientSet.h"


class CFmclientSrvrItem;

class CFmclientDoc : public COleServerDoc
{
protected: // create from serialization only
	CFmclientDoc();
	DECLARE_DYNCREATE(CFmclientDoc)

// Attributes
public:
	CFmclientSrvrItem* GetEmbeddedItem()
		{ return (CFmclientSrvrItem*)COleServerDoc::GetEmbeddedItem(); }
	CFmclientSet m_fmclientSet;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmclientDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFmclientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFmclientDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCLIENTDOC_H__28B8F8A4_679A_4458_87CA_A5DC76D4EE96__INCLUDED_)
