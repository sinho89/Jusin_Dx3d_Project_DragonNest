#pragma once



// CNaviForm �� ���Դϴ�.

class CNaviForm : public CFormView
{
	DECLARE_DYNCREATE(CNaviForm)

protected:
	CNaviForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


