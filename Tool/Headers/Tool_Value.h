#ifndef Value_h__
#define Value_h__

const int WINCX = 800;
const int WINCY = 600;

extern HWND g_hWnd;
extern HINSTANCE	g_hInst;

const D3DXVECTOR3		g_vLook = D3DXVECTOR3(0.f, 0.f, -1.f);

typedef struct tagInfo
{
	TCHAR*				szName;
	D3DXVECTOR3			vScale;
	D3DXVECTOR3			vPos;
	OBJTYPE				eObjType;
}INFO;

#endif // Value_h__
