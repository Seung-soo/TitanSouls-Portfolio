#include "stdafx.h"
#include "..\public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_Field.h"
#include "Level_IceBrain.h"
#include "Level_Slime.h"
#include "Level_Colossus.h"
#include "Level_EyeCube.h"
#include "Level_Yeti.h"
#include "Level_Main.h"
#include "Level_Respawn1.h"
#include "Level_Respawn2.h"
#include "Level_EndingCredit.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}
#include "Terrain.h"
// 로딩 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Loading::NativeConstruct");
		return E_FAIL;
	}



	m_eNextLevel = eNextLevel;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned the E_FAIL in CLevel_Loading::NativeConstruct");
		return E_FAIL;
	}

	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);

	return S_OK;
}

// 로딩 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_Loading::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Loading::Tick");
		return -1;
	}

	return 0;
}

_int CLevel_Loading::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Loading::LateTick");
		return -1;
	}

	if (true == m_pLoader->isFinished())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		//Safe_AddRef(pGameInstance);

		CLevel*		pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_MAIN:
			g_eCurrentLevel = LEVEL_MAIN;
			pLevel = CLevel_Main::Create(m_pGraphic_Device);
			break;
		case LEVEL_FIELD:
			g_eCurrentLevel = LEVEL_FIELD;
			pLevel = CLevel_Field::Create(m_pGraphic_Device);
			break;
		case LEVEL_SLIME:
			g_eCurrentLevel = LEVEL_SLIME;
			pLevel = CLevel_Slime::Create(m_pGraphic_Device);
			break;
		case LEVEL_ICEBRAIN:
			g_eCurrentLevel = LEVEL_ICEBRAIN;
			pLevel = CLevel_IceBrain::Create(m_pGraphic_Device);
			break;
		case LEVEL_COLOSUSS:
			g_eCurrentLevel = LEVEL_COLOSUSS;
			pLevel = CLevel_Colossus::Create(m_pGraphic_Device);
			break;
		case LEVEL_EYECUBE:
			g_eCurrentLevel = LEVEL_EYECUBE;
			pLevel = CLevel_EyeCube::Create(m_pGraphic_Device);
			break;
		case LEVEL_YETI:
			g_eCurrentLevel = LEVEL_YETI;
			pLevel = CLevel_Yeti::Create(m_pGraphic_Device);
			break;
		case LEVEL_RESPAWN1:
			g_eCurrentLevel = LEVEL_RESPAWN1;
			pLevel = CLevel_Respawn1::Create(m_pGraphic_Device);
			break;
		case LEVEL_RESPAWN2:
			g_eCurrentLevel = LEVEL_RESPAWN2;
			pLevel = CLevel_Respawn2::Create(m_pGraphic_Device);
			break;
		case LEVEL_CREDIT:
			g_eCurrentLevel = LEVEL_CREDIT;
			pLevel = CLevel_EndingCredit::Create(m_pGraphic_Device);
			break;
		}
		
		if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, pLevel)))
		{
			MSGBOX("pGameInstance::OpenLevel returned E_FAIL in CLevel_Loading::LateTick");
			return E_FAIL;
		}
		
		Safe_Release(pGameInstance);
	}

	return 0;
}

// 로딩 레벨에서 그려야하는 것들
HRESULT CLevel_Loading::Render()
{
	if (0 > __super::Render())		// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Loading::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("로딩레벨임. "));

	return S_OK;
}


HRESULT CLevel_Loading::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (false == g_bColossusClear)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject(Loading_Back)")))) {
			MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CLevel_Logo::Ready_Prototype_GameObject(Loading_Back)");
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject(Loading_Text)")))) {
			MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CLevel_Logo::Ready_Prototype_GameObject(Loading_Text)");
			return E_FAIL;
		}

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject(Loading_Icon)")))) {
			MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CLevel_Logo::Ready_Prototype_GameObject(Loading_Icon)");
			return E_FAIL;
		}


	}
	else
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, pLayerTag, TEXT("Prototype_GameObject_Effect_WhiteScreen")))) {
			MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CLevel_Logo::Ready_Prototype_GameObject(Loading_White)");
			return E_FAIL;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("CLevel_Loading::NativeConstruct returned the E_FAIL in CLevel_Loading::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
