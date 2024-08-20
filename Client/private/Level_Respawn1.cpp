#include "stdafx.h"
#include "..\public\Level_Respawn1.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"
#include "Torch.h"

CLevel_Respawn1::CLevel_Respawn1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Respawn1::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
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
	if (true == g_bSlimeClear)
		static_cast<CTorch*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Torch1")))->Set_TorchFire(true);
	if (true == g_bEyeCubeClear)
		static_cast<CTorch*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Torch2")))->Set_TorchFire(true);
	pGameInstance->PlayBGM(L"Respawn1.ogg", 0.3f);// 사운드
	RELEASE_INSTANCE(CGameInstance);
	g_iKill = 0;

	return S_OK;
}

_int CLevel_Respawn1::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Respawn1::Tick");
		return -1;
	}

	if (g_iKill >= 5)
		g_bRespawn1Clear = true;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_SLIME))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_Respawn1::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Respawn1::LateTick");
		return -1;
	}

	return 0;
}

HRESULT CLevel_Respawn1::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Respawn1::Render");
		return E_FAIL;
	}


	SetWindowText(g_hWnd, TEXT("리스폰1 레벨임. "));

	return S_OK;
}


HRESULT CLevel_Respawn1::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Terrain1_Respawn1"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Terrain");
		return E_FAIL;
	}

	if (false == g_bRespawn1Clear)
	{
		for (int i = 0; i < 5; i++) {
			_float3 pos;
			switch (i)
			{
			case 0:
				pos.x = 30;
				pos.y = 1;
				pos.z = 45;
				break;
			case 1:
				pos.x = 35;
				pos.y = 1;
				pos.z = 50;
				break;
			case 2:
				pos.x = 20;
				pos.y = 1;
				pos.z = 54;
				break;
			case 3:
				pos.x = 50;
				pos.y = 1;
				pos.z = 44;
				break;
			case 4:
				pos.x = 40;
				pos.y = 1;
				pos.z = 52;
				break;
			}
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_Short_Monster"), TEXT("Prototype_GameObject_Monster_Short_Range"), &pos)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Terrain(Monster)");
				return E_FAIL;
			}
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn1::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	tPosAngle.vPos = _float3(0.f, 12.f, 128.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 12.f, 64.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(60.f, 5.f, 64.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-51.7f, 5.f, 64.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-51.7f, 5.f, 0.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}


	////

	tPosAngle.vPos = _float3(5.f, -2.f, -13.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-19.f, -2.f, 19.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"),&tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(13.f, -2.f, 19.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"),&tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(13.f, -2.f, 51.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(13.f, -2.f, 56.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(45.f, -2.f, 56.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(58.f, -2.f, 56.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(58.f, -2.f, 24.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, -2.f, 92.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(58.f, -2.f, 10.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(44.f, -2.f, 10.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(12.f, -2.f, 10.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(16.f, -2.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Wall1_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Wall");
		return E_FAIL;
	}




	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Respawn1::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Respawn1::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	//terrain2

	tPosAngle.vPos = _float3(-3.f, 0.f, -64.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-17.f, 0.f, 64.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(64.f, 0.f, 72.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(-64.f, 0.f, 78.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Respawn1"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	// Wall ABCD

	tPosAngle.vPos = _float3(12.1f, 12.f, 55.9f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(60.f, 12.f, 50.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(12.2f, 21.f, 14.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 40.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(74.f, 0.f, 40.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_WallA_Respawn01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}




	//Grass1
	{
		tPosAngle.vPos = _float3(2.f, 0.1f, 2.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(14.f, 0.09f, 7.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(9.f, 0.09f, 20.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(13.f, 0.08f, 40.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(26.f, 0.09f, 55.f);
		tPosAngle.fAngle = 90.f;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(48.f, 0.09f, 53.f);
		tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(55.f, 0.1f, 46.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(56.f, 0.1f, 20.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(49.f, 0.09f, 12.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(56.f, 0.08f, 10.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(32.f, 0.09f, 13.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(25.f, 0.08f, 17.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(28.f, 0.08f, 42.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(38.f, 0.09f, 46.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}


tPosAngle.vPos = _float3(44.f, 0.08f, 24.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}

tPosAngle.vPos = _float3(38.f, 0.09f, 28.f);
tPosAngle.fAngle = 90.f;

if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Grass1_Respawn01"), &tPosAngle)))
{
	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
	return E_FAIL;
}
	}


	//Grass2

	tPosAngle.vPos = _float3(32.f, 11.f, 32.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(24.f, 11.1f, 24.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(48.f, 11.2f, 24.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(48.f, 11.3f, 52.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass02"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	//Grass3

	tPosAngle.vPos = _float3(56.f, 0.3f, 50.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(55.f, 0.3f, 51.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(55.5f, 0.3f, 52.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(54.f, 0.3f, 49.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(14.f, 0.3f, 21.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(15.f, 0.3f, 20.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(15.5f, 0.3f, 21.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(14.5f, 0.3f, 22.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(48.f, 0.3f, 21.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(49.f, 0.3f, 20.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(49.5f, 0.3f, 21.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(48.5f, 0.3f, 22.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(51.f, 0.3f, 15.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(52.f, 0.3f, 14.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(52.5f, 0.3f, 15.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(51.5f, 0.3f, 16.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(61.f, 0.3f, 42.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(30.f, 0.3f, 38.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(30.5f, 0.3f, 39.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(30.5f, 0.3f, 40.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}



	tPosAngle.vPos = _float3(16.f, 0.3f, 39.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(17.f, 0.3f, 41.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(17.5f, 0.3f, 42.5f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(16.5f, 0.3f, 43.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass3"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	//Grass4
	
	tPosAngle.vPos = _float3(32.5f, 10.0f, 32.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass04"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(13.5f, 12.0f, 21.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass04"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(45.5f, 11.0f, 48.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass04"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(15.5f, 13.0f, 54.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass04"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(14.5f, 13.0f, 55.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_Respawn01_Grass04"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	//0308추가


	tPosAngle.vPos = _float3(32.f, 0.0f, 70.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.0f, -14.0f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(65.f, 0.0f, 40.0f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}


	tPosAngle.vPos = _float3(0.1f, 0.0f, 40.0f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(31.5f, 6.f, 58.1f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_BigDoorLeft"), TEXT("Prototype_GameObject_Door1_Field"), &tPosAngle);

	tPosAngle.vPos = _float3(38.5f, 6.f, 58.1f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_BigDoorRight"), TEXT("Prototype_GameObject_Door2_Field"), &tPosAngle);

	//0311 추가
	tPosAngle.vPos = _float3(28.5f, 6.0f, 56.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_Torch1"), TEXT("Prototype_GameObject_Torch"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(42.f, 6.0f, 56.0f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_Torch2"), TEXT("Prototype_GameObject_Torch"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Accessories");
		return E_FAIL;
	}
	

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Respawn1::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(13.5f, 0.f, 42.0f);
	tPosAngleScale.fAngle = -90.f;
	tPosAngleScale.fScale = 8.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Door");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(57.5f, 0.f, 44.0f);
	tPosAngleScale.fAngle = 90.f;
	tPosAngleScale.fScale = 9.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Door");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(30.f, 0.f, 55.5f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 11.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Respawn1::Ready_Layer_Door");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Respawn1::Ready_Layer_OpenDoor(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	if (false == g_bSlimeClear)
	{
		tPosAngle.vPos = _float3(12.0f, 4.f, 45.8f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_RESPAWN1, pLayerTag, TEXT("Prototype_GameObject_AllDoor"), &tPosAngle);
	}

	if (false == g_bEyeCubeClear)
	{
		tPosAngle.vPos = _float3(60.2f, 4.f, 40.0f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_RESPAWN1, TEXT("Layer_OpenDoor2"), TEXT("Prototype_GameObject_AllDoor"), &tPosAngle);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Respawn1 * CLevel_Respawn1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Respawn1*	pInstance = new CLevel_Respawn1(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CLevel_Field::NativeConstruct returned the E_FAIL in CLevel_Respawn1::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Respawn1::Free()
{
	__super::Free();
}
