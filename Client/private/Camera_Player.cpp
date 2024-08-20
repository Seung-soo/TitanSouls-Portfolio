#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "Level_Loading.h"
#include "All_Door.h"
#include "EndDoorLeft.h"
#include "EndDoorRight.h"
#include "Field01_DoorLight.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Player::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CCamera_Player::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Player::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CCamera_Player::NativeConstruct");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransform->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CCamera_Player::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CCamera_Player::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	_long		MouseClick = 0;
	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransform->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();


	if (LEVEL_LOADING != g_eCurrentLevel && LEVEL_CREDIT != g_eCurrentLevel)
		for (_uint i = 0; i < pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Wall")); i++)
		{
			if (LEVEL_FIELD == g_eCurrentLevel && i > 3)
				break;
			_float3 vPos = pGameInstance->Collision_SphereToRect_Camera((CCollider_Rect*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Wall"), TEXT("Com_Collider"), i), m_pColliderCom);
			if (vPos != _float3(0.f, 0.f, 0.f))
			{
				m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
				CCollider_Sphere::SPHEREDESC SphereDesc;
				SphereDesc.ObjectMatrix = m_pTransform->Get_WorldMatrix();
				SphereDesc.fRadius = 0.5f;
				SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
				m_pColliderCom->Set_SphereDesc(SphereDesc);
				m_pColliderCom->Tick();
			}
		}

	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if (vPos.y < 0.2f)
	{
		vPos.y = 0.2f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}

	// 마우스 줌인 줌아웃
	if (MouseClick = pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON))
	{
		if (true == m_isArrow)
			if (D3DXToRadian(30.0f) < m_CameraDesc.fFovy)
				m_CameraDesc.fFovy = D3DXToRadian(D3DXToDegree(m_CameraDesc.fFovy) - 0.5f);

		iZoom += 1; //++

		if (iZoom <= 1) //++
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_Zoom"), TEXT("Prototype_GameObject_Zoom"))))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CLevel_Field::Ready_Layer_UI");
				return E_FAIL;
			}
		}
	}
	else
	{
		if (D3DXToRadian(60.0f) > m_CameraDesc.fFovy)
			m_CameraDesc.fFovy = D3DXToRadian(D3DXToDegree(m_CameraDesc.fFovy) + 2.f);

		pGameInstance->Release_LayerObject(LEVEL_STATIC, TEXT("Layer_Zoom")); //++ 
		iZoom = 0; //++
	}


	if (m_bShakeX)
	{
		Camera_Shake_Horizon(fTimeDelta);
		m_bShakeX = false;
	}

	if (m_bShakeY)
	{
		Camera_Shake_Vertical(fTimeDelta);
		m_bShakeY = false;
	}

	if (m_bShakeZ)
	{
		Camera_Shake_Z(fTimeDelta);
		m_bShakeZ = false;
	}

	if (m_bShakeXEx)
	{
		Camera_Shake_Horizon(m_fShakePower, fTimeDelta);
		m_bShakeXEx = false;
	}

	if (m_bShakeYEx)
	{
		Camera_Shake_Vertical(m_fShakePower, fTimeDelta);
		m_bShakeYEx = false;
	}

	if (m_bShakeZEx)
	{
		Camera_Shake_Z(m_fShakePower, fTimeDelta);
		m_bShakeZEx = false;
	}

	if (m_bSoulMove)
		Camera_Soul_Moving(fTimeDelta);

	if (m_bIntro)
		Camera_Map_Intro(fTimeDelta);

	if (m_bInDoor)
		Camera_InDoor(fTimeDelta);

	if (m_bInBigDoor)
		Camera_InBigDoor(fTimeDelta);

	if (false == g_bCameraDirecting && D3DXToRadian(60.0f) < m_CameraDesc.fFovy)
		m_CameraDesc.fFovy = D3DXToRadian(60.0f);

	if (m_bEnding)
		Camera_EndingStart(fTimeDelta);

	if (m_bEndingSoulHit)
		Camera_EndingSoulHit(fTimeDelta);

	if (m_bEndingExit)
		Camera_EndingExit(fTimeDelta);
	
	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);
}

_int CCamera_Player::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CCamera_Player::LateTick");
		return -1;
	}

	return 0;
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

void CCamera_Player::Set_Shake(_bool bX, _bool bY, _bool bZ)
{
	m_bShakeX = bX;
	m_bShakeY = bY;
	m_bShakeZ = bZ;
}

void CCamera_Player::Set_ShakeEX(_bool bX, _bool bY, _bool bZ)
{
	m_bShakeXEx = bX;
	m_bShakeYEx = bY;
	m_bShakeZEx = bZ;
}

void CCamera_Player::Set_Directing(_bool bSoul, _bool bIntro, _bool bInDoor)
{
	m_bSoulMove = bSoul;
	m_bIntro = bIntro;
	m_bInDoor = bInDoor;
}

void CCamera_Player::Set_Map_Intro(_bool bIntro)
{
	m_bIntro = bIntro;
}

void CCamera_Player::Set_InDoor(_bool bInDoor, LEVEL eNextLevel)
{
	m_bInDoor = bInDoor;
	m_eNextLevel = eNextLevel;
}

void CCamera_Player::Set_InBigDoor(_bool bInBigDoor, LEVEL eNextLevel)
{
	m_bInBigDoor = bInBigDoor;
	m_eNextLevel = eNextLevel;
}

void CCamera_Player::Camera_Shake_Horizon(_float fTimeDelta)
{
	m_iShakeDelay--;
	if (m_iShakeDelay == 0)
	{
		if (0 >= m_fMove)
			m_fMove = 0.3f;
		else
			m_fMove = -0.3f;

		m_pTransform->Go_Right(m_fMove * fTimeDelta);
		m_iShakeDelay = 3;
	}

}

void CCamera_Player::Camera_Shake_Vertical(_float fTimeDelta)
{
	m_iShakeDelayUp--;
	if (m_iShakeDelayUp == 0)
	{
		if (0 >= m_fMoveUp)
			m_fMoveUp = 0.7f;
		else
			m_fMoveUp = -0.7f;

		m_pTransform->Go_Up(m_fMoveUp * fTimeDelta);
		m_iShakeDelayUp = 3;
	}

}

void CCamera_Player::Camera_Shake_Z(_float fTimeDelta)
{
	m_iShakeDelayZ--;
	if (m_iShakeDelayZ == 0)
	{
		if (0 >= m_fMoveZ)
			m_fMoveZ = 1.f;
		else
			m_fMoveZ = -1.f;

		m_pTransform->Go_Straight(m_fMoveZ * fTimeDelta);
		m_iShakeDelayZ = 3;
	}

}

void CCamera_Player::Camera_Stop()
{
	m_fMove = 0;
	m_fMoveZ = 0;
	m_fMoveUp = 0;
}

void CCamera_Player::Camera_Shake_Horizon(_float fPower, _float fTimeDelta)
{
	m_iShakeDelay--;
	if (m_iShakeDelay == 0)
	{
		if (0 >= m_fMove)
			m_fMove = fPower;
		else
			m_fMove = -fPower;

		m_pTransform->Go_Right(m_fMove * fTimeDelta);
		m_iShakeDelay = 3;
	}
}

void CCamera_Player::Camera_Shake_Vertical(_float fPower, _float fTimeDelta)
{
	m_iShakeDelayUp--;
	if (m_iShakeDelayUp == 0)
	{
		if (0 >= m_fMoveUp)
			m_fMoveUp = fPower;
		else
			m_fMoveUp = -fPower;

		m_pTransform->Go_Up(m_fMoveUp * fTimeDelta);
		m_iShakeDelayUp = 3;
	}
}

void CCamera_Player::Camera_Shake_Z(_float fPower, _float fTimeDelta)
{
}

void CCamera_Player::Camera_Soul_Moving(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (D3DXToRadian(89.0f) > m_CameraDesc.fFovy)
		m_CameraDesc.fFovy = D3DXToRadian(D3DXToDegree(m_CameraDesc.fFovy) + 10.f * fTimeDelta);

	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	vPos.y = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION).y;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Player::Camera_Map_Intro(_float fTimeDelta)
{
	if (m_iIntroTime < 280)
		m_pTransform->Go_Right(fTimeDelta * 0.09f);
	else if (m_iIntroTime == 280)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(4.f, 1.f, 28.f));
		m_pTransform->LookAt(_float3(28.f, 1.f, 29.f));
	}
	else if (m_iIntroTime > 280 && m_iIntroTime < 600)
		m_pTransform->LookTurn(_float3(0.f, 1.f, 0.f), _float3(5.f, 1.5f, 5.f), fTimeDelta * 0.15f);
	else if (m_iIntroTime == 600)
	{
		m_CameraDesc.fFovy = D3DXToRadian(89.0f);
		m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.3f, 16.f));
		m_pTransform->LookAt(_float3(16.f, 5.f, 20.f));
	}
	else if (m_iIntroTime > 600 && m_iIntroTime < 900)
		m_pTransform->Go_BackWard(fTimeDelta * 0.4f);
	else if (m_iIntroTime == 900)
	{
		m_iIntroTime = 0;
		m_bIntro = false;
		g_bCameraDirecting = false;
		m_CameraDesc.fFovy = D3DXToRadian(60.0f);
	}

	++m_iIntroTime;
}

void CCamera_Player::Camera_InDoor(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (D3DXToRadian(89.0f) > m_CameraDesc.fFovy)
	{
		m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
		m_CameraDesc.fFovy = D3DXToRadian(D3DXToDegree(m_CameraDesc.fFovy) + 0.15f);
		m_pPlayer->Set_Dir(CPlayer::DIR_UP);
	}
	if (D3DXToRadian(89.0f) < m_CameraDesc.fFovy)
	{
		if (0 == m_iWalkTime)
			pGameInstance->LetsPlaySound(L"EnterDoor.ogg", CSoundMgr::PLAYER_EFFECT, 1.f);
		m_iWalkTime++;
		if (m_iWalkTime < 60)
		{
			//m_pPlayer->Set_Dir(CPlayer::DIR_UP);
			m_pPlayer->Set_State(CPlayer::STATE_WALK);
			static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Go_Straight(fTimeDelta);
		}
		else if (false == m_bStartLoading)
		{
			SCENECHANGE tSceneChange;
			tSceneChange.bCrease = false;
			tSceneChange.iLevelIndex = (_uint)m_eNextLevel;

			pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Layer_SceneLoading"), TEXT("Prototype_GameObject_SceneLoading"), &tSceneChange);
			m_bStartLoading = true;
		}
		else if (true == m_bStartLoading && pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_SceneLoading")) == 0)
		{
			g_bCameraDirecting = false;
			m_iWalkTime = 0;
			m_bStartLoading = false;
			m_bInDoor = false;
			m_pPlayer->Set_State(CPlayer::STATE_IDLE);
			if (LEVEL_FIELD == g_eCurrentLevel)
			{
				m_pPlayer->Get_TransForm()->Set_State(CTransform::STATE_POSITION, _float3(9.f, 0.5f, 3.f));
				m_pPlayer->Get_TransForm()->LookAtXZ(_float3(9.f, 0.5f, 5.f));
			}
			else if (LEVEL_RESPAWN1 == g_eCurrentLevel && m_eNextLevel == LEVEL_RESPAWN2)
			{
				m_pPlayer->Get_TransForm()->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.5f, 3.f));
				m_pPlayer->Get_TransForm()->LookAtXZ(_float3(32.f, 0.5f, 5.f));
			}
			pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, m_eNextLevel));
		}
	}
	

	RELEASE_INSTANCE(CGameInstance);

}

void CCamera_Player::Camera_InBigDoor(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (200 > m_iIntroTime)
	{
		m_CameraDesc.fFovy = D3DXToRadian(89.0f);
		m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
		m_pTransform->Go_BackWard(fTimeDelta * 0.25f);
		m_pPlayer->Set_Dir(CPlayer::DIR_UP);
	}
	else if (220 > m_iIntroTime)
	{

	}
	else if (220 <= m_iIntroTime)
	{
		m_iWalkTime++;
		if (m_iWalkTime < 31)
		{
			m_pPlayer->Set_Dir(CPlayer::DIR_UP);
			m_pTransform->Go_Straight(fTimeDelta * 2.f);
		}
		else if (m_iWalkTime < 180)
		{

		}
		else
		{
			g_bCameraDirecting = false;
			m_iWalkTime = 0;
			m_bInBigDoor = false;
			m_iIntroTime = 0;
			m_pPlayer->Set_State(CPlayer::STATE_IDLE);
			m_CameraDesc.fFovy = D3DXToRadian(60.f);
			if (LEVEL_FIELD == g_eCurrentLevel)
			{
				m_pPlayer->Get_TransForm()->Set_State(CTransform::STATE_POSITION, _float3(9.f, 0.5f, 3.f));
				m_pPlayer->Get_TransForm()->LookAtXZ(_float3(9.f, 0.5f, 5.f));
			}
			else if (LEVEL_RESPAWN1 == g_eCurrentLevel && m_eNextLevel == LEVEL_RESPAWN2)
			{
				m_pPlayer->Get_TransForm()->Set_State(CTransform::STATE_POSITION, _float3(32.f, 0.5f, 3.f));
				m_pPlayer->Get_TransForm()->LookAtXZ(_float3(32.f, 0.5f, 5.f));
			}
			pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, m_eNextLevel));
		}
	}
	++m_iIntroTime;

	RELEASE_INSTANCE(CGameInstance);

}

void CCamera_Player::Camera_EndingStart(_float fTimeDelta)
{
	// 함수 들어오기 전에 _float3(16.f, 20.f, 6.f) 에서 _float3(16.f, 16.f, 0.f) 를 바라보게

	// y가 12.f보다 클 때까지 y축 방향으로 -
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if (vPos.y > 16.f)
		m_pTransform->Go_Vector(_float3(0.f, -1.f, 0.f), fTimeDelta);
	else if (vPos.y > 10.f)
	{
		m_pTransform->Turn(_float3(1.f, 0.f, 0.f), fTimeDelta * 1.2f);
		m_pTransform->Go_Vector(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	else if (vPos.y > 2.f)
	{
		m_pTransform->Go_Vector(_float3(0.f, 0.f, 1.f), fTimeDelta * 1.5f);
		m_pTransform->Go_Vector(_float3(0.f, -1.f, 0.f), fTimeDelta);
	}
	else if (vPos.y < 2.f)
	{
		vPos.y = 2.f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else if (vPos.y == 2.f)
	{
		++m_iIntroTime;
		if (m_iIntroTime < 300)
			m_pTransform->Go_Vector(_float3(0.f, 0.f, 1.f), fTimeDelta * 0.1f);
		else if (m_iIntroTime == 300)
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.0f, 23.f));
			m_pTransform->LookAt(_float3(16.f, 0.f, 25.f));
		}
		else if (m_iIntroTime < 600)
		{
			m_pTransform->Go_Vector(_float3(0.f, 0.f, -1.f), fTimeDelta * 0.02f);
		}
		else
		{
			m_bEnding = false;
			g_bCameraDirecting = false;
			m_iIntroTime = 0;
		}
	}
	// y가 8.f보다 클 때까지 룩엣 _float3(16.f, 8.f, 0.f), y축 방향으로 -
	// y가 0.2f보다 클 때까지 y축 방향으로 -, z축 방향으로 +
	// y가 0.2f보다 작거나 같을 때 y를 0.2f에 고정 그리고 천천히 z축 방향으로 + 일정 시간 동안
	
	// 일정 시간 끝나면 _float3(14.f, 0.5f, 8.f)로 좌표 옮기고 방향은 정면
	// 천천히 Go_Right하고 x가 18.f가 넘으면 카메라 풀고
}

void CCamera_Player::Camera_EndingSoulHit(_float fTimeDelta)
{
	// 화살이 맞았을 때 선형 보간 사용해서 빠르게 다가가고 아주 천천히 전진
	m_pTransform->Go_Vector(_float3(0.f, 0.f, 1.f), fTimeDelta * 0.01f);
}

void CCamera_Player::Camera_EndingExit(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	// 소울이 사라지면 _float3(16.f, 0.5f, 8.f)로 이동,  _float3(16.f, 0.5f, 0.f)를 바라본다.
	// 캐릭터 _float3(16.f, 0.5f, 4.f), _float3(16.f, 0.5f, 0.f)를 바라본다.
	if (0 == m_iIntroTime)
		m_CameraDesc.fFovy = D3DXToRadian(80.f);
	++m_iIntroTime;
	if (m_iIntroTime < 300)
	{
		m_bShakeX = true;
		m_pTransform->Go_Vector(_float3(0.f, 0.f, 1.f), fTimeDelta * 0.05f);
	}
	else if (m_iIntroTime == 300)
	{
		m_bShakeX = false;
		static_cast<CEndDoorLeft*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_EndDoorLeft")))->Set_DoorSprite(true);
		static_cast<CEndDoorRight*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_EndDoorRight")))->Set_DoorSprite(true);
	}
	else if (m_iIntroTime == 600)
	{
		pGameInstance->LetsPlaySound(L"DoorProximity.ogg", CSoundMgr::SYSTEM_EFFECT3, 1.f);
		static_cast<CEndDoorLeft*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_EndDoorLeft")))->Set_DoorOpen(true);
		static_cast<CEndDoorRight*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_EndDoorRight")))->Set_DoorOpen(true);
		POSANGLE tPosAngle;

		tPosAngle.vPos = _float3(16.f, 15.f, 1.f);
		tPosAngle.fAngle = 180.f;
		pGameInstance->Add_Prototype(LEVEL_FIELD, TEXT("Prototype_Component_Texture_DoorLight_Field01"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Door_Light.png")));
		pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Light"), TEXT("Prototype_GameObject_DoorLight_Field01"), &tPosAngle);
		static_cast<CField01_DoorLight*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Light")))->Set_Light(true);
	}
	else if (m_iIntroTime >= 600 && m_iIntroTime < 1700)
	{
		static_cast<CTransform*>(pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Light"), TEXT("Com_Transform")))->Go_Vector(_float3(0.f, 0.f, 1.f), fTimeDelta);
		if (m_iIntroTime >= 1200 && m_iIntroTime < 1700)
		{
			pGameInstance->VolumeDown(CSoundMgr::SYSTEM_EFFECT3, 0.002f);
			pGameInstance->VolumeDown(CSoundMgr::BGM, 0.001f);
		}
	}
	else if (m_iIntroTime == 1700)
	{
		m_bEndingExit = false;
		//g_bCameraDirecting = false;
		m_CameraDesc.fFovy = D3DXToRadian(60.f);
		pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_CREDIT));
		//RELEASE_INSTANCE(CGameInstance);

	}
		// 이 때 카메라가 흔들리면서 문이 천천히 열리고 빛이 나온다
		// 캐릭터가 전진하면서 들어가고
		// 씬을 넘긴다.
	RELEASE_INSTANCE(CGameInstance);
}

CCamera_Player * CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Player*	pInstance = new CCamera_Player(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CCamera_Player in CCamera_Player::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Player::Clone(void * pArg)
{
	CCamera_Player*	pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CCamera_Player in CCamera_Player::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
}
