// NaviForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviForm.h"


// CNaviForm

IMPLEMENT_DYNCREATE(CNaviForm, CFormView)

CNaviForm::CNaviForm()
	: CFormView(CNaviForm::IDD)
{

}

CNaviForm::~CNaviForm()
{
}

void CNaviForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaviForm, CFormView)
END_MESSAGE_MAP()


// CNaviForm �����Դϴ�.

#ifdef _DEBUG
void CNaviForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNaviForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNaviForm �޽��� ó�����Դϴ�.
