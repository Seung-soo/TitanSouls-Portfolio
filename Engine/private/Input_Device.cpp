#include "..\public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;
	
	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->Acquire()))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyboard)
	{
		MSGBOX("m_pKeyboard is nullptr in CInput_Device::SetUp_InputDeviceState");
		return E_FAIL;
	}

	if (nullptr == m_pMouse)
	{
		MSGBOX("m_pMouse is nullptr in CInput_Device::SetUp_InputDeviceState");
		return E_FAIL;
	}

	m_pKeyboard->GetDeviceState(256, m_byKeyState);

	_ubyte byMouseState[4];
	memcpy(byMouseState, m_MouseState.rgbButtons, sizeof(_ubyte) * 4);
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	for (_uint i = 0; i < 4; i++)
	{
		if (byMouseState[i] != m_MouseState.rgbButtons[i])
		{
			if (byMouseState[i] & 0x80)
				m_bUpMouseState[i] = true;
		}
		else
			m_bUpMouseState[i] = false;
	}
	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInput);
}
