#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"
#include "Tool_Include.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_fCameraSpeed(0.f)
, m_bClick(false)
, m_bMouseFix(true) 

{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCameraSpeed = 500.f;

	m_vEye = *pEye;
	m_vAt  = *pAt;
	Invalidate_ViewMatrix();


	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINCX) / float(WINCY);
	m_fNear = 1.f;
	m_fFar  = 10000.f;

	Invalidate_ProMatrix();


	return S_OK;
}

void CDynamicCamera::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	if(Engine::Get_Input()->GetDIKeyState(DIK_LEFT) & 0x80)
	{
		D3DXVECTOR3	vRight;
		D3DXMATRIX	matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCameraSpeed * fTime;
		m_vAt  -= vRight * m_fCameraSpeed * fTime;
	}

	if(Engine::Get_Input()->GetDIKeyState(DIK_RIGHT) & 0x80)
	{
		D3DXVECTOR3	vRight;
		D3DXMATRIX	matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCameraSpeed * fTime;
		m_vAt  += vRight * m_fCameraSpeed * fTime;
	}

	if(Engine::Get_Input()->GetDIKeyState(DIK_UP) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCameraSpeed * fTime;
		m_vAt  += vLook * m_fCameraSpeed * fTime;
	}

	if(Engine::Get_Input()->GetDIKeyState(DIK_DOWN) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCameraSpeed * fTime;
		m_vAt  -= vLook * m_fCameraSpeed * fTime;
	}

	if(Engine::Get_Input()->GetDIKeyState(DIK_Q) & 0x80)
	{
		if(m_bClick)
			return;

		m_bClick = true;

		if(m_bMouseFix == true)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	if(m_bMouseFix == false)
		return;


}

void CDynamicCamera::Update(void)
{
	KeyCheck();

	if(m_bMouseFix)
	{
		FixMouse();
		MouseMove();
	}

	Engine::CCamera::Update();
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, 
									   const D3DXVECTOR3* pEye, 
									   const D3DXVECTOR3* pAt)
{
	CDynamicCamera*	pCamera = new CDynamicCamera(pDevice);

	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
	POINT	ptMouse = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDynamicCamera::MouseMove(void)
{
	int		iDistance = 0;

	if(iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.f));
	
		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

}

