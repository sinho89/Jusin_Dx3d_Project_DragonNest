// ObjectForm.cpp : 구현 파일입니다.
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


// CObjectForm 진단입니다.

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


// CObjectForm 메시지 처리기입니다.

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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnBnClickedOk()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangePosx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangePosy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangePosz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangeScalex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangeScaley()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectForm::OnEnChangeScalez()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
