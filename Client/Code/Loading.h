/*!
 * \file Loading.h
 * \date 2016/09/02 14:44
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

#ifndef Loading_h__
#define Loading_h__

#include <process.h>

class CLoading
{
public:
	enum LOADID { LOADID_STAGE, LOADID_STAGE1, LOADID_STAGE2 };

private:
	CLoading(LOADID eLoadID);

public:
	CLoading(void);

public:
	const TCHAR*		GetLoadingMessage(void);
	bool				GetComplete(void);

public:
	HRESULT		InitLoading(void);
	void		StageLoading1(void);
	void		StageLoading2(void);
	void		StageLoading3(void);

public:
	static CLoading* Create(LOADID eLoadID);
	static UINT __stdcall LoadingFunction(void* pArg);

private:
	void	Release(void);
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LOADID					m_eLoadID;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR					m_szLoadingMessage[128];
	bool					m_bComplete;	
};


#endif // Loading_h__