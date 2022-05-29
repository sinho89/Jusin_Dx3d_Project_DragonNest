#ifndef Engine_Macro_h__
#define Engine_Macro_h__


#define SHADOW_MAP_SIZE	1024
#define SCREEN_WIDTH	1600
#define SCREEN_HEIGHT	900

#define MSG_BOX(_message)	MessageBox(NULL, _message, L"System Message", MB_OK)
#define TAGMSG_BOX(_tag,_message)	MessageBox(NULL, _message, _tag, MB_OK)

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}

#define NULL_CHECK_RETURN_TAGMSG( _ptr, _return, _tag, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, _tag, MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ MessageBox(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}


#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }


#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport) 
#else
#define ENGINE_DLL _declspec(dllimport) 
#endif


#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}


#define SAFE_NEWINIT(p, CLASSTYPE)									\
{																	\
	if(NULL == (p))													\
	{																\
	p = new CLASSTYPE;											\
	if(!(p))													\
	return -1;												\
	\
	if(FAILED((p)->Init()))										\
		{															\
		delete p;												\
		p = NULL;												\
		return -1;												\
		}															\
	}																\
}

#define SAFE_NEWCREATE1(p, CLASSTYPE, v0)							\
{																	\
	if(NULL == (p))													\
	{																\
	p = new CLASSTYPE;											\
	if(!(p))													\
	return -1;												\
	\
	if(FAILED((p)->Create(v0)))									\
		{															\
		delete p;												\
		p = NULL;												\
		return -1;												\
		}															\
	}																\
}

#define SAFE_RESTORE(p)												\
{																	\
	if(p && FAILED((p)->Restore() ))								\
	{																\
	return -1;													\
	}																\
}

#define SAFE_FRMOV(p)												\
{																	\
	if(p && FAILED((p)->FrameMove() ))								\
	{																\
	return -1;													\
	}																\
}

#define SAFE_INVALIDATE(p)		{	if(p)	(p)->Invalidate();		}
#define SAFE_RENDER(p)			{	if(p)	(p)->Render();			}

#endif // Engine_Macro_h__
