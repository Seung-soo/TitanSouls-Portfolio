#include "stdafx.h"
#include "..\public\UI_Skill_Empty.h"
#include "GameInstance.h"
CUI_Skill_Empty::CUI_Skill_Empty(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_Skill_Empty::CUI_Skill_Empty(const CUI_Skill_Empty & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Skill_Empty::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CUI_Skill_Empty::NativeConstruct(void * pArg)
{
	/*
	if (FAILED(SetUp_Components()))
	return E_FAIL;
	//나중에 플레이어 포지션으로 바꿔주기
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 2.f, 16.f));
	*/

	return S_OK;
}

_int CUI_Skill_Empty::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CUI_Skill_Empty::LateTick(_float fTimeDelta)
{
	return _int();
}


HRESULT CUI_Skill_Empty::SetUp_Components(void)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);
	//사본 컴포넌트 추가, 
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

CUI_Skill_Empty * CUI_Skill_Empty::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{

	CUI_Skill_Empty*	pInstance = new CUI_Skill_Empty(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CPlayer in CPlayer::Create");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CUI * CUI_Skill_Empty::Clone(void * pArg)
{
	CUI*	pInstance = new CUI_Skill_Empty(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CEffect_Empty in CEffect_Empty::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Skill_Empty::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

}
