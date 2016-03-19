// fmclientView.cpp : implementation of the CFmclientView class
//

#include "stdafx.h"
#include "fmclient.h"

#include "fmclientSet.h"
#include "fmclientDoc.h"
#include "fmclientView.h"

#include <winsock.h>
#include <afxsock.h>

#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmclientView

IMPLEMENT_DYNCREATE(CFmclientView, CRecordView)

BEGIN_MESSAGE_MAP(CFmclientView, CRecordView)
	//{{AFX_MSG_MAP(CFmclientView)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmclientView construction/destruction

CFmclientView::CFmclientView()
	: CRecordView(CFmclientView::IDD)
{
	//{{AFX_DATA_INIT(CFmclientView)
		// NOTE: the ClassWizard will add member initialization here
	m_pSet = NULL;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CFmclientView::~CFmclientView()
{
}

void CFmclientView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmclientView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CFmclientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRecordView::PreCreateWindow(cs);
}

void CFmclientView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_fmclientSet;
	CRecordView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CFmclientView printing

BOOL CFmclientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFmclientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFmclientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CFmclientView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CFmclientView diagnostics

#ifdef _DEBUG
void CFmclientView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CFmclientView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CFmclientDoc* CFmclientView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFmclientDoc)));
	return (CFmclientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmclientView database support
CRecordset* CFmclientView::OnGetRecordset()
{
	return m_pSet;
}

#define PRINTERROR(s)	\
		fprintf(stderr,"\n%s: %d\n", s, WSAGetLastError())

void getlist(CListBox *pList1)
{

	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;
	short nPort;

	char szBuf[1000];

	nPort = 6030;

    
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		fprintf(stderr,"\n Wrong version\n");
		return;
	}

	struct hostent *hp;
	hp = gethostbyname ("localhost");
	if (hp == NULL)
			/*main->Printf ("Unknown host\n")*/;
	else
	{
		struct sockaddr_in sa;
		SOCKET s;

		memset (&sa, 0, sizeof(sa));
		memcpy (&sa.sin_addr, hp->h_addr, hp->h_length);
		sa.sin_family = hp->h_addrtype;
		sa.sin_port = htons (6030);

		s = socket (hp->h_addrtype, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET)
		{
			/*main->Printf ("Invalid socket\n");*/
		}
		else
		{
			if (connect (s, (struct sockaddr *)&sa, sizeof sa) == SOCKET_ERROR)
			{
				closesocket (s);
				/*main->Printf ("Connect failed\n");*/
			}
			else
			{
				char buf[1000];
				int status;

				/*main->Printf ("Connected\n");*/

				/*
				sprintf (buf, "FAXRCV\n%d\n%d\n%s\n\n\n%d\n%d\n%d\n%d\n%s\n%s\n%s\n", 
					pFmrtr->rtr,
					pre->line,
					pre->id,
					pre->status,
					pre->hng,
					pre->duration,
					pre->pgnb,
					pre->rfn,
					xdate,
					xtime
					);	
					*/

				sprintf (buf, "BASEFAX\nc:\\faxmailer\\db\nUSER\nadmin\nPASS\npadmin\nLISTREC\n\n\n");

				status = send (s, buf, strlen(buf), 0);
				sprintf (buf, "send -> %d\n", status);
				/*main->Printf (buf);*/
				
	for (int i=0; i<270; i++)
	{
	memset(szBuf, 0, sizeof(szBuf));
	nRet = recv(s,					// Connected client
				szBuf,							// Receive buffer
				sizeof(szBuf),					// Lenght of buffer
				0);								// Flags
	if (!strcmp(szBuf,"END")) break;
	if (nRet == INVALID_SOCKET)
	{
		PRINTERROR("recv()");
		closesocket(s);
		return;
	}

	/* CList *pList1 = (CList *) GetDlgItem (w, IDC_LIST1); */

	pList1->AddString (szBuf);
	}
				status = closesocket(s);
				sprintf (buf, "closesocket -> %d\n", status);
				/*main->Printf (buf);*/


			}

		}
	

	}


}


/////////////////////////////////////////////////////////////////////////////
// CFmclientView message handlers

void CFmclientView::OnButton1() 
{
	// TODO: Add your control notification handler code here

	CListBox *pList1 = (CListBox *) GetDlgItem (IDC_LIST1);

	getlist(pList1);
	
}

void CFmclientView::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	
}
