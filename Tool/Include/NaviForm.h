#pragma once



// CNaviForm 폼 뷰입니다.

class CNaviForm : public CFormView
{
	DECLARE_DYNCREATE(CNaviForm)

protected:
	CNaviForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNaviForm();

public:
	enum { IDD = IDD_NAVIFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


