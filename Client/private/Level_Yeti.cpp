#include "stdafx.h"
#include "..\public\Level_Yeti.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"


CLevel_Yeti::CLevel_Yeti(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Yeti::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Yeti::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Yeti(TEXT("Layer_Yeti"))))
	{
		MSGBOX("Ready_Layer_Slime returned E_FAIL in CLevel_Yeti::NativeConstruct")
			return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_Yeti::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Yeti::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	SCENECHANGE tSceneChange;
	tSceneChange.bCrease = true;
	tSceneChange.iLevelIndex = LEVEL_END;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 3.f));
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(_float3(16.f, 0.5f, 5.f));
	pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_Yeti::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Yeti::Tick");
		return -1;
	}
	
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_COLOSUSS))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_Yeti::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Yeti::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_Yeti::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Yeti::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("예티레벨임. "));


	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Terrain_Yeti01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Wall1_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Wall2_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 0.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Wall2_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Wall2_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Yeti(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	//예티 본체 생성
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Yeti"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Yeti(Boss)");
		return E_FAIL;
	}
	


	


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LEVEL	eCurrentLv = LEVEL_YETI;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_UIName_Empty"), &eCurrentLv))) {
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	tPosAngle.vPos = _float3(-10.f, -1.f, -10.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ceiling01_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-10.f, 20.f, 36.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ceiling02_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(16.f, 0.2f, 16.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Terrain02_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(7.f, -1.f, -26.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Terrain3_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(22.f, 20.f, -23.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Terrain3_Yeti01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
		return E_FAIL;
	}

	//ICE

	{

		tPosAngle.vPos = _float3(22.f, 24.0f, 16.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ice03_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(8.f, 16.0f, 14.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ice03_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(11.f, 16.0f, 19.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ice03_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(24.f, 16.0f, 7.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ice03_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(19.f, 16.0f, 9.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_Ice03_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(31.f, 0.2f, 31.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_ice04_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(31.f, 0.2f, 1.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_ice04_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(1.f, 0.2f, 31.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_ice04_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(1.f, 0.2f, 1.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_ice04_Yeti01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

	}

	tPosAngle.vPos = _float3(24.f, 0.0f, -35.0f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Accessories");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Yeti::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(18.f, 0.f, 0.5f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 6.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Yeti::Ready_Layer_Door");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Yeti * CLevel_Yeti::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Yeti*	pInstance = new CLevel_Yeti(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_Yeti::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Yeti::Free()
{
	__super::Free();
}
