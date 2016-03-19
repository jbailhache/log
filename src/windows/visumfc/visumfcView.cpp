// visumfcView.cpp : implementation of the CVisumfcView class
//

#include "stdafx.h"
#include "visumfc.h"

#include "visumfcDoc.h"
#include "visumfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView

IMPLEMENT_DYNCREATE(CVisumfcView, CView)

BEGIN_MESSAGE_MAP(CVisumfcView, CView)
	//{{AFX_MSG_MAP(CVisumfcView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView construction/destruction

CVisumfcView::CVisumfcView()
{
	// TODO: add construction code here

}

CVisumfcView::~CVisumfcView()
{
}

BOOL CVisumfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView drawing

void CVisumfcView::OnDraw(CDC* pDC)
{
	CVisumfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView printing

BOOL CVisumfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVisumfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVisumfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView diagnostics

#ifdef _DEBUG
void CVisumfcView::AssertValid() const
{
	CView::AssertValid();
}

void CVisumfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVisumfcDoc* CVisumfcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisumfcDoc)));
	return (CVisumfcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVisumfcView message handlers
