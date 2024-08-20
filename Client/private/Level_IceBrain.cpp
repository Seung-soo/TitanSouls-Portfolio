#include "stdafx.h"
#include "..\public\Level_IceBrain.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"


CLevel_IceBrain::CLevel_IceBrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_IceBrain::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}


	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_IceBrain(TEXT("Layer_Ice"))))
	{
		MSGBOX("Ready_Layer_IceBrain returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Fire(TEXT("Layer_Fire"))))
	{
		MSGBOX("Ready_Layer_Fire returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_FireButton(TEXT("Layer_FireButton"))))
	{
		MSGBOX("Ready_Layer_FireButton returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_IceBrain::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Respawn1::NativeConstruct");
		return E_FAIL;
	}

	SCENECHANGE tSceneChange;
	tSceneChange.bCrease = true;
	tSceneChange.iLevelIndex = LEVEL_END;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//pGameInstance->PlayBGM(L"IceBrain_map.ogg", 0.3f);// 사운드

	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 3.f));
	static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->LookAtXZ(_float3(16.f, 0.5f, 5.f));
	pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_IceBrain::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_IceBrain::Tick");
		return -1;
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_YETI))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_IceBrain::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_IceBrain::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_IceBrain::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_IceBrain::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("아이스브레인레벨임. "));


	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Camera(const _tchar * pLayerTag)
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


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Camera");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Terrain_IceBrain01"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Slime::Ready_Layer_Terrain");
		return E_FAIL;
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 0.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall2_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 0.f, 0.f);
	tPosAngle.fAngle = -90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, 0.f, 32.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall1_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, 20.f, 32.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Ceiling_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Player");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_IceBrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_IceDeath.bBrainDead = false;
	m_IceDeath.bIceDead = false;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Ice_Brain"), &m_IceDeath)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Ice_Brain");
		return E_FAIL;
	}
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Brain"), &m_IceDeath)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Brain");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Aim"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_UI");
		return E_FAIL;
	}
	LEVEL	eCurrentLv = LEVEL_ICEBRAIN;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_UIName_Empty"), &eCurrentLv))) {
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Fire(const _tchar * pLayerTag)
{
	m_Button.bOnButton = false;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Fire"), &m_Button)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Fire");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_FireButton(const _tchar * pLayerTag)
{
	m_Button.bOnButton = false;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	for (_uint i = 0; i < 4; i++) {
		m_Button.iButtonNum = i;
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Fire_Button"), &m_Button)))
		{
			MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_FireButton");
			return E_FAIL;
		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Door(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(20.f, 0.f, 0.5f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 8.f;

	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Ready_Layer_Door");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_IceBrain::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	POSANGLE tPosAngle;

	tPosAngle.vPos = _float3(20.f, 0.f, -8.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall3_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Prototype_GameObject_Wall3_IceBrain01");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(10.f, 0.f, -8.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall3_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Prototype_GameObject_Wall3_IceBrain01");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(16.f, 4.f, -14.f);
	tPosAngle.fAngle = 180.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall3_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Prototype_GameObject_Wall3_IceBrain01");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(16.f, 8.f, -8.f);
	tPosAngle.fAngle = 90.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall4_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Prototype_GameObject_Wall4_IceBrain01");
		return E_FAIL;
	}
	tPosAngle.vPos = _float3(16.f, 0.f, -8.f);
	tPosAngle.fAngle = 270.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_Wall4_IceBrain01"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_IceBrain::Prototype_GameObject_Wall4_IceBrain01");
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_IceBrain * CLevel_IceBrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_IceBrain*	pInstance = new CLevel_IceBrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("pInstance->NativeConstruct returned the E_FAIL in CLevel_IceBrain::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_IceBrain::Free()
{
	__super::Free();
}
