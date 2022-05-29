#pragma once
#include "afxwin.h"



// CObjectForm �� ���Դϴ�.
class CStaticObj;
class CObjectForm : public CFormView
{
	DECLARE_DYNCREATE(CObjectForm)

protected:
	CObjectForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CObjectForm();
public:
	enum { IDD = IDD_OBJECTFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ObjList;
	float m_fPosX;
	float m_PosY;
	float m_PosZ;
	float m_ScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	afx_msg void OnLbnSelchangeObjlist();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangePosx();
	afx_msg void OnEnChangePosy();
	afx_msg void OnEnChangePosz();
	afx_msg void OnEnChangeScalex();
	afx_msg void OnEnChangeScaley();
	afx_msg void OnEnChangeScalez();
	virtual void OnInitialUpdate();
};


