#include "Resources.h"

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_dwRefCnt(1)
{

}

Engine::CResources::CResources(const CResources& rhs)
: m_pDevice(rhs.m_pDevice)
, m_dwRefCnt(rhs.m_dwRefCnt)
{

}

Engine::CResources::~CResources(void)
{

}

void Engine::CResources::AddRefCnt(void)
{
	++m_dwRefCnt;
}
