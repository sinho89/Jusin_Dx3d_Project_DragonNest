/*!
 * \file DynamicCamera.h
 * \date 2016/09/05 13:34
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CDynamicCamera(void);

public:
	HRESULT		InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	void		KeyCheck(void);

public:
	virtual void Update(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, 
								const D3DXVECTOR3* pEye, 
								const D3DXVECTOR3* pAt);
private:
	float			m_fCameraSpeed;
	bool			m_bClick;
	bool			m_bMouseFix;

private:
	void			FixMouse(void);
	void			MouseMove(void);

};


#endif // DynamicCamera_h__