// visumfcDoc.cpp : implementation of the CVisumfcDoc class
//

#include "stdafx.h"
#include "visumfc.h"

#include "visumfcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisumfcDoc

IMPLEMENT_DYNCREATE(CVisumfcDoc, CDocument)

BEGIN_MESSAGE_MAP(CVisumfcDoc, CDocument)
	//{{AFX_MSG_MAP(CVisumfcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisumfcDoc construction/destruction

CVisumfcDoc::CVisumfcDoc()
{
	// TODO: add one-time construction code here

}

CVisumfcDoc::~CVisumfcDoc()
{
}

BOOL CVisumfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVisumfcDoc serialization

void CVisumfcDoc::Serialize(CArchive& ar)
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
// CVisumfcDoc diagnostics

#ifdef _DEBUG
void CVisumfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVisumfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisumfcDoc commands
