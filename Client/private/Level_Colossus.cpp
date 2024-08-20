#include "stdafx.h"
#include "..\public\Level_Colossus.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"


CLevel_Colossus::CLevel_Colossus(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Colossus::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Closuss::NativeConstruct");
		return E_FAIL;
	}

	//if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	//{
	//	MSGBOX("Ready_Layer_Camera returned E_FAIL in CLevel_Slime::NativeConstruct");
	//	return E_FAIL;
	//}

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

	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//{
	//	MSGBOX("Ready_Layer_Player returned E_FAIL in CLevel_Slime::NativeConstruct");
	//	return E_FAIL;
	//}

	if (FAILED(Ready_Layer_Colossus(TEXT("Layer_Colossus"))))
	{
		MSGBOX("Ready_Layer_Slime returned E_FAIL in CLevel_Colossus::NativeConstruct")
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Colossus::NativeConstruct");
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
_int CLevel_Colossus::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Colossus::Tick");
		return -1;
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_CREDIT))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}
	

	return 0;
}

_int CLevel_Colossus::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Colossus::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_Colossus::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Colossus::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("콜로서스레벨임. "));


	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Terrain_Colossus01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Wall1_Colossus01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Wall2_Colossus01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 0.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Wall1_Colossus01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Wall1_Colossus01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_Colossus(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_ColoInfo.bStandUp = false;
	//m_ColoInfo.bRightAttacking == true;
	//콜로서스 본체 생성
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus"),&m_ColoInfo)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Colossus(Boss)");
		return E_FAIL;
	}
	// 콜로서스 왼손 생성
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Clossus_Fist_Left"), TEXT("Prototype_GameObject_Colossus_Fist"), &m_ColoInfo)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Colossus(Boss)");
		return E_FAIL;
	}
	// 콜로서스 오른손 생성
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Clossus_Fist_Right"), TEXT("Prototype_GameObject_Colossus_Fist_Right"), &m_ColoInfo)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Colossus(Boss)");
		return E_FAIL;
	}

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Slime_Heart"))))
	//{
	//	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Slime(Heart)");
	//	return E_FAIL;
	//}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LEVEL	eCurrentLv = LEVEL_COLOSUSS;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_UIName_Empty"), &eCurrentLv))) {
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Colossus::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	//terrain , wall
	{
		tPosAngle.vPos = _float3(0.f, 32.f, 0.f);
		tPosAngle.fAngle = 180.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_CeilingEx_Colossus01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(0.f, 32.f, 32.f);
		tPosAngle.fAngle = 180.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_CeilingEx2_Colossus01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(0.f, 0.f, -32.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_TerrainEx_Colossus01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(32.f, 0.f, -25.f);
		tPosAngle.fAngle = 180.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_WallEX_Colossus01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(0.f, 32.f, -30.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Dark01_Colossus01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 1");
			return E_FAIL;
		}
	}

	//Grass
	{
		tPosAngle.vPos = _float3(2.f, 0.3f, 29.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(3.f, 0.3f, 28.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(2.5f, 0.3f, 27.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(28.f, 0.3f, 25.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(29.f, 0.3f, 25.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(29.5f, 0.3f, 24.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(12.f, 0.3f, 16.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(13.5f, 0.3f, 17.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(12.5f, 0.3f, 16.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(13.f, 0.3f, 17.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(8.5f, 0.3f, 14.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(9.f, 0.3f, 15.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(21.f, 0.3f, 13.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(21.5f, 0.3f, 14.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(20.5f, 0.3f, 14.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(20.f, 0.3f, 13.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(6.5f, 0.3f, 10.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(5.5f, 0.3f, 10.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(5.f, 0.3f, 9.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(27.5f, 0.3f, 3.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(28.5f, 0.3f, 3.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(28.f, 0.3f, 4.5f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_Colossus01_Grass_Down"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Colossus * CLevel_Colossus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Colossus*	pInstance = new CLevel_Colossus(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_Colossus::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Colossus::Free()
{
	__super::Free();
}
