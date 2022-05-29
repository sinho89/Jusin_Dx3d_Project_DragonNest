/*!
 * \file Engine_Functor.h
 * \date 2015/04/04 23:39
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
#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T> void operator () (T& pInstance)
		{			
			if(NULL != pInstance)
			{
				delete pInstance;
				pInstance = NULL;
			}
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void){}
		~CDeleteMap(void){}
	public: // operator	
		template <typename T> void operator () (T& Pair)
		{			
			if(NULL != Pair.second)
			{
				delete Pair.second;
				Pair.second = NULL;
			}
		}
	};

	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if(0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*				m_pTag;
	};
}

#endif // Engine_Functor_h__