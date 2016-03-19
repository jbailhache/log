// testprintView.cpp : implementation of the CTestprintView class
//

#include "stdafx.h"
#include "testprint.h"

#include "testprintDoc.h"
#include "testprintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestprintView

IMPLEMENT_DYNCREATE(CTestprintView, CView)

BEGIN_MESSAGE_MAP(CTestprintView, CView)
	//{{AFX_MSG_MAP(CTestprintView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestprintView construction/destruction

CTestprintView::CTestprintView()
{
	// TODO: add construction code here
	/*
	CPrintInfo *pInfo = new CPrintInfo();
	OnPreparePrinting (pInfo);
	CDC *pDC = GetDC();
	OnBeginPrinting (pDC, pInfo);
	OnPrint (pDC, pInfo);
	*/
}

CTestprintView::~CTestprintView()
{
}

BOOL CTestprintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestprintView drawing

void CTestprintView::OnDraw(CDC* pDC)
{
	CTestprintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestprintView printing

BOOL CTestprintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestprintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestprintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestprintView::OnPrint (CDC *pDC, CPrintInfo *pInfo)
{
	pDC->TextOut (100, 60, "Test impression MFC");
	RECT rect;
	rect.left = 300;
	rect.top = 200;
	rect.right = 2000;
	rect.bottom = 900;
	pDC->Ellipse (&rect);
}

/////////////////////////////////////////////////////////////////////////////
// CTestprintView diagnostics

#ifdef _DEBUG
void CTestprintView::AssertValid() const
{
	CView::AssertValid();
}

void CTestprintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestprintDoc* CTestprintView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestprintDoc)));
	return (CTestprintDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CTestprintView message handlers
