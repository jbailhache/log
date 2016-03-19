// testprintDoc.cpp : implementation of the CTestprintDoc class
//

#include "stdafx.h"
#include "testprint.h"

#include "testprintDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestprintDoc

IMPLEMENT_DYNCREATE(CTestprintDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestprintDoc, CDocument)
	//{{AFX_MSG_MAP(CTestprintDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTestprintDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CTestprintDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ITestprint to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {66686B4B-87FB-4BA8-81CE-601BC3C21C91}
static const IID IID_ITestprint =
{ 0x66686b4b, 0x87fb, 0x4ba8, { 0x81, 0xce, 0x60, 0x1b, 0xc3, 0xc2, 0x1c, 0x91 } };

BEGIN_INTERFACE_MAP(CTestprintDoc, CDocument)
	INTERFACE_PART(CTestprintDoc, IID_ITestprint, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestprintDoc construction/destruction

CTestprintDoc::CTestprintDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CTestprintDoc::~CTestprintDoc()
{
	AfxOleUnlockApp();
}

BOOL CTestprintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestprintDoc serialization

void CTestprintDoc::Serialize(CArchive& ar)
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
// CTestprintDoc diagnostics

#ifdef _DEBUG
void CTestprintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestprintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestprintDoc commands
