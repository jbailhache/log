// fmclientSet.cpp : implementation of the CFmclientSet class
//

#include "stdafx.h"
#include "fmclient.h"
#include "fmclientSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmclientSet implementation

IMPLEMENT_DYNAMIC(CFmclientSet, CRecordset)

CFmclientSet::CFmclientSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CFmclientSet)
	m_ID = _T("");
	m_FILE = _T("");
	m_CNX = _T("");
	m_DCNX = _T("");
	m_TIME = 0.0;
	m_PGENB = 0.0;
	m_HGN = 0.0;
	m_REM = _T("");
	m_READ = FALSE;
	m_RTR = 0.0;
	m_LINE = 0.0;
	m_SPEED = 0.0;
	m_NOISE = 0.0;
	m_SIGNALS = 0.0;
	m_SIGNALQ = 0.0;
	m_PRINTED = FALSE;
	m_OCR = FALSE;
	m_SAVED = FALSE;
	m_FRS_REF = 0.0;
	m_FRS_SAV = FALSE;
	m_FRS_OCR = FALSE;
	m_FRS_EXP = FALSE;
	m_FRS_PRT = FALSE;
	m_nFields = 23;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CFmclientSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=AT01");
}

CString CFmclientSet::GetDefaultSQL()
{
	return _T("[REC]");
}

void CFmclientSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CFmclientSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[FILE]"), m_FILE);
	RFX_Text(pFX, _T("[CNX]"), m_CNX);
	RFX_Text(pFX, _T("[DCNX]"), m_DCNX);
	RFX_Double(pFX, _T("[TIME]"), m_TIME);
	RFX_Double(pFX, _T("[PGENB]"), m_PGENB);
	RFX_Double(pFX, _T("[HGN]"), m_HGN);
	RFX_Text(pFX, _T("[REM]"), m_REM);
	RFX_Bool(pFX, _T("[READ]"), m_READ);
	RFX_Double(pFX, _T("[RTR]"), m_RTR);
	RFX_Double(pFX, _T("[LINE]"), m_LINE);
	RFX_Double(pFX, _T("[SPEED]"), m_SPEED);
	RFX_Double(pFX, _T("[NOISE]"), m_NOISE);
	RFX_Double(pFX, _T("[SIGNALS]"), m_SIGNALS);
	RFX_Double(pFX, _T("[SIGNALQ]"), m_SIGNALQ);
	RFX_Bool(pFX, _T("[PRINTED]"), m_PRINTED);
	RFX_Bool(pFX, _T("[OCR]"), m_OCR);
	RFX_Bool(pFX, _T("[SAVED]"), m_SAVED);
	RFX_Double(pFX, _T("[FRS_REF]"), m_FRS_REF);
	RFX_Bool(pFX, _T("[FRS_SAV]"), m_FRS_SAV);
	RFX_Bool(pFX, _T("[FRS_OCR]"), m_FRS_OCR);
	RFX_Bool(pFX, _T("[FRS_EXP]"), m_FRS_EXP);
	RFX_Bool(pFX, _T("[FRS_PRT]"), m_FRS_PRT);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CFmclientSet diagnostics

#ifdef _DEBUG
void CFmclientSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFmclientSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
