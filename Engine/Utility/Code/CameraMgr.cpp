#include "CameraMgr.h"
#include "Transform.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(Engine::CCameraMgr)

Engine::CCameraMgr::CCameraMgr(void)
:m_pDevice(NULL)
, m_vEye(0.f, 0.f, 0.f)
, m_vAt(0.f, 0.f, 0.f)
, m_vUp(0.f, 1.f, 0.f)
, m_vRayDir(0.f, 0.f, 0.f)
, m_vRayPos(0.f, 0.f, 0.f)
, m_fFovY(0.f)
, m_fAspect(0.f)
, m_fNear(0.f)
, m_fFar(0.f)
, m_fDistance(0.f)
, m_fHeight(0.f)
, m_fCameraSpeed(0.f)
, m_pTargetInfo(NULL)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

Engine::CCameraMgr::~CCameraMgr(void)
{

}
void Engine::CCameraMgr::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}
void Engine::CCameraMgr::SetViewMatrix()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCameraMgr::SetProMatrix(const WORD* WINCX, const WORD* WINCY)
{
	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(*WINCX) / float(*WINCY);
	m_fNear = 1.f;
	m_fFar  = 10000.f;

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Engine::CCameraMgr::SetTargetTransform(CTransform* pInfo)
{
	m_pTargetInfo = pInfo;

	m_fDistance = 500.f;
	m_fHeight = 100.f;
	m_fCameraSpeed = 10.f;

	m_vEye = m_pTargetInfo->m_vDir * (-1);
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

}

void Engine::CCameraMgr::FixMouse(const WORD* WINCX, const WORD* WINCY, HWND _hWnd)
{
	POINT	ptMouse = { *WINCX >> 1, *WINCY >> 1 };

	ClientToScreen(_hWnd, &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);
}

void Engine::CCameraMgr::MouseMove(void)
{
	int		iDistance = 0;

	if(iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
		m_pTargetInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(iDistance / 20.f);

	if(iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		if(iDistance < 0)
			m_fHeight -= 5.f;
		else if(iDistance > 0)
			m_fHeight += 5.f;
	}
	if(iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_Z))
	{
		if(iDistance > 0)
			m_fDistance -= m_fCameraSpeed;
		else if(iDistance < 0)
			m_fDistance += m_fCameraSpeed;
	}
}

D3DXVECTOR3* Engine::CCameraMgr::GetCameraAtPos(void)
{
	return &m_vAt;
}

void Engine::CCameraMgr::SetCameraTransForm(void)
{

	m_vAt = m_pTargetInfo->m_vPos;
	m_vEye = m_pTargetInfo->m_vDir * (-1);
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;
	m_vEye += m_pTargetInfo->m_vPos;
	m_vEye.y += m_fHeight;
	m_vAt.y += 80.f;

	m_vRayPos = m_vAt;
	m_vRayDir = m_vEye - m_vAt;

	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

D3DXVECTOR3* Engine::CCameraMgr::GetCameraEyePos(void)
{
	return &m_vEye;
}

FLOAT Engine::CCameraMgr::GetDistance(void)
{
	const D3DXVECTOR3	vLength = m_vEye -  m_pTargetInfo->m_vPos;
	
	FLOAT fDistance = D3DXVec3Length(&vLength);

	return fDistance;
}

D3DXVECTOR3* Engine::CCameraMgr::GetRayDir(void)
{
	return &m_vRayDir;
}

D3DXVECTOR3* Engine::CCameraMgr::GetRayPos(void)
{
	return &m_vRayPos;
}

void Engine::CCameraMgr::SetDistance(FLOAT fDist)
{
	m_fDistance = fDist - 10;
}

D3DXMATRIX* Engine::CCameraMgr::GetView(void)
{
	return &m_matView;
}

D3DXMATRIX* Engine::CCameraMgr::GetProj(void)
{
	return &m_matProj;
}

