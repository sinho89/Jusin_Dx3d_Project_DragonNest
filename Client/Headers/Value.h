#ifndef Value_h__
#define Value_h__

const WORD		WINCX = 1024;
const WORD		WINCY = 768;

const WORD		VTXCNTX = 129;
const WORD		VTXCNTZ = 129;
const WORD		VTXITV = 1;

const D3DXVECTOR3		g_vLook = D3DXVECTOR3(0.f, 0.f, -1.f);


typedef	D3DXVECTOR3						VEC3;
typedef D3DXMATRIX						MATA;

typedef LPDIRECT3DDEVICE9				PDEV;
typedef LPDIRECT3DTEXTURE9				PDTX;
typedef LPDIRECT3DCUBETEXTURE9			PDTC;
typedef LPDIRECT3DVERTEXDECLARATION9	PDVD;
typedef	LPD3DXEFFECT					PDEF;

typedef LPDIRECT3DTEXTURE9		PDTX;
typedef LPD3DXRenderToEnvMap	PDRE;
typedef LPDIRECT3DCUBETEXTURE9	PDTC;

struct VtxIdx
{
	WORD a, b, c;

	VtxIdx() : a(0), b(0), c(0){}
	VtxIdx(WORD A, WORD B, WORD C) : a(A), b(B), c(C){}
	enum {FVF = D3DFMT_INDEX16, };
};

struct Vtx
{
	VEC3	p;
	enum { FVF = (D3DFVF_XYZ),};
};

struct XFILE_FVF_INFO
{
	D3DXVECTOR3 p; // 포지션(x,y,z)
	D3DXVECTOR3 n; // 노멀벡터(x,y,z)
	D3DXVECTOR2 t; // 텍스쳐(u,y)
};

typedef struct tagParticle
{
	// 입자의 운동 요소
	D3DXVECTOR3		IntP;		// 초기 위치
	D3DXVECTOR3		IntV;		// 초기 속도
	D3DXVECTOR3		IntA;		// 초기 기가속도

	D3DXVECTOR3		CrnP;		// 현재 위치
	D3DXVECTOR3		CrnV;		// 현재 속도
	D3DXVECTOR3		CrnA;		// 현재 가속도

	D3DXVECTOR3		CrnR ;		// 현재 회전
	D3DXVECTOR3		CrnRv;		// 현재 회전 속도

	D3DXVECTOR3		CrnS ;		// 스케일

	FLOAT			fElst;		// 탄성(Elastic) 계수
	FLOAT			fDamp;		// 공기 저항(Air Registance) 계수

	// 입자의 생명 요소
	BOOL			bLive;		// Active (Yes/No)
	FLOAT			fLife;		// Particle fLife
	FLOAT			fFade;		// Fade Speed
	DWORD			dColor;		// Color


	// 입자의 표현 요소
	FLOAT			PrsW;		// Present Billboard W
	FLOAT			PrsH;		// Present Billboard H
	FLOAT			PrsZ;		// 카메라의 z축과 위치의 내적 값

	INT				m_PrsImg;	// Image Index

}PARTICLE;

typedef struct tagVertexParticle
{
	D3DXVECTOR3		vPos;
	DWORD			dwColor;
	FLOAT			fU, fV;

	tagVertexParticle() : vPos(0,0,0), fU(0), fV(0), dwColor(0xFFFFFFFF){}
	tagVertexParticle(FLOAT X, FLOAT Y, FLOAT Z
		, FLOAT U, FLOAT V
		, DWORD D=0xFFFFFFFF) : vPos(X,Y,Z), fU(U), fV(V), dwColor(D){}

	enum {	FVF= (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) };

}VTXPTC;

typedef struct tagVertexParticle2
{
	D3DXVECTOR3	p;
	DWORD		d;

	tagVertexParticle2() : p(0,0,0), d(0xFFFFFFFF){}
	tagVertexParticle2(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF): p(X,Y,Z), d(D){}
	enum { FVF =(D3DFVF_XYZ|D3DFVF_DIFFUSE) };
}VTXPTC2;

inline DWORD FtoDW( FLOAT f )	{ return *((DWORD*)&f); }

struct VtxDUV1
{
	D3DXVECTOR3	p;
	DWORD		d;
	FLOAT		u, v;

	VtxDUV1() : p(0,0,0), u(0), v(0), d(0xFFFFFFFF){}
	VtxDUV1(FLOAT X, FLOAT Y, FLOAT Z
		, FLOAT U, FLOAT V
		, DWORD D=0xFFFFFFFF) : p(X,Y,Z), u(U), v(V), d(D){}

	enum {	FVF= (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)	};

};


struct Tpart
{
	// 입자의 운동 요소
	D3DXVECTOR3		m_IntP;		// 초기 위치
	D3DXVECTOR3		m_IntV;		// 초기 속도
	D3DXVECTOR3		m_IntA;		// 초기 기가속도

	D3DXVECTOR3		m_IntRight;
	D3DXVECTOR3		m_IntUp;
	D3DXVECTOR3		m_IntLook;

	D3DXVECTOR3		m_CrnP;		// 현재 위치
	D3DXVECTOR3		m_CrnV;		// 현재 속도
	D3DXVECTOR3		m_CrnA;		// 현재 가속도

	D3DXVECTOR3		m_CrnRight;
	D3DXVECTOR3		m_CrnUp;
	D3DXVECTOR3		m_CrnLook;

	FLOAT			m_fElst;	// 탄성(Elastic) 계수
	FLOAT			m_fDamp;	// 공기 저항(Air Registance) 계수

	// 입자의 생명 요소
	BOOL			m_bLive;	// Active (Yes/No)
	FLOAT			m_fLife;	// Particle fLife
	FLOAT			m_fFade;	// Fade Speed
	DWORD			m_dColor;	// Color


	// 입자의 표현 요소
	FLOAT			m_PrsW;		// Present Billboard W
	FLOAT			m_PrsH;		// Present Billboard H
	FLOAT			m_PrsZ;		// 카메라의 z축과 위치의 내적 값

	INT				m_PrsImg;	// Image Index
};

static int SortFnc(const Tpart* p1, const Tpart* p2)
{
	FLOAT	v1, v2;

	v1 = p1->m_PrsZ;
	v2 = p2->m_PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1 == v2)
		return 0;

	else 
		return -1;
}


typedef D3DXVECTOR2						VEC2;
typedef	D3DXVECTOR3						VEC3;
typedef D3DXVECTOR4						VEC4;
typedef D3DXMATRIX						MATA;

typedef LPDIRECT3DDEVICE9				PDEV;

typedef LPDIRECT3DVERTEXSHADER9			PDVS;
typedef LPDIRECT3DPIXELSHADER9			PDPS;

typedef LPDIRECT3DVERTEXDECLARATION9	PDVD;
typedef LPD3DXCONSTANTTABLE				PDCT;
typedef LPD3DXEFFECT					PDEF;			// ID3DXEffect 객체

typedef LPDIRECT3DTEXTURE9				PDTX;

class CStringCmp
{
private:
	const TCHAR* m_pString;

public:
	explicit CStringCmp(const TCHAR* pKey)
		:m_pString(pKey){}
	~CStringCmp(){}

public:
	template<typename T>
	bool operator () (T data)
	{
		return !lstrcmp(data.first, m_pString);
	}

};
#endif // Value_h__
