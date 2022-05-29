#include "Input.h"

IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
: m_pInput(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
{
	ZeroMemory(m_byKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput::~CInput(void)
{
	Release();
}
void Engine::CInput::Release(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);
}

BYTE Engine::CInput::GetDIKeyState(BYTE KeyFlag)
{
	return m_byKeyState[KeyFlag];
}

BYTE Engine::CInput::GetDIMouseState(BYTE KeyFlag)
{
	return m_MouseState.rgbButtons[KeyFlag];
}

long Engine::CInput::GetDIMouseMove(MOUSEMOVE KeyFlag)
{
	return *(((long*)&m_MouseState) + KeyFlag);
}

void Engine::CInput::SetInputState(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
}

HRESULT Engine::CInput::InitInputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = NULL;

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK_MSG(hr, L"입력장치 생성 실패");

	hr = InitKeyBoard(hWnd);
	FAILED_CHECK_MSG(hr, L"키보드 장치 생성 실패");

	hr = InitMouse(hWnd);
	FAILED_CHECK_MSG(hr, L"마우스 장치 생성 실패");


	return S_OK;
}

HRESULT Engine::CInput::InitKeyBoard(HWND hWnd)
{
	HRESULT	hr = NULL;

	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	FAILED_CHECK(hr);

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	FAILED_CHECK(hr);

	m_pKeyBoard->Acquire();

	return S_OK;
}

HRESULT Engine::CInput::InitMouse(HWND hWnd)
{
	HRESULT	hr = NULL;

	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	FAILED_CHECK(hr);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	FAILED_CHECK(hr);

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	FAILED_CHECK(hr);

	m_pMouse->Acquire();

	return S_OK;
}	


