/*!
 * \file AttPart.h
 * \date 2016/11/03 15:47
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

#ifndef AttPart_h__
#define AttPart_h__


#include "Particle.h"

namespace Engine
{
	class CResourceMgr;
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CAttPart : public CParticle
{
protected:
	explicit CAttPart(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CAttPart();

public:
	static CAttPart* Create(LPDIRECT3DDEVICE9 pDevice);
	static CAttPart* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _vPos);
public:
	virtual void Update(void);
	virtual void Render(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// 애니메이션 활성화/ 비활성화
	virtual void	SetAvgTime(FLOAT fTime);	// 평균 시간 설정
	//virtual int		SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	HRESULT SetConstantTable(void);
private:
	HRESULT AddComponent(void);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
	void SetInfo(D3DXVECTOR3 _vPos);


private:
	float		m_fAlpha;
};

#endif // AttPart_h__