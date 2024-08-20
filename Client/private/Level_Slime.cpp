#include "stdafx.h"
#include "..\public\Level_Slime.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"

CLevel_Slime::CLevel_Slime(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Slime::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Slime::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 4.f));
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(_float3(16.f, 0.5f, 5.f));
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

	if (FAILED(Ready_Layer_Slime(TEXT("Layer_Slime_Boss"))))
	{
		MSGBOX("Ready_Layer_Slime returned E_FAIL in CLevel_Slime::NativeConstruct")
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
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Slime::NativeConstruct");
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
	pGameInstance->PlayBGM(L"Slime_map.ogg", 1.f);//사운드
	pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_Slime::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Slime::Tick");
		return -1;
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_EYECUBE))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_Slime::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Slime::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_Slime::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Slime::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("슬라임레벨임. "));


	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 3.f, -3.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Camera");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Terrain_Slime01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Terrain");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Wall_Slime01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Wall2_Slime01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 0.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Wall_Slime01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Wall_Slime01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 20.f, 32.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Ceiling_Slime01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Player");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Slime(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_SlimeDead.bBossSlimeDead = false;
	m_SlimeDead.bHeartDead = false;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Slime_Boss"), &m_SlimeDead)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Slime(Boss)");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Heart"), TEXT("Prototype_GameObject_Slime_Heart"), &m_SlimeDead)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Slime(Heart)");
		return E_FAIL;
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	//Light
	{
		tPosAngle.vPos = _float3(16.f, 10.f, 16.f);
		tPosAngle.fAngle = 10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Light_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 1");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(5.f, 10.f, 25.f);
		tPosAngle.fAngle = 5.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Light_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(28.f, 10.f, 11.f);
		tPosAngle.fAngle = -10.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Light_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 1");
			return E_FAIL;
		}

	}

	//Mist
	{
		tPosAngle.vPos = _float3(10.f, 19.5f, 22.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(22.f, 17.5f, 8.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(8.f, 15.5f, 10.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(25.f, 13.5f, 24.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(10.f, 14.5f, 20.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(22.f, 16.5f, 15.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}


		tPosAngle.vPos = _float3(16.f, 18.5f, 28.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Mist_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 2");
			return E_FAIL;
		}

	}

	//Pipe
	{
		tPosAngle.vPos = _float3(8.f, 10.f, 31.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe1_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(12.f, 9.7f, 31.8f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(4.f, 9.7f, 31.8f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(8.f, 10.f, 31.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe1_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(12.f, 9.7f, 31.8f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(4.f, 9.7f, 31.8f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(1.f, 17.f, 14.f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe1_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(0.8f, 16.7f, 10.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(0.8f, 16.7f, 18.0f);
		tPosAngle.fAngle = 180.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Pipe2_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 3");
			return E_FAIL;
		}
	}

	//Skull ,Legiron
	{
		tPosAngle.vPos = _float3(16.f, 0.5f, 31.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Skull_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(29.f, 0.5f, 5.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Skull_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(7.f, 0.5f, 19.f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Skull_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}


		//Legiron
		tPosAngle.vPos = _float3(14.f, 4.f, 31.0f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(18.f, 4.f, 31.0f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(31.f, 4.f, 11.0f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(31.f, 4.f, 22.0f);
		tPosAngle.fAngle = 90.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(1.f, 4.f, 11.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(1.f, 4.f, 22.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Legiron_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

	}

	//Flask
	{
		tPosAngle.vPos = _float3(2.f, 0.2f, 10.0f);
		tPosAngle.fAngle = 320.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask01_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(4.f, 0.2f, 28.0f);
		tPosAngle.fAngle = 45.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask01_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(18.f, 0.2f, 15.0f);
		tPosAngle.fAngle = 45.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask01_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
		tPosAngle.vPos = _float3(24.f, 0.2f, 24.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask01_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
		//
		tPosAngle.vPos = _float3(6.f, 0.2f, 27.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask02_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(28.f, 0.2f, 4.0f);
		tPosAngle.fAngle = 45.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask02_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(18.f, 0.2f, 24.0f);
		tPosAngle.fAngle = 320.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask02_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(22.f, 0.2f, 16.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask02_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
		//
		tPosAngle.vPos = _float3(19.f, 0.2f, 4.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask03_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(11.f, 0.2f, 31.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask03_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(7.f, 0.2f, 14.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask03_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(28.f, 0.2f, 11.0f);
		tPosAngle.fAngle = 270.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Flask03_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
	}

	//Water_drop
	{
		tPosAngle.vPos = _float3(26.f, 9.5f, 10.0f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Water_drop_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(19.f, 9.5f, 28.0f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Water_drop_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}

		tPosAngle.vPos = _float3(3.f, 9.5f, 22.0f);
		tPosAngle.fAngle = 0.f;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_Water_drop_Slime01"), &tPosAngle)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Accessories 4");
			return E_FAIL;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Aim"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	LEVEL	eCurrentLv = LEVEL_SLIME;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_UIName_Empty"), &eCurrentLv))) {
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");

		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Slime::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(20.5f, 0.f, 0.5f);
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

CLevel_Slime * CLevel_Slime::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Slime*	pInstance = new CLevel_Slime(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_Slime::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Slime::Free()
{
	__super::Free();
}
