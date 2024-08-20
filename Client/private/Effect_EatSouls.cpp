#include "stdafx.h"
#include "..\public\Effect_EatSouls.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Player.h"

CEffect_EatSouls::CEffect_EatSouls(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device), m_iDelay(0)
{
}

CEffect_EatSouls::CEffect_EatSouls(const CEffect_EatSouls & rhs)
	: CEffect(rhs), m_iDelay(0)
{
}

HRESULT CEffect_EatSouls::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEffect_EatSouls::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_EatSouls::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEffect_EatSouls::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEffect_EatSouls::NativeConstruct");
		return E_FAIL;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_float3*)pArg);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));
	m_pPlayer->Set_SoulStart(true);
	m_vLook = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vCenterPoint = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vLook * 0.5f;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Cross(&m_vRight, &_float3(0.f, 1.f, 0.f), &m_vLook);

	for (_uint i = 0; i < 8; i++)
	{
		_float3 vAxis;
		_float fAngle = 0.f;
		if (4 > i)
			fAngle = 100.f - i * 30.f;
		else
			fAngle = 110.f - i * 30.f;
		_float4x4 RotationMatrix;
		D3DXMatrixRotationAxis(&RotationMatrix, &m_vLook, D3DXToRadian(fAngle));
		D3DXVec3TransformNormal(&vAxis, &m_vRight, &RotationMatrix);
		m_vAxis[i] = vAxis;
	}

	for (size_t i = 0; i < 50; i++)
	{
		_uint iRandFirst = rand() % 8;
		_uint iRandSecond = rand() % 8;
		_float3 vTemp = m_vAxis[iRandFirst];
		m_vAxis[iRandFirst] = m_vAxis[iRandSecond];
		m_vAxis[iRandSecond] = vTemp;
	}
	pGameInstance->StopSound(CSoundMgr::SYSTEM_EFFECT);
	pGameInstance->VolumeUp(CSoundMgr::SYSTEM_EFFECT, 0.6f);
	pGameInstance->LetsPlaySound(L"Release.ogg", CSoundMgr::SYSTEM_EFFECT, 0.6f);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CEffect_EatSouls::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEffect_EatSouls::Tick");
		return -1;
	}

	if (m_iMakeTime <= 0)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_iDelay % 10 == 0)
	{


		--m_iMakeTime;
		CSoul::SOULDESC tSoulDesc;
		tSoulDesc.vAxis = m_vAxis[m_iMakeTime];
		tSoulDesc.vCenterPosition = m_vCenterPoint;
		if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Soul"), TEXT("Prototype_GameObject_Soul"), &tSoulDesc)))
		{
			MSGBOX("pGameInstance->AddGameObject returned E_FAIL in CEffect_EatSouls::Tick");
			return E_FAIL;
		}

	}
	m_iDelay++;




	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CEffect_EatSouls::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEffect_EatSouls::LateTick");
		return -1;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Find_Layer(g_eCurrentLevel, TEXT("Layer_Soul")) != nullptr && pGameInstance->Get_Size(g_eCurrentLevel, TEXT("Layer_Soul")) == 0)
	{
		//pGameInstance->StopSound(CSoundMgr::SYSTEM_EFFECT);
		pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Effect"), TEXT("Prototype_GameObject_Effect_LightLeak"));
		//pGameInstance->LetsPlaySound(L"Player_soul3.ogg", CSoundMgr::SYSTEM_EFFECT2, 1.f); // »ç¿îµå
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}
	RELEASE_INSTANCE(CGameInstance);


	return _int();
}

HRESULT CEffect_EatSouls::Render()
{


	return S_OK;
}

HRESULT CEffect_EatSouls::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	TransformDesc.fRotationPerSec = D3DXToRadian(PLAYER_ROTATIONSPEED);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEffect_EatSouls::SetUp_Components(Transform)");
		return E_FAIL;
	}

	return S_OK;
}

CEffect_EatSouls * CEffect_EatSouls::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_EatSouls*	pInstance = new CEffect_EatSouls(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_EatSouls in CEffect_EatSouls::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_EatSouls::Clone(void * pArg)
{
	CGameObject*	pInstance = new CEffect_EatSouls(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CEffect_EatSouls in CEffect_EatSouls::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_EatSouls::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
}
