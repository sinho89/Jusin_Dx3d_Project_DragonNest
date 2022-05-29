/*!
 * \file ObjMgr.h
 * \date 2016/10/17 22:34
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

#ifndef ObjMgr_h__
#define ObjMgr_h__


#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;
class CGameObject;
class ENGINE_DLL CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	typedef list<CGameObject*>		OBJLIST;
	typedef unordered_map<const TCHAR*, OBJLIST>	MAPOBJLIST;

	MAPOBJLIST						m_mapObjlist[LAYER_END];

public:
	enum DivisionType { DIV_SINGLE, DIV_MULTI, DIV_LOGO, DIV_END };

private:
	DivisionType	m_eDivType;
public:
	void			SetDivisionType(DivisionType _eType);
	DivisionType	GetDivisionType();
public:
	void	Update(void);
	void	Render(void);
	void	Release(void);

public:
	HRESULT			AddObject(const TCHAR* szObjKey, 
		CGameObject* pGameObject, 
		LAYERTYPE LayerType);

	const CComponent*		GetComponent(const TCHAR*	szObjKey,
		const TCHAR*	szComponentKey,
		LAYERTYPE LayerType);

	CGameObject*	GetGameObject(const TCHAR* szObjKey,
		LAYERTYPE LayerType);

	OBJLIST*		GetObjList(const TCHAR* szObjKey,
		LAYERTYPE LayerType);


private:
	explicit CObjMgr(void);
public:
	~CObjMgr(void);
};

END

#endif // ObjMgr_h__