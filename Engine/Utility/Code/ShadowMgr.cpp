#include "ShadowMgr.h"
#include "Export_Function.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(Engine::CShadowMgr)

Engine::CShadowMgr::CShadowMgr(void)
:m_pDevice(NULL)
{

}

Engine::CShadowMgr::~CShadowMgr(void)
{

}

void Engine::CShadowMgr::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	Engine::CComponent*		pComponent = NULL;

	pComponent = Engine::Get_ShaderMgr()->Clone_Shader(L"Shader_Shadow");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	m_pEffect = m_pShader->Get_EffectHandle();

}

void Engine::CShadowMgr::Create_Render_Target(const int _iW, const int _iH)
{
	m_pDevice->CreateTexture(_iW, _iH, 1,  D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pRenderTargetTex, NULL );
	m_pDevice->CreateDepthStencilSurface( _iW, _iH, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pRenderTargetSurf, NULL );
}

void Engine::CShadowMgr::Release()
{
	Safe_Release(m_pRenderTargetTex);
	Safe_Release(m_pRenderTargetSurf);
}

void Engine::CShadowMgr::Start_Render_Target()
{
	m_pDevice->GetRenderTarget( 0, &m_pOrigSurf );
	m_pDevice->GetDepthStencilSurface( &m_pOrigStencil );
	LPDIRECT3DSURFACE9 pTargetSurf;
	m_pRenderTargetTex->GetSurfaceLevel( 0, &pTargetSurf );
	m_pDevice->SetRenderTarget( 0, pTargetSurf );
	Safe_Release( pTargetSurf ); 
	m_pDevice->SetDepthStencilSurface( m_pRenderTargetSurf );

	m_pDevice->Clear( 0, NULL, (D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER), 0xffffffff, 1.f, 0 );
}

void Engine::CShadowMgr::End_Render_Target()
{
	m_pDevice->SetRenderTarget( 0, m_pOrigSurf );
	m_pDevice->SetDepthStencilSurface( m_pOrigStencil );
	Safe_Release( m_pOrigSurf ); 
	Safe_Release( m_pOrigStencil );
}

void Engine::CShadowMgr::SetCamera()
{
	D3DXVECTOR3	vDir = (*Get_CameraMgr()->GetCameraAtPos()) - (*Get_CameraMgr()->GetCameraEyePos());
	D3DXVec3Normalize(&vDir, &vDir);

	m_vCamDir = vDir;
	m_vLightPos = *Get_CameraMgr()->GetCameraEyePos();
	m_vLightDir = vDir;

	m_vLightAt = m_vCamPos;
	m_vLightPos = m_vLightAt;
	m_vLightPos.x += 500.f * m_vLightDir.x;
	m_vLightPos.z += 500.f * m_vLightDir.z;
	m_vLightPos.y += 500.f;

	D3DXMatrixOrthoLH( &m_matLightProj, 500, 500, 1.f, 10000.f);
	D3DXMatrixLookAtLH( &m_matLightView, &m_vLightPos, &m_vLightAt, &D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pEffect->SetMatrix("_matLightView", &m_matLightView);
	m_pEffect->SetMatrix("_matLightPro", &m_matLightProj);
}
