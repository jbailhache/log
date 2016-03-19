// fmservwView.cpp : implementation of the CFmservwView class
//

#include "stdafx.h"
#include "fmservw.h"

#include "fmservwSet.h"
#include "fmservwDoc.h"
#include "fmservwView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmservwView

IMPLEMENT_DYNCREATE(CFmservwView, CRecordView)

BEGIN_MESSAGE_MAP(CFmservwView, CRecordView)
	//{{AFX_MSG_MAP(CFmservwView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmservwView construction/destruction

CFmservwView::CFmservwView()
	: CRecordView(CFmservwView::IDD)
{
	//{{AFX_DATA_INIT(CFmservwView)
		// NOTE: the ClassWizard will add member initialization here
	m_pSet = NULL;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CFmservwView::~CFmservwView()
{
}

void CFmservwView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmservwView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CFmservwView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRecordView::PreCreateWindow(cs);
}

void CFmservwView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_fmservwSet;
	CRecordView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CFmservwView printing

BOOL CFmservwView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFmservwView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFmservwView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFmservwView diagnostics

#ifdef _DEBUG
void CFmservwView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CFmservwView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CFmservwDoc* CFmservwView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFmservwDoc)));
	return (CFmservwDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFmservwView database support
CRecordset* CFmservwView::OnGetRecordset()
{
	return m_pSet;
}


/////////////////////////////////////////////////////////////////////////////
// CFmservwView message handlers

#include <winsock.h>
#include <afxsock.h>

// Function prototype
void StreamServer(short nPort,CFmservwSet* m_pSet);

#define PRINTERROR(s)	\
		fprintf(stderr,"\n%s: %d\n", s, WSAGetLastError())


void fmserv(CFmservwSet* m_pSet)
{

    WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;
	short nPort;

	nPort = 6030;

    
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		fprintf(stderr,"\n Wrong version\n");
		return;
	}
	

	//
	// Do the stuff a stream server does
	//
	StreamServer(nPort,m_pSet);

	
	//
	// Release WinSock
	//
	/*WSACleanup();*/

}

void StreamServer(short nPort,CFmservwSet* m_pSet)
{
//
	// Create a TCP/IP stream socket to "listen" with
	//
	SOCKET	listenSocket;

	listenSocket = socket(AF_INET,			// Address family
						  SOCK_STREAM,		// Socket type
						  IPPROTO_TCP);		// Protocol
	if (listenSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}


	//
	// Fill in the address structure
	//
	SOCKADDR_IN saServer;		

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;	// Let WinSock supply address
	saServer.sin_port = htons(nPort);		// Use port from command line

	//
	// bind the name to the socket
	//
	int nRet;

	nRet = bind(listenSocket,				// Socket 
				(LPSOCKADDR)&saServer,		// Our address
				sizeof(struct sockaddr));	// Size of address structure
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("bind()");
		closesocket(listenSocket);
		return;
	}

	//
	// This isn't normally done or required, but in this 
	// example we're printing out where the server is waiting
	// so that you can connect the example client.
	//
	int nLen;
	nLen = sizeof(SOCKADDR);
	char szBuf[256];

	nRet = gethostname(szBuf, sizeof(szBuf));
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("gethostname()");
		closesocket(listenSocket);
		return;
	}

	//
	// Show the server name and port number
	//
	printf("\nServer named %s waiting on port %d\n",
			szBuf, nPort);

	//
	// Set the socket to listen
	//

	printf("\nlisten()");
	nRet = listen(listenSocket,					// Bound socket
				  SOMAXCONN);					// Number of connection request queue
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("listen()");
		closesocket(listenSocket);
		return;
	}

	//
	// Wait for an incoming request
	//
	SOCKET	remoteSocket;

	printf("\nBlocking at accept()");
	remoteSocket = accept(listenSocket,			// Listening socket
						  NULL,					// Optional client address
						  NULL);
	if (remoteSocket == INVALID_SOCKET)
	{
		PRINTERROR("accept()");
		closesocket(listenSocket);
		return;
	}
	printf ("\nconnected");
	//
	// We're connected to a client
	// New socket descriptor returned already
	// has clients address

	//
	// Receive data from the client
	//
	memset(szBuf, 0, sizeof(szBuf));
	nRet = recv(remoteSocket,					// Connected client
				szBuf,							// Receive buffer
				sizeof(szBuf),					// Lenght of buffer
				0);								// Flags
	if (nRet == INVALID_SOCKET)
	{
		PRINTERROR("recv()");
		closesocket(listenSocket);
		closesocket(remoteSocket);
		return;
	}

	//
	// Display received data
	//
	printf("\nData received: %s", szBuf);

	//
	// Send data back to the client
	//
	/*
	strcpy(szBuf, "From the Server");
	nRet = send(remoteSocket,				// Connected socket
				szBuf,						// Data buffer
				strlen(szBuf),				// Lenght of data
				0);							// Flags
	*/
	m_pSet->MoveFirst();
	CString str;
	while (!m_pSet->IsEOF())
	{
		str.Format ("%s", m_pSet->m_FILE);
		nRet = send(remoteSocket,				// Connected socket
				str,						// Data buffer
				strlen(szBuf),				// Lenght of data
				0);							// Flags

		m_pSet->MoveNext();
	}

	strcpy (szBuf,"END");
	nRet = send(remoteSocket,				// Connected socket
				m_pSet->m_FILE,						// Data buffer
				strlen(szBuf),				// Lenght of data
				0);							// Flags
	//
	// Close BOTH sockets before exiting
	//
	closesocket(remoteSocket);
	closesocket(listenSocket);
	return;
}

void CFmservwView::OnButton1() 
{
	// TODO: Add your control notification handler code here
	fmserv(m_pSet);
}
