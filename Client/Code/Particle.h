/*!
 * \file Particle.h
 * \date 2016/10/28 22:04
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

#ifndef Particle_h__
#define Particle_h__

#include "GameObject.h"
#include "Include.h"

class CParticle : public Engine::CGameObject
{
protected:
	FLOAT						m_fTimeAvg;		// Frame ��� �ð�
	INT							m_PrtN;			// ������ ��
	Tpart*						m_PrtD;			// Particle Data
	D3DXVECTOR3					m_vcWind;		// �ٶ� ����

	// ȭ�� ��¿�
	INT							m_iVtx;			// Vertex Number
	VtxDUV1*				m_pVtx;			// Vertex
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DTEXTURE9			m_pTx;
	BOOL						m_bAni;

	DWORD				m_dTimeCur;
	DWORD				m_dTimeInt;	// TimeInterval

	float				m_fBrightness;
	bool				m_bBrightCheck;
	float				m_fMagmaY;

	float				m_fTime;

	Engine::CComponent* m_pTargetInfo;
	D3DXVECTOR3			m_vTargetPos;

	ID3DXMesh*			m_pMsh;
	D3DXMATRIX			m_mtMsh;

	D3DLIGHT9			m_d3Lgt;
	D3DMATERIAL9		m_d3Mtl;

protected:
	explicit CParticle(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CParticle();

public:
	virtual void Update(void);
	virtual void Render(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ
	virtual void	SetAvgTime(FLOAT fTime);	// ��� �ð� ����
	//virtual int		SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};

#endif // Particle_h__