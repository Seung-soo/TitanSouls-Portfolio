#include "stdafx.h"
#include "..\public\Level_Respawn2.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"
#include "Torch.h"

CLevel_Respawn2::CLevel_Respawn2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Respawn2::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Respawn2::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_Respawn2::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_Respawn2::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Respawn2::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn2::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_OpenDoor(TEXT("Layer_OpenDoor"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}


	SCENECHANGE tSceneChange;
	tSceneChange.bCrease = true;
	tSceneChange.iLevelIndex = LEVEL_END;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
	if (true == g_bYetiClear)
		static_cast<CTorch*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Torch1")))->Set_TorchFire(true);
	if (true == g_bIceBrainClear)
		static_cast<CTorch*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Torch2")))->Set_TorchFire(true);
	pGameInstance->PlayBGM(L"Respawn2.ogg", 0.3f);// 사운드
	RELEASE_INSTANCE(CGameInstance);

	g_iKill = 0;
	return S_OK;
}

_int CLevel_Respawn2::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Respawn2::Tick");
		return -1;
	}

	if (g_iKill >= 3)
		g_bRespawn2Clear = true;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_ICEBRAIN))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_Respawn2::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Respawn2::LateTick");
		return -1;
	}

	return 0;
}

HRESULT CLevel_Respawn2::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Respawn2::Render");
		return E_FAIL;
	}


	SetWindowText(g_hWnd, TEXT("리스폰2 레벨임. "));

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Terrain_Respawn2"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Terrain");
		return E_FAIL;
	}

	if (false == g_bRespawn2Clear)
	{
		for (int i = 0; i < 3; i++) {
			_float3 pos;
			switch (i)
			{
			case 0:
				pos.x = 30;
				pos.y = 1;
				pos.z = 25;
				break;
			case 1:
				pos.x = 35;
				pos.y = 1;
				pos.z = 30;
				break;
			case 2:
				pos.x = 40;
				pos.y = 1;
				pos.z = 32;
				break;
			}
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, TEXT("Layer_Archor"), TEXT("Prototype_GameObject_Monster_Archor"), &pos)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Terrain(Monster)");
				return E_FAIL;
			}
		}
	}
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	
	//Ceiling
	tPosAngle.vPos = _float3(0.f, 20.f, 64.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ceiling01_Respawn2"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}



	//Wall
	tPosAngle.vPos = _float3(29.f, -2.f, 7.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(67.f, -2.f, 7.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(9.f, -2.f, 7.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(9.f, -2.f, 39.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(55.f, -2.f, 71.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(55.f, -2.f, 39.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-4.f, -2.f, 44.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(36.f, -2.f, 44.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}
	tPosAngle.vPos = _float3(24.f, -2.f, 63.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(36.f, -2.f, 76.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(28.f, -2.f, 44.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(35.f, -2.f, 7.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(29.f, -2.f, -25.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	//ICE1

	tPosAngle.vPos = _float3(16.f, 0.01f, 16.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ice1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(48.f, 0.01f, 16.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ice1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(16.f, 0.02f, 36.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ice1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(48.f, 0.02f, 36.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ice1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.03f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Ice1_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(52.f, 16.f, 44.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_WallA_Respawn02");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(12.f, 16.f, 44.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallB_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_WallB_Respawn02");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(3.f, 15.f, 72.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_CeilingYeti_Respawn2"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_CeilingYeti_Respawn2");
		return E_FAIL;
	}
	tPosAngle.vPos = _float3(2.f, 0.01f, 46.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_CeilingYeti_Respawn2"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_CeilingYeti_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.0f, -64.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn2"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 8.55f, 64.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn2"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 12.f, -16.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallC_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 20.f, 80.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallC_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 20.f, 96.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallC_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, -24.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallD_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 28.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_WallD_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Prototype_GameObject_Terrain2_Respawn2");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 16.0f, 70.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.0f, -10.0f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(51.f, 16.0f, 60.0f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_IceWall_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(43.f, 16.0f, 60.0f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_IceWall_Respawn02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(70.f, 16.0f, 50.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(26.5f, 6.0f, 43.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, TEXT("Layer_Torch1"), TEXT("Prototype_GameObject_Torch"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(37.3f, 6.0f, 43.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN2, TEXT("Layer_Torch2"), TEXT("Prototype_GameObject_Torch"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(3.f, 0.f, 43.5f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 20.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Door");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(35.f, 0.f, 43.5f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 20.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Door");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(28.f, 0.f, 62.5f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 30.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn2::Ready_Layer_Door");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn2::Ready_Layer_OpenDoor(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	//tPosAngle.vPos = _float3(28.5f, 6.f, 44.1f);
	//tPosAngle.fAngle = 0.f;
	//pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Door1_Field"), &tPosAngle);

	//tPosAngle.vPos = _float3(35.5f, 6.f, 44.1f);
	//tPosAngle.fAngle = 0.f;
	//pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_Door2_Field"), &tPosAngle);

	if (false == g_bIceBrainClear)
	{
		tPosAngle.vPos = _float3(47.2f, 4.f, 44.1f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_RESPAWN2, pLayerTag, TEXT("Prototype_GameObject_AllDoor"), &tPosAngle);
	}

	if (false == g_bYetiClear)
	{
		tPosAngle.vPos = _float3(17.2f, 5.f, 44.1f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_RESPAWN2, TEXT("Layer_OpenDoor2"), TEXT("Prototype_GameObject_AllDoor2"), &tPosAngle);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
//RECT Door ++


CLevel_Respawn2 * CLevel_Respawn2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Respawn2*	pInstance = new CLevel_Respawn2(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CLevel_Field::NativeConstruct returned the E_FAIL in CLevel_Respawn2::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Respawn2::Free()
{
	__super::Free();
}
