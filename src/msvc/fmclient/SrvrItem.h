// SrvrItem.h : interface of the CFmclientSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__0473E892_7655_40FD_AD91_1CA5A329A6AF__INCLUDED_)
#define AFX_SRVRITEM_H__0473E892_7655_40FD_AD91_1CA5A329A6AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFmclientSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CFmclientSrvrItem)

// Constructors
public:
	CFmclientSrvrItem(CFmclientDoc* pContainerDoc);

// Attributes
	CFmclientDoc* GetDocument() const
		{ return (CFmclientDoc*)COleServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmclientSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CFmclientSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__0473E892_7655_40FD_AD91_1CA5A329A6AF__INCLUDED_)
