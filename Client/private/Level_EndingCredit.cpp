#include "stdafx.h"
#include "..\public\Level_EndingCredit.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_EndingCredit::CLevel_EndingCredit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_EndingCredit::NativeConstruct()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_Player"));
	pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_UI"));
	pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_Arrow"));

	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Main::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI_Background(TEXT("EndingBack")))) {
		MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_EndingCredit::NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI_Credit(TEXT("EndingCredit")))) {
		MSGBOX("Ready_Layer_UI Background returned E_FAIL in CLevel_EndingCredit::NativeConstruct");
		return E_FAIL;
	}
	pGameInstance->StopAll();
	pGameInstance->PlayBGM(L"Ending_Credit.ogg", 0.5f);// 사운드
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CLevel_EndingCredit::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Main::Tick");
		return -1;
	}
	return 0;
}

_int CLevel_EndingCredit::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Main::LateTick");
		return -1;
	}



	return 0;
}

HRESULT CLevel_EndingCredit::Render()
{
	if (0 > __super::Render())		// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Main::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("Ending"));
	return S_OK;
}

HRESULT CLevel_EndingCredit::Ready_Layer_UI_Background(const _tchar * pLayerTag)
{
	// 메인 씬 백그라운드
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_CREDIT, pLayerTag, TEXT("Prototype_GameObject_EndingBack")))) {
		MSGBOX("Failed to Add Prototype UI_Background in CLevel_EndingCredit::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_EndingCredit::Ready_Layer_UI_Credit(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_CREDIT, pLayerTag, TEXT("Prototype_GameObject_EndingCredit")))) {
		MSGBOX("Failed to Add Prototype UI_EndingBack in CLevel_EndingCredit::Ready_Prototype_GameObject");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}



CLevel_EndingCredit * CLevel_EndingCredit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_EndingCredit*		pInstance = new CLevel_EndingCredit(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct())) {
		MSGBOX("CLevel_Ending::NativeConstruct returned the E_FAIL in CLevel_Ending::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_EndingCredit::Free()
{
	__super::Free();
}
