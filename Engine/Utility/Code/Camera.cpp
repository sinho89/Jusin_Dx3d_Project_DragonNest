#include "Camera.h"

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_vEye(0.f, 0.f, 0.f)
, m_vAt(0.f, 0.f, 0.f)
, m_vUp(0.f, 1.f, 0.f)
, m_fFovY(0.f)
, m_fAspect(0.f)
, m_fNear(0.f)
, m_fFar(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

Engine::CCamera::~CCamera(void)
{

}

void Engine::CCamera::Update(void)
{
	Invalidate_ViewMatrix();
}

void Engine::CCamera::Invalidate_ViewMatrix(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::Invalidate_ProMatrix(void)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

