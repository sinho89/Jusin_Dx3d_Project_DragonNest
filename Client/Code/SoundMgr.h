
#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Include.h"


#pragma comment (lib, "fmodex_vc.lib")



class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

	//////////fmod �������̽�
private:
	FMOD_SYSTEM*		m_pSystem;		//fmod �ý��� ������
	FMOD_CHANNEL*		m_pEffect;		//ȿ���� 
	FMOD_CHANNEL*		m_pMainBGM;		//����� 
	FMOD_RESULT			m_Result;		//fmod�����߿� ����üũ
	FMOD_CHANNEL*		m_pPlayerAttack;		//�÷��̾� ����
	FMOD_CHANNEL*		m_pPlayerMove;		//�÷��̾� �̵�
	FMOD_CHANNEL*		m_pPlayerHarf;		
	FMOD_CHANNEL*		m_pPlayerCresent;		
	FMOD_CHANNEL*		m_pPlayerGreat;			
	FMOD_CHANNEL*		m_pGreatWind;			
	FMOD_CHANNEL*		m_pGoblinAttack;		
	FMOD_CHANNEL*		m_pHit;		
	FMOD_CHANNEL*		m_pKnockBack;	
	FMOD_CHANNEL*		m_pDown;	
	FMOD_CHANNEL*		m_pBite;	
	FMOD_CHANNEL*		m_pBash;		
	FMOD_CHANNEL*		m_pBashAttack;	
	FMOD_CHANNEL*		m_pFireStart;	
	FMOD_CHANNEL*		m_pFireLoop;	
	FMOD_CHANNEL*		m_pFire;	
	FMOD_CHANNEL*		m_pBossApear;		

	unsigned int		m_iVersion;

	map<TCHAR*, FMOD_SOUND*>		m_mapSound;

	//��� �Լ��� ��������.
public:
	void Initialize(void);
	void LoadSoundFile(void);
	void PlaySound(TCHAR* pSoundKey);
	void PlayBGMSound(TCHAR* pSoundKey);
	void SoundAllStop(void);

public:
	void PlayerAttackSound(TCHAR* pSoundKey);
	void PlayerMoveSound(TCHAR* pSoundKey);
	void PlayerHarfSound(TCHAR* pSoundKey);
	void PlayerCresentSound(TCHAR* pSoundKey);
	void PlayerGreatSound(TCHAR* pSoundKey);
	void PlayerHitSound(TCHAR* pSoundKey);
	void PlayerBiteSound(TCHAR* pSoundKey);
	void PlayerBashSound(TCHAR* pSoundKey);
	void PlayerBashAttackSound(TCHAR* pSoundKey);
	void PlayerFireStartSound(TCHAR* pSoundKey);
	void PlayerFireLoopSound(TCHAR* pSoundKey);
	void PlayerFireSound(TCHAR* pSoundKey);
	void PlayerBossApearSound(TCHAR* pSoundKey);
	void PlayerKnockbackSound(TCHAR* pSoundKey);

	void MonsterDownSound(TCHAR* pSoundKey);

	void GreatWindSound(TCHAR* pSoundKey);

	void GoblinAttackSound(TCHAR* pSoundKey);

public:
	map<TCHAR*, FMOD_SOUND*>* GetSoundMap(void)
	{
		return &m_mapSound;
	}

private:
	void ErrorCheck(FMOD_RESULT _result);


private:
	CSoundMgr(void);
public:
	~CSoundMgr(void);
};

#endif // SoundMgr_h__
