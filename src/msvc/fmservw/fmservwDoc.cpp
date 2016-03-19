// fmservwDoc.cpp : implementation of the CFmservwDoc class
//

#include "stdafx.h"
#include "fmservw.h"

#include "fmservwSet.h"
#include "fmservwDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmservwDoc

IMPLEMENT_DYNCREATE(CFmservwDoc, CDocument)

BEGIN_MESSAGE_MAP(CFmservwDoc, CDocument)
	//{{AFX_MSG_MAP(CFmservwDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmservwDoc construction/destruction

CFmservwDoc::CFmservwDoc()
{
	// TODO: add one-time construction code here

}

CFmservwDoc::~CFmservwDoc()
{
}

BOOL CFmservwDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFmservwDoc serialization

void CFmservwDoc::Serialize(CArchive& ar)
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
// CFmservwDoc diagnostics

#ifdef _DEBUG
void CFmservwDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFmservwDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmservwDoc commands
