// fmclientDoc.cpp : implementation of the CFmclientDoc class
//

#include "stdafx.h"
#include "fmclient.h"

#include "fmclientSet.h"
#include "fmclientDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc

IMPLEMENT_DYNCREATE(CFmclientDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CFmclientDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CFmclientDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc construction/destruction

CFmclientDoc::CFmclientDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CFmclientDoc::~CFmclientDoc()
{
}

BOOL CFmclientDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc server implementation

COleServerItem* CFmclientDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CFmclientSrvrItem* pItem = new CFmclientSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}



/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc serialization

void CFmclientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc diagnostics

#ifdef _DEBUG
void CFmclientDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CFmclientDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmclientDoc commands
