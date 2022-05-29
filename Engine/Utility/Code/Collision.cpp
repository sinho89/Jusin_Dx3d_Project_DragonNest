#include "Collision.h"

Engine::CCollision::CCollision(void)
: m_dwRefCnt(1)
{

}

Engine::CCollision::~CCollision(void)
{
	
}

void Engine::CCollision::AddRefCnt(void)
{
	++m_dwRefCnt;
}