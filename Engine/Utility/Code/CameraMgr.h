/*!
 * \file CameraMgr.h
 * \date 2016/10/06 16:17
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CameraMgr_h__
#define CameraMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CTransform;
class ENGINE_DLL CCameraMgr
{
public:
	DECLARE_SINGLETON(CCameraMgr)
private:
	CCameraMgr(void);
	~CCameraMgr(void);

public:
	void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	void SetTargetTransform(CTransform* pInfo);

public:
	void SetViewMatrix();
	void SetProMatrix(const WORD* WINCX, const WORD* WINCY);

public:
	void			FixMouse(const WORD* WINCX, const WORD* WINCY, HWND _hWnd);
	void			MouseMove(void);

public:
	D3DXVECTOR3*	GetCameraAtPos(void);
	D3DXVECTOR3*	GetCameraEyePos(void);
	FLOAT			GetDistance(void);
	void			SetDistance(FLOAT fDist);
	D3DXMATRIX*		GetView(void);
	D3DXMATRIX*		GetProj(void);

	D3DXVECTOR3*	GetRayDir(void);
	D3DXVECTOR3*	GetRayPos(void);

public:
	void			SetCameraTransForm(void);
private:
	D3DXMATRIX		m_matView;
	D3DXVECTOR3		m_vEye, m_vAt, m_vUp;

	D3DXMATRIX		m_matProj;
	float			m_fFovY, m_fAspect, m_fNear, m_fFar;

	D3DXVECTOR3		m_vRayDir, m_vRayPos;

private:
	float			m_fDistance;
	float			m_fHeight;
	float			m_fCameraSpeed;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CTransform*				m_pTargetInfo;

};
END


#endif // CameraMgr_h__