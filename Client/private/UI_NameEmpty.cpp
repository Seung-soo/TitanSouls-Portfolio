#include "stdafx.h"
#include "..\public\UI_NameEmpty.h"
#include "GameInstance.h"
CUI_NameEmpty::CUI_NameEmpty(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_NameEmpty::CUI_NameEmpty(const CUI_NameEmpty & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_NameEmpty::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CUI_NameEmpty::NativeConstruct(void * pArg)
{
	/*
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//나중에 플레이어 포지션으로 바꿔주기 
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 2.f, 16.f));
	*/
	m_eCurrentLevel = *(LEVEL*)pArg;
	Ready_Level_UI(m_eCurrentLevel, TEXT("Layer_UI"));



	return S_OK;
}

_int CUI_NameEmpty::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	

	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CUI_NameEmpty::LateTick(_float fTimeDelta)
{
	return _int();
}

void CUI_NameEmpty::Ready_Level_UI(LEVEL eCurrentLevel, _tchar * pLayerTag)
{
	switch (eCurrentLevel)
	{
	case LEVEL_SLIME:
		Ready_BossName_UI(eCurrentLevel, pLayerTag);
		break;
	case LEVEL_ICEBRAIN:
		Ready_BossName_UI(eCurrentLevel, pLayerTag);
		break;
	case LEVEL_COLOSUSS:
		Ready_BossName_UI(eCurrentLevel, pLayerTag);
		break;
	case LEVEL_EYECUBE:
		Ready_BossName_UI(eCurrentLevel, pLayerTag);
	case LEVEL_YETI:
		Ready_BossName_UI(eCurrentLevel, pLayerTag);
	}
}

HRESULT CUI_NameEmpty::Ready_BossName_UI(LEVEL eCurrentLevel, _tchar * pLayerTag)
{                                                          
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	if (eCurrentLevel == LEVEL_SLIME) {
		// 사본 오브젝트 호출  백 유아이랑 네임 유아이 콜 해줘
		if (pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_UIBack"), nullptr)) {
			return E_FAIL;
		}
		if (pGameInstance->Add_GameObject(LEVEL_SLIME, pLayerTag, TEXT("Prototype_GameObject_UIName_Slime"))) {
			return E_FAIL;
		}
	}
	else if (eCurrentLevel == LEVEL_ICEBRAIN) {
		if (pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_UIBack"))) {
			return E_FAIL;
		}
		if (pGameInstance->Add_GameObject(LEVEL_ICEBRAIN, pLayerTag, TEXT("Prototype_GameObject_UIName_IceBrain"))) {
			return E_FAIL;
		}
	}
	else if (eCurrentLevel == LEVEL_COLOSUSS) {
		if (pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_UIBack"))) {
			return E_FAIL;
		}
		if (pGameInstance->Add_GameObject(LEVEL_COLOSUSS, pLayerTag, TEXT("Prototype_GameObject_UIName_Colosuss"))) {
			return E_FAIL;
		}
	}
	else if (eCurrentLevel == LEVEL_EYECUBE) {
		if (pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_UIBack"))) {
			return E_FAIL;
		}
		if (pGameInstance->Add_GameObject(LEVEL_EYECUBE, pLayerTag, TEXT("Prototype_GameObject_UIName_EyeCube"))) {
			return E_FAIL;
		}
	}

	
	else if (eCurrentLevel == LEVEL_YETI){
		if (pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_UIBack"), nullptr)) {
			return E_FAIL;
		}
		if (pGameInstance->Add_GameObject(LEVEL_YETI, pLayerTag, TEXT("Prototype_GameObject_UIName_Yeti"), nullptr)) {
			return E_FAIL;
		}
	}
	
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_NameEmpty::SetUp_Components(void)
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

CUI_NameEmpty * CUI_NameEmpty::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{

	CUI_NameEmpty*	pInstance = new CUI_NameEmpty(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CPlayer in CPlayer::Create");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CUI * CUI_NameEmpty::Clone(void * pArg)
{
	CUI*	pInstance = new CUI_NameEmpty(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CEffect_Empty in CEffect_Empty::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_NameEmpty::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

}
