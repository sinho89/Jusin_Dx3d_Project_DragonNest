/*!
 * \file VIBuffer.h
 * \date 2015/04/04 22:28
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
#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer
	: public CResources
{
public:
	enum BUFFERTYPE {BUFFER_RCTEX, BUFFER_TERRAIN, BUFFER_CUBETEX, BUFFER_CUBECOL, BUFFER_TRAIL};

public:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);

public:
	virtual CResources* CloneResource(void) PURE;
	void GetVtxInfo(void* pVtxInfo);

public:
	void SetVtxInfo(void* pVtxInfo);
	void SetIdxInfo(void* pIdxInfo, const DWORD* pTriCnt);
	void Change_BoxColor(DWORD dwColor);

public:
	virtual HRESULT CreateBuffer(void);
	virtual HRESULT CreateBuffer(DWORD dwVertexCnt);
	virtual DWORD Release(void);

protected:
public:
	void Render(const D3DXMATRIX* pmatWorld);

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	DWORD		m_dwVtxSize;
	DWORD		m_dwVtxCnt;
	DWORD		m_dwVtxFVF;

	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD		m_dwIdxSize;
	DWORD		m_dwTriCnt;
	D3DFORMAT	m_IdxFmt;

};

END

#endif // VIBuffer_h__