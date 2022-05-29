/*!
 * \file AniCtrl.h
 * \date 2016/09/21 13:33
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

#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"

BEGIN(Engine)



class CAniCtrl
{
public:
	CAniCtrl(void);
	CAniCtrl(const CAniCtrl& rhs);
	~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER* GetAniCtrl(void);
	DOUBLE GetTrackPos(void);
	DOUBLE GetPeriod(void);

public:
	void SetTrackPos(DOUBLE dPostion);
	void SetAnimationSet(UINT iAniIdx, FLOAT fWeight, DOUBLE dDuration);
	void FrameMove(const float& fTimePerSec);

private:
	void Release(void);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;

private:
	UINT			m_iMaxAniSet;
	UINT			m_iCurrentTrack;
	UINT			m_iNewTrack;
	DOUBLE			m_dAccTime;
	DOUBLE			m_dPeriod;
};

END


#endif // AniCtrl_h__