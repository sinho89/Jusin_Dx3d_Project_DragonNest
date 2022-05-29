/*!
 * \file Component.h
 * \date 2015/04/03 18:15
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
#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected:
	explicit CComponent(void);

public:
	virtual ~CComponent(void);

public:
	virtual void Update(void) {}
	virtual DWORD Release(void) PURE;
};

END

#endif // Component_h__