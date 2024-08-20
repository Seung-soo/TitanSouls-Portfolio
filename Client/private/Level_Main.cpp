#include "stdafx.h"
#include "..\public\Level_Main.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Background.h"
CLevel_Main::CLevel_Main(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Main::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Main::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI_Background(TEXT("Layer_UI_Main_Background")))) {
		MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_Main::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI_Loby(TEXT("Layer_UI_Main_MenuLoby")))) {
		MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_Main::NativeConstruct");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->PlayBGM(L"MainSound.ogg", 0.8f);// 사운드

	RELEASE_INSTANCE(CGameInstance);


	/*

	if (FAILED(Ready_Layer_UI_Exit(TEXT("Layer_UI_Main_MenuExit")))) {
	MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_Main::NativeConstruct");
	return E_FAIL;
	}
	*/
	/*
	if (FAILED(Ready_Layer_UI_Option(TEXT("Layer_UI_Main_MenuOption")))) {
	MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_Main::NativeConstruct");
	return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI_Pause(TEXT("Layer_UI_Game_MenuPause")))) {
	return E_FAIL;
	}
	*/
	m_eCurrentMenu = MAIN_LOBY;
	return S_OK;
}

_int CLevel_Main::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Main::Tick");
		return -1;
	}

	//Move_Icon(m_eCurrentMenu);
	return 0;
}

_int CLevel_Main::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Main::LateTick");
		return -1;
	}

	/*

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if(m_eCurrentMenu == MAIN_LOBY){
	if(m_eBtnState ==MENU_START){
	if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_FIELD))))
	return E_FAIL;
	}
	}

	RELEASE_INSTANCE(CGameInstance);
	}
	*/
	//
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
		if (g_bStartgame == true) {

			if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_FIELD))))
				return E_FAIL;

		}

		RELEASE_INSTANCE(CGameInstance);
	}
	return 0;
}
void CLevel_Main::Move_Icon(BOARD eCurrentMenu)
{
	if (GetAsyncKeyState('W') & 1) {
		++m_iInputBtn;
	}
	if (GetAsyncKeyState('S') & 1) {
		--m_iInputBtn;
	}

	if (MAIN_LOBY == eCurrentMenu) {
		switch (m_iInputBtn)
		{
		case 0:
			m_fInterval = 100.f;
			m_eBtnState = MENU_EXIT;
			break;
		case 1:
			m_fInterval = 50.f;
			m_eBtnState = MENU_OPTION;
			break;
		case 2:
			m_fInterval = 0.f;
			m_eBtnState = MENU_START;
			break;
		}
	}
	if (MAIN_QUIT == eCurrentMenu) {
		switch (m_iInputBtn)
		{
		case 1:
			m_fInterval = 50.f;
			m_eBtnState = MENU_NO;
			break;
		case 2:
			m_fInterval = 0.f;
			m_eBtnState = MENU_YES;
			break;
		}
	}
}
HRESULT CLevel_Main::Render()
{
	if (0 > __super::Render())		// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Main::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("Main"));
	return S_OK;
}

HRESULT CLevel_Main::Select_MenuBoard(void)
{
	return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_UI_Background(const _tchar * pLayerTag)
{
	// 메인 씬 백그라운드
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UIBackground")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_UI_Loby(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/*

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_LobyBoard"), &m_eCurrentMenu))) {
	MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
	return E_FAIL;
	}
	// 스타트
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_MainMenu_Start"), &m_eCurrentMenu))) {
	MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
	return E_FAIL;
	}

	// 옵션
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_MainMenu_Option"), &m_eCurrentMenu))) {
	MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
	return E_FAIL;
	}

	// 종료
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_MainMenu_Quit"), &m_eCurrentMenu))) {
	MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
	return E_FAIL;
	}
	// 선택 아이콘
	*/
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_MainMenu_Select"), &m_bInputStart))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_Main::Ready_Layer_UI_Option(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_OptionBoard")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_MusicVolText")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_SFXVolText")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_ScreenText")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_ScreenTypeText")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}

	// 선택 아이콘


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_UI_Exit(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	BOARD eCurrentBoard = MAIN_QUIT;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_ExitBoard"), &m_eCurrentMenu))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	// 네
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_ExitMenu_Yes"), &m_eCurrentMenu))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}

	// 아니요
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_ExitMenu_No"), &m_eCurrentMenu))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	// 선택 아이콘

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_MainMenu_Select"), &m_eCurrentMenu))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Main::Ready_Layer_UI_Pause(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_PauseBoard")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_BackIcon")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_OptionIcon")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_MAIN, pLayerTag, TEXT("Prototype_GameObject_UI_QuitIcon")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_Main::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Main::SetUpBoard(BOARD eCurrentMenu, _tchar * pEraseBoardTag)
{

	return S_OK;
}

CLevel_Main * CLevel_Main::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Main*	pInstance = new CLevel_Main(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CLevel_Main::NativeConstruct returned the E_FAIL in CLevel_Main::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Main::Free()
{
	__super::Free();
}