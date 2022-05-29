/*!
 * \file Line2D.h
 * \date 2016/09/13 13:57
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

#ifndef Line2D_h__
#define Line2D_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLine2D
{
private:
	CLine2D(void);

public:
	~CLine2D(void);

public:
	HRESULT Init_Line(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);
	bool Check_LinePass(const D3DXVECTOR2* pMoveEndPoint);

public:
	static CLine2D* Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);

private:
	D3DXVECTOR2		m_vStartPoint;
	D3DXVECTOR2		m_vEndPoint;
	D3DXVECTOR2		m_vNormal;
};

END

#endif // Line2D_h__