// ObjectForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectForm.h"

#include "MainFrm.h"
#include "MainApp.h"
#include "ToolView.h"
// CObjectForm

IMPLEMENT_DYNCREATE(CObjectForm, CFormView)

CObjectForm::CObjectForm()
	: CFormView(CObjectForm::IDD)
	, m_fPosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_ScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
{
}

CObjectForm::~CObjectForm()
{
}

void CObjectForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJLIST, m_ObjList);
	DDX_Text(pDX, IDC_POSX, m_fPosX);
	DDX_Text(pDX, IDC_POSY, m_PosY);
	DDX_Text(pDX, IDC_POSZ, m_PosZ);
	DDX_Text(pDX, IDC_SCALEX, m_ScaleX);
	DDX_Text(pDX, IDC_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_SCALEZ, m_fScaleZ);
}

BEGIN_MESSAGE_MAP(CObjectForm, CFormView)
	ON_LBN_SELCHANGE(IDC_OBJLIST, &CObjectForm::OnLbnSelchangeObjlist)
	ON_BN_CLICKED(IDC_SAVE, &CObjectForm::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CObjectForm::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_OK, &CObjectForm::OnBnClickedOk)
	ON_EN_CHANGE(IDC_POSX, &CObjectForm::OnEnChangePosx)
	ON_EN_CHANGE(IDC_POSY, &CObjectForm::OnEnChangePosy)
	ON_EN_CHANGE(IDC_POSZ, &CObjectForm::OnEnChangePosz)
	ON_EN_CHANGE(IDC_SCALEX, &CObjectForm::OnEnChangeScalex)
	ON_EN_CHANGE(IDC_SCALEY, &CObjectForm::OnEnChangeScaley)
	ON_EN_CHANGE(IDC_SCALEZ, &CObjectForm::OnEnChangeScalez)
END_MESSAGE_MAP()


// CObjectForm �����Դϴ�.

#ifdef _DEBUG
void CObjectForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CObjectForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CObjectForm �޽��� ó�����Դϴ�.

void CObjectForm::OnLbnSelchangeObjlist()
{
	/*UpdateData(TRUE);

	CString strSelectName;

	int iSelNum = m_ObjList.GetCurSel();
	m_ObjList.GetText(iSelNum, strSelectName);

	CMainFrame*		pMainFrame	= (CMainFrame*)AfxGetMainWnd();
	CMainApp*	pMain	= (CMainApp*)pMainFrame->m_pMain;
		
	TCHAR*	tSelectName =  (TCHAR*)(LPCTSTR)strSelectName;

	pMain->m_szName = tSelectName;

	UpdateData(FALSE);*/

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnBnClickedOk()
{

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangePosx()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangePosy()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangePosz()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangeScalex()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangeScaley()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnEnChangeScalez()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CObjectForm::OnInitialUpdate()
{
	/*CFormView::OnInitialUpdate();

	TCHAR	szForderPath[MAX_PATH] = L"../bin/Resources/Mesh/StaticMesh/StaticObject/";
	char	szFullPath[MAX_PATH]  = "";

	WideCharToMultiByte(CP_ACP, NULL,
		szForderPath, -1, szFullPath, MAX_PATH, NULL, FALSE);

	_finddata_t		fd;
	long			handle;
	int				iResult = 1;
	
	strcat_s(szFullPath, "/*.X");
	handle = _findfirst(szFullPath, &fd);

	if(handle == -1)
		return;

	while(iResult != -1)
	{
		CString strPath = szForderPath;
		strPath += L"/";
		strPath += fd.name;

		CString strImageName;
		strImageName = PathFindFileName(strPath);
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());

		if(strImageName != "" && strImageName != "." && strImageName != "..")
		{		
			m_ObjList.AddString(strImageName);						
		}	
		iResult = _findnext(handle, &fd);
	}*/

	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
