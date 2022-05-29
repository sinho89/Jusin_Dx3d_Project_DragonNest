#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
public:
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float		m_fTime;

public:
	float GetTime(void);

public:
	void InitTime(void);
	void SetTime(void);

private:
	CTimeMgr(void);

public:
	~CTimeMgr(void);
};

END