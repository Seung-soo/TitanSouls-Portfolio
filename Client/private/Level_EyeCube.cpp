#include "stdafx.h"
#include "..\public\Level_EyeCube.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"


CLevel_EyeCube::CLevel_EyeCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_EyeCube::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Closuss::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_EyeCube::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_EyeCube::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_EyeCube::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_EyeCube(TEXT("Layer_EyeCube"))))
	{
		MSGBOX("Ready_Layer_Slime returned E_FAIL in CLevel_EyeCube::NativeConstruct")
			return E_FAIL;
	}
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_EyeCube::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_EyeCube::NativeConstruct");
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
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 5.f));
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(_float3(16.f, 0.5f, 7.f));
	pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_EyeCube::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_EyeCube::Tick");
		return -1;
	}
	
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN2))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_EyeCube::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_EyeCube::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_EyeCube::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_EyeCube::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("아이큐브레벨임. "));


	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Terrain_EyeCube01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 30.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wall2_EyeCube01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(30.f, 0.f, 2.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wall1_EyeCube01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(2.f, 0.f, 2.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wall2_EyeCube01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(30.f, 0.f, 30.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wall2_EyeCube01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Colossus::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_EyeCube(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	//m_ColoInfo.bRightAttacking == true;
	//아이큐브 본체 생성
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_EyeCube"),&m_bDead)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Ready_Layer_EyeCube(Boss)");
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

HRESULT CLevel_EyeCube::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LEVEL	eCurrentLv = LEVEL_EYECUBE;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_UIName_Empty"), &eCurrentLv))) {
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;


	//Terrainex
	{
		tPosAngle.vPos = _float3(16.f, -2.f, 16.f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Terrain2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(16.f, 22.f, 16.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Terrain2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 1");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(15.f, 0.f, -16.5f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Terrain3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 1");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(15.f, 8.f, -14.5f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Terrain3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 1");
			return E_FAIL;
		}
	}


	//Wallex
	{
		tPosAngle.vPos = _float3(16.f, 6.f, 48.f);
		tPosAngle.fAngle = 0.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(48.f, 6.f, 16.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(16.f, 6.f, -16.f);
		tPosAngle.fAngle = 180.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(-16.f, 6.f, 16.f);
		tPosAngle.fAngle = 270.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 5");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(10.7f, 16.f, -14.5f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 5");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(19.f, 16.f, -14.5f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 5");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(11.2f, 16.f, -14.5f);
		tPosAngle.fAngle = 270.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 5");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(19.5f, 16.f, -14.5f);
		tPosAngle.fAngle = 270.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Wallex2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Terrain2_EyeCube01 5");
			return E_FAIL;
		}


	}


	//Chess

	{
		tPosAngle.vPos = _float3(-8.f, 8.f, -8.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess1_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(-8.f, 8.f, 32.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess1_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(32.f, 8.f, 44.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess1_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(44.f, 8.f, 24.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess1_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(40.f, 8.f, -8.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(18.f, 8.f, 40.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess2_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(-8.f, 8.f, 14.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess4_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(40.f, 8.f, 18.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess4_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(26.f, 8.f, -8.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(4.f, 8.f, -8.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(40.f, 8.f, 40.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(8.f, 8.f, 40.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_Chess3_EyeCube01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Prototype_GameObject_Chess1_EyeCube01 4");
			return E_FAIL;
		}
		//0308추가
		tPosAngle.vPos = _float3(16.f, 0.0f, -5.0f);
		tPosAngle.fAngle = 180.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_EyeCube::Ready_Layer_Accessories");
			return E_FAIL;
		}

	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_EyeCube::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(19.f, 0.f, 2.5f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 8.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Door");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_EyeCube * CLevel_EyeCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_EyeCube*	pInstance = new CLevel_EyeCube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_EyeCube::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_EyeCube::Free()
{
	__super::Free();
}
