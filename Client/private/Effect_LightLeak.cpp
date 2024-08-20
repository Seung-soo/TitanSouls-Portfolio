#include "stdafx.h"
#include "..\public\Effect_LightLeak.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Clear_Effect.h"
#include "Player.h"
#include "Brother.h"

CEffect_LightLeak::CEffect_LightLeak(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{
}

CEffect_LightLeak::CEffect_LightLeak(const CEffect_LightLeak & rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_LightLeak::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEffect_LightLeak::NativeConstruct_Prototype");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LightLeak"), CLightLeak::Create(m_pGraphic_Device))))
	{
		MSGBOX("m_pGameInstance->Add_Prototype returned E_FAIL in CEffect_LightLeak::NativeConstruct_Prototype");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CEffect_LightLeak::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEffect_LightLeak::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEffect_LightLeak::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_pTransformCom->Set_WorldMatrix(pPlayerTransform->Get_WorldMatrix());


	m_pCameraPlayer = static_cast<CCamera_Player*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Camera")));

	static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")))->Set_State(CPlayer::STATE_LIGHTLEAK);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CEffect_LightLeak::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEffect_LightLeak::Tick");
		return -1;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if ((m_iMakeLightTime <= m_iDelay && m_iMakeLightTime > m_iDelay - 4) && m_iDelay >= 0)
	{
		m_pCameraPlayer->Set_Shake(false, false, true);
	}


	m_iMakeLightTime--;

	if (m_iMakeLightTime <= 0 && m_iDelay >= 0)
	{
		m_iDir++;
		m_pCameraPlayer->Set_Shake(false, false, true);
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPos += vLook * (rand() % 100 / 100000.f);

		POSANGLE tPosAngle = { vPos , _float(rand() % 360) };
		pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_LightLeak"), TEXT("Prototype_GameObject_LightLeak"), &tPosAngle);

		if (m_iDelay >= 20)
			m_iDelay -= 4;
		else if (m_iDelay >= 10)
			m_iDelay -= 2;
		else
			m_iDelay--;
		m_iMakeLightTime = m_iDelay;
	}
	else if (m_iDelay < 0 && m_iMakeLightTime > -150)
	{
		if (m_iMakeLightTime == -10)
			pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_White"), TEXT("Prototype_GameObject_Effect_White"));
		else if (-10 > m_iMakeLightTime)
		{
			static_cast<CClear_Effect*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_White")))->Set_Alpha(m_iAlpha);
			if (m_bDown == false)
			{
				m_iAlpha += 2;
				//static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Go_Up(fTimeDelta * 0.1f);

			}

			if (m_bDown == false && m_iAlpha > 300)
				m_bDown = true;

			if (m_bDown == true && m_iAlpha > 254)
			{
				pGameInstance->Release_LayerObject(g_eCurrentLevel, TEXT("Layer_LightLeak"));
			}
		}
		m_iDir++;
		m_pCameraPlayer->Set_Shake(false, false, true);

		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPos += vLook * (rand() % 100 / 100000.f);

		POSANGLE tPosAngle = { vPos , _float(rand() % 360) };
		pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_LightLeak"), TEXT("Prototype_GameObject_LightLeak"), &tPosAngle);
	}

	else if (m_iMakeLightTime <= -150)
	{
		static_cast<CClear_Effect*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_White")))->Set_Alpha(m_iAlpha);

		//if (m_bDown == false && m_iAlpha < 255)
		//	m_iAlpha += 2;
		if (m_bDown == false)
		{
			m_iAlpha += 2;
			//static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Go_Up(fTimeDelta * 0.1f);

		}
		if (m_bDown == false && m_iAlpha > 300)
			m_bDown = true;

		if (m_bDown == true && m_iAlpha > 254)
		{
			pGameInstance->Release_LayerObject(g_eCurrentLevel, TEXT("Layer_LightLeak"));
		}

		if (m_bDown == true && m_iAlpha == 0)
		{
			pGameInstance->Release_LayerObject(g_eCurrentLevel, TEXT("Layer_White"));
			RELEASE_INSTANCE(CGameInstance);
			g_bCameraDirecting = false;
			m_pCameraPlayer->Set_Directing(false, false, false);

			switch (g_eCurrentLevel)
			{
			case LEVEL_SLIME:
				g_bSlimeClear = true;
				break;
			case LEVEL_ICEBRAIN:
				g_bIceBrainClear = true;
				break;
			case LEVEL_COLOSUSS:
				g_bColossusClear = true;
				// 새로운 카메라 연출
				static_cast<CTransform*>(m_pCameraPlayer->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _float3(16.f, 30.f, 3.f));
				static_cast<CTransform*>(m_pCameraPlayer->Get_Component(TEXT("Com_Transform")))->LookAt(_float3(16.f, 28.f, 1.f));
				pGameInstance->StopSound(CSoundMgr::BGM);
				pGameInstance->PlayBGM(L"EndingSound.ogg", 0.3f);
				g_bCameraDirecting = true;
				m_pCameraPlayer->Set_Ending(true);
				break;
			case LEVEL_EYECUBE:
				g_bEyeCubeClear = true;
				break;
			case LEVEL_YETI:
				g_bYetiClear = true;
				break;
			}
			return -1;
		}



	}
	if (m_bDown == false && m_iAlpha > 300)
		m_bDown = true;
	if (m_bDown == true)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
		m_iAlpha--;
		static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Go_Up(fTimeDelta * -0.15f);
		if (LEVEL_COLOSUSS == g_eCurrentLevel)
		{
			if (true == m_bEndStart)
			{
				g_bColossusClear = true;
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, pPlayer->Get_TransForm()->Get_State(CTransform::STATE_POSITION) - _float3(0.f, 10.f, 0.f));
				m_bEndStart = false;
				POSANGLE tPosAngle;
				tPosAngle.vPos = _float3(0.f, 32.f, 32.f);
				tPosAngle.fAngle = 270.f;

				if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Wall"), TEXT("Prototype_GameObject_Ceiling_IceBrain01"), &tPosAngle)))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CEffect_LightLeak::Tick");
					return E_FAIL;
				}
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Brother"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Brother.png")))))
				{
					MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLevel_Ending::Ready_Layer_Brothe(Component_Texture_Brother)");
					return E_FAIL;
				}
				if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Brother"), CBrother::Create(m_pGraphic_Device))))
				{
					MSGBOX("pGameInstance->Add_Prototype returned E_FAIL in CLevel_Ending::Ready_Layer_Brother(GameObject_Brother)");
					return E_FAIL;
				}
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_COLOSUSS, TEXT("Layer_Brother"), TEXT("Prototype_GameObject_Brother"), &tPosAngle)))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CEffect_LightLeak::Tick");
					return E_FAIL;
				}
				//0312추가

				tPosAngle.vPos = _float3(11.f, 14.f, 0.01f);
				tPosAngle.fAngle = 180.f;

				if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_EndDoorRight"), TEXT("Prototype_GameObject_EndDoorRight"), &tPosAngle)))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CEffect_LightLeak::Ready_Layer_Door");
					return E_FAIL;
				}

				tPosAngle.vPos = _float3(21.f, 14.f, 0.01f);
				tPosAngle.fAngle = 180.f;

				if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_EndDoorLeft"), TEXT("Prototype_GameObject_EndDoorLeft"), &tPosAngle)))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CEffect_LightLeak::Ready_Layer_Door");
					return E_FAIL;
				}

				tPosAngle.vPos = _float3(16.f, 15.8f, 0.015f);
				tPosAngle.fAngle = 180.f;

				if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_EndDoorFrame"), TEXT("Prototype_GameObject_EndDoor"), &tPosAngle)))
				{
					MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CEffect_LightLeak::Ready_Layer_Door");
					return E_FAIL;
				}
				CTransform* pPlayerTrans = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
				pPlayerTrans->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 4.f));
				pPlayerTrans->LookAt(_float3(16.f, 0.5f, 6.f));

				_float3 vLook = pPlayerTrans->Get_State(CTransform::STATE_LOOK);
				D3DXVec3Normalize(&vLook, &vLook);

				_float3 vPlayerPos = pPlayerTrans->Get_State(CTransform::STATE_POSITION);


				_float3 CameraPos = vPlayerPos - (vLook * 2);
				CameraPos += _float3(0.f, 0.3f, 0.f);

				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, CameraPos);
				static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->LookAt(vPlayerPos + _float3(0.f, 0.7f, 0.f));
			}
			//m_bDown = false;
		}
		pPlayer->Set_TurnNum(0);
		if (static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION).y < 0.5f)
		{
			pPlayer->Set_Fly(false);
			pPlayer->Set_State(CPlayer::STATE_IDLE);
			pPlayer->Set_Dir(CPlayer::DIR_UP);
			pPlayer->Set_SoulStart(false);
		}
	}

	if (m_iAlpha > 254)
		static_cast<CClear_Effect*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_White")))->Set_Alpha(255);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}


_int CEffect_LightLeak::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEffect_LightLeak::LateTick");
		return -1;
	}



	return _int();
}

HRESULT CEffect_LightLeak::Render()
{


	return S_OK;
}

HRESULT CEffect_LightLeak::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	TransformDesc.fRotationPerSec = D3DXToRadian(PLAYER_ROTATIONSPEED);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEffect_LightLeak::SetUp_Components(Transform)");
		return E_FAIL;
	}

	return S_OK;
}

CEffect_LightLeak * CEffect_LightLeak::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_LightLeak*	pInstance = new CEffect_LightLeak(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Effect_LightLeak in CEffect_LightLeak::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_LightLeak::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_LightLeak(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Effect_LightLeak in CEffect_LightLeak::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_LightLeak::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
