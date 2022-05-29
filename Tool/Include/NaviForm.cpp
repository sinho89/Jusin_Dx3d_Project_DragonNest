// NaviForm.cpp : 구현 파일입니다.
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


// CNaviForm 진단입니다.

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


// CNaviForm 메시지 처리기입니다.
