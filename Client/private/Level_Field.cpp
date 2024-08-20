#include "stdafx.h"
#include "..\public\Level_Field.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Level_Loading.h"
#include "Camera_Player.h"

CLevel_Field::CLevel_Field(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

// 필드 레벨이 시작할 때 불러오고 호출해야하는 것들
HRESULT CLevel_Field::NativeConstruct()
{
	if (0 > __super::NativeConstruct())		// CLevel의 초기화 함수를 호출
	{
		MSGBOX("CLevel::NativeConstruct returned the E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		MSGBOX("Ready_Layer_Camera returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		MSGBOX("Ready_Layer_Terrain returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Wall(TEXT("Layer_Wall"))))
	{
		MSGBOX("Ready_Layer_Wall returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Handrail(TEXT("Layer_Handrail"))))
	{
		MSGBOX("Ready_Layer_Handrail returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Accessories(TEXT("Layer_Accessories"))))
	{
		MSGBOX("Ready_Layer_Accessories returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	{
		MSGBOX("Ready_Layer_Player returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	{
		MSGBOX("Ready_Layer_Sky returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
	{
		MSGBOX("Ready_Layer_UI returned E_FAIL in CLevel_Field::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->PlayBGM(L"Overworld1.ogg", 0.3f);// 사운드

	CCamera_Player* pCamera = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));
	pCamera->Set_Map_Intro(true);
	g_bCameraDirecting = true;
	static_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(27.f, 15.f, 4.f));
	static_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")))->LookAt(_float3(24.f, 10.f, 7.f));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

// 필드 레벨에서 계속 갱신해줘야하는 것들
_int CLevel_Field::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))		// CLevel의 갱신 함수를 호출
	{
		MSGBOX("0 > CLevel::Tick in CLevel_Field::Tick");
		return -1;
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_RESPAWN1))))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

_int CLevel_Field::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))	// CLevel의 늦게 갱신하는 함수를 호출
	{
		MSGBOX("0 > CLevel::LateTick in CLevel_Field::LateTick");
		return -1;
	}

	return 0;
}

// 필드 레벨에서 그려야하는 것들
HRESULT CLevel_Field::Render()
{
	if (0 > __super::Render())				// CLevel의 그리는 함수를 호출
	{
		MSGBOX("CLevel::Render returned the E_FAIL in CLevel_Field::Render");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("필드레벨임. "));


	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Camera(const _tchar * pLayerTag)
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
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Camera");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Terrain_Field"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Terrain");
		return E_FAIL;
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Wall(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;
	tPosAngle.vPos = _float3(0.f, -2.f, 32.f);
	tPosAngle.fAngle = 0.f;
	// 정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallFront_Field"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, -2.f, 0.f);
	tPosAngle.fAngle = 180.f;
	// 후면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallBack_Field"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(0.f, -2.f, 0.f);
	tPosAngle.fAngle = -90.f;
	// 왼면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallSide_Field"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngle.vPos = _float3(32.f, -2.f, 32.f);
	tPosAngle.fAngle = 90.f;
	// 오른면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallSide_Field"), &tPosAngle)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	// 투명벽
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(12.f, 0.f, 0.f);
	tPosAngleScale.fAngle = -90.f;
	tPosAngleScale.fScale = 6.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}


	tPosAngleScale.vPos = _float3(20.f, 0.f, 6.f);
	tPosAngleScale.fAngle = 90.f;
	tPosAngleScale.fScale = 6.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(12.f, 0.f, 6.f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 3.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(23.f, 0.f, 6.f);
	tPosAngleScale.fAngle = 180.f;
	tPosAngleScale.fScale = 3.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(9.f, 0.f, 6.f);
	tPosAngleScale.fAngle = -90.f;
	tPosAngleScale.fScale = 10.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(23.f, 0.f, 18.f);
	tPosAngleScale.fAngle = 90.f;
	tPosAngleScale.fScale = 10.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(9.f, 0.f, 18.f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 3.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(20.f, 0.f, 18.f);
	tPosAngleScale.fAngle = 0.f;
	tPosAngleScale.fScale = 3.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(12.f, 0.f, 18.f);
	tPosAngleScale.fAngle = -90.f;
	tPosAngleScale.fScale = 16.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	tPosAngleScale.vPos = _float3(20.f, 0.f, 32.f);
	tPosAngleScale.fAngle = 90.f;
	tPosAngleScale.fScale = 16.f;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Handrail(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	POSANGLE tPosAngle;

	// 왼
	tPosAngle.vPos = _float3(10.5f, -1.f, 1.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 4.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(7.5f, -1.f, 7.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(7.5f, -1.f, 10.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(7.5f, -1.f, 13.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(7.5f, -1.f, 16.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 19.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 22.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 25.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 28.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(10.5f, -1.f, 31.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);


	//오

	tPosAngle.vPos = _float3(21.5f, -1.f, 1.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 4.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(24.5f, -1.f, 7.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(24.5f, -1.f, 10.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(24.5f, -1.f, 13.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(24.5f, -1.f, 16.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 19.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 22.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 25.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 28.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);

	tPosAngle.vPos = _float3(21.5f, -1.f, 31.5f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle);


	RELEASE_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Handrall_Field01"), &tPosAngle)))
	//{
	//	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Wall");
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Player");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Sky(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Sky");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Aim"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_SkillRolling")))) {
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_SkillArrow"))))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_LButtonIcon")))) {
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_LShiftIcon")))) {
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_UI_PlayerAlive")))) {
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
		return E_FAIL;
	}

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_SceneLoading"))))
	//{
	//	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
	//	return E_FAIL;
	//}

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_UIBackground"))))
	//{
	//	MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
	//	return E_FAIL;
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Accessories(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	POSANGLE tPosAngle;

	tPosAngle.vPos = _float3(16.f, 5.f, 31.9f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, TEXT("Layer_Light"), TEXT("Prototype_GameObject_DoorLight_Field01"), &tPosAngle);

	// 밑바닥
	tPosAngle.vPos = _float3(16.f, -2.f, 16.f);
	tPosAngle.fAngle = 90.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Ceiling"), &tPosAngle);
	//RECT Water
	tPosAngle.vPos = _float3(16.f, -1.f, 16.f);
	tPosAngle.fAngle = 90.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Water"), &tPosAngle);

	// RECT 천장
	tPosAngle.vPos = _float3(16.f, 26.f, 16.f);
	tPosAngle.fAngle = -90.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Ceiling"), &tPosAngle);

	//RECT Door

	tPosAngle.vPos = _float3(12.5f, 6.f, 32.1f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, TEXT("Layer_BigDoorLeft"), TEXT("Prototype_GameObject_Door1_Field"), &tPosAngle);

	tPosAngle.vPos = _float3(19.5f, 6.f, 32.1f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, TEXT("Layer_BigDoorRight"), TEXT("Prototype_GameObject_Door2_Field"), &tPosAngle);

	// RECT 천장 Grass
	{

		tPosAngle.vPos = _float3(10.f, 25.5f, 5.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(27.f, 25.f, 10.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(27.f, 25.3f, 7.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 25.5f, 29.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(36.f, 25.f, 26.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(32.f, 25.3f, 24.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);

		tPosAngle.vPos = _float3(-4.f, 24.9f, 16.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Up"), &tPosAngle);


	}

	// RECT Grass
	{
		tPosAngle.vPos = _float3(31.f, 10.f, 10.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(30.8f, 10.f, 22.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(30.6f, 12.f, 16.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(30.4f, 15.f, 22.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(30.2f, 16.f, 10.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		//==
		tPosAngle.vPos = _float3(0.2f, 10.f, 10.f);
		tPosAngle.fAngle = -90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(0.4f, 10.f, 22.f);
		tPosAngle.fAngle = -90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(0.6f, 12.f, 16.f);
		tPosAngle.fAngle = -90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(0.8f, 15.f, 22.f);
		tPosAngle.fAngle = -90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(1.0f, 16.f, 10.f);
		tPosAngle.fAngle = -90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		//==
		tPosAngle.vPos = _float3(10.f, 16.f, 31.5f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(21.f, 16.f, 31.3f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		//==
		tPosAngle.vPos = _float3(10.f, 16.f, 0.5f);
		tPosAngle.fAngle = 180.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

		tPosAngle.vPos = _float3(21.f, 16.f, 0.3f);
		tPosAngle.fAngle = 180.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass00"), &tPosAngle);

	}

	//Rect Light / Dark
	{
		tPosAngle.vPos = _float3(11.f, 16.f, 16.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 16.f, 11.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 16.f, 21.f);
		tPosAngle.fAngle = 180.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light"), &tPosAngle);

		tPosAngle.vPos = _float3(21.f, 16.f, 16.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light"), &tPosAngle);

		tPosAngle.vPos = _float3(11.f, 16.f, 16.f);
		tPosAngle.fAngle = 270.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Dark"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 16.f, 11.f);
		tPosAngle.fAngle = 180.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Dark"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 16.f, 21.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Dark"), &tPosAngle);

		tPosAngle.vPos = _float3(21.f, 16.f, 16.f);
		tPosAngle.fAngle = 90.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Dark"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 14.f, 10.f);
		tPosAngle.fAngle = -20.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 14.f, 20.f);
		tPosAngle.fAngle = -10.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);

		tPosAngle.vPos = _float3(25.f, 14.f, 20.f);
		tPosAngle.fAngle = -10.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);

		tPosAngle.vPos = _float3(16.f, 14.f, 10.f);
		tPosAngle.fAngle = 20.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 14.f, 20.f);
		tPosAngle.fAngle = 10.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);

		tPosAngle.vPos = _float3(25.f, 14.f, 20.f);
		tPosAngle.fAngle = 10.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Light2"), &tPosAngle);
	}

	// Grass_down
	{
		tPosAngle.vPos = _float3(12.5f, 0.2f, 29.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(13.f, 0.2f, 27.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(18.f, 0.2f, 25.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(19.f, 0.2f, 24.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 0.2f, 17.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(11.f, 0.2f, 16.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(11.5f, 0.2f, 16.5f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 0.2f, 14.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(10.5f, 0.2f, 14.5f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(9.5f, 0.2f, 13.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(21.5f, 0.2f, 14.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(20.5f, 0.2f, 14.5f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(20.f, 0.2f, 15.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(19.f, 0.2f, 5.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(18.5f, 0.2f, 5.5f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(18.f, 0.2f, 6.f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);

		tPosAngle.vPos = _float3(19.f, 0.2f, 5.7f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass_Down"), &tPosAngle);
	}

	tPosAngle.vPos = _float3(22.f, 2.f, 16.0f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Tree"), &tPosAngle);

	// Grass_EX
	{
		tPosAngle.vPos = _float3(20.f, 16.f, 7.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(7.f, 22.f, 18.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(17.f, 24.f, 12.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(15.f, 18.f, 28.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(11.f, 20.f, 25.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(10.f, 21.f, 11.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(19.f, 17.f, 27.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(19.f, 19.f, 15.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(22.f, 23.f, 9.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);

		tPosAngle.vPos = _float3(9.f, 18.f, 23.0f);
		tPosAngle.fAngle = 0.f;
		pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Field01_Grass04"), &tPosAngle);


	}

	// Terrain2

	tPosAngle.vPos = _float3(0.f, 0.f, -31.f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Field"), &tPosAngle);

	tPosAngle.vPos = _float3(1.f, 0.f, 32.f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_Terrain2_Field"), &tPosAngle);

	tPosAngle.vPos = _float3(16.f, 15.f, -16.f);
	tPosAngle.fAngle = 270.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_CField01_Ceiling2"), &tPosAngle);

	tPosAngle.vPos = _float3(16.f, 15.f, 48.f);
	tPosAngle.fAngle = 270.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_CField01_Ceiling2"), &tPosAngle);

	tPosAngle.vPos = _float3(16.f, 16.f, 48.f);
	tPosAngle.fAngle = 0.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle);

	tPosAngle.vPos = _float3(16.f, 16.f, -16.f);
	tPosAngle.fAngle = 180.f;
	pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_CField01_WallDark"), &tPosAngle);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Field::Ready_Layer_Door(const _tchar * pLayerTag)
{
	POSANGLESCALE tPosAngleScale;
	tPosAngleScale.vPos = _float3(12.f, 0.f, 31.5f);
	tPosAngleScale.fAngle =	0.f;
	tPosAngleScale.fScale = 8.f;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_FIELD, pLayerTag, TEXT("Prototype_GameObject_WallHide_Field"), &tPosAngleScale)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_Door");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Field * CLevel_Field::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Field*	pInstance = new CLevel_Field(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("CLevel_Field::NativeConstruct returned the E_FAIL in CLevel_Field::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Field::Free()
{
	__super::Free();
}
