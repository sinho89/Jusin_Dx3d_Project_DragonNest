#pragma once
#include "afxwin.h"



// CObjectForm 폼 뷰입니다.
class CStaticObj;
class CObjectForm : public CFormView
{
	DECLARE_DYNCREATE(CObjectForm)

protected:
	CObjectForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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


