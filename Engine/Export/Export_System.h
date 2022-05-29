/*!
 * \file Export_System.h
 * \date 2015/04/02 19:19
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
#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "FontMgr.h"
#include "Input.h"

BEGIN(Engine)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr* Get_TimeMgr(void);
inline CFontMgr* Get_FontMgr(void);
inline CInput*	 Get_Input(void);

#include "Export_System.inl"

END

#endif // Export_System_h__