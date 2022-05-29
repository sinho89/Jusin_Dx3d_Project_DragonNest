/*!
 * \file ShaderEx.h
 * \date 2016/10/28 12:52
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

#ifndef ShaderEx_h__
#define ShaderEx_h__

#include "Include.h"
class CShaderEx
{

public:
	PDEV		m_pDev;				// Device
	PDVD		m_pFVF;				// 정점선언
	PDEF		m_pEft;				// ID3DXEffect 객체

	VtxDUV1		m_pVtx[4];			// Vertex Buffer
	PDTX		m_pTexS;			// Texture for Scene
	PDTX		m_pTexD;			// Texture for Distortion



public:
	CShaderEx();
	virtual ~CShaderEx();

	INT		Create(PDEV pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

	void	SetSceneTexture(PDTX pTx);
	void	SetDistortionTexture(PDTX pTx);

	INT		Restore();
	void	Invalidate();
};


#endif // ShaderEx_h__