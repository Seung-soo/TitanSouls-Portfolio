#include "stdafx.h"
#include "..\public\UI_PlayerState.h"
#include "GameInstance.h"

CUI_PlayerState::CUI_PlayerState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CUI_PlayerState::CUI_PlayerState(const CUI_PlayerState & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_PlayerState::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_PlayerState::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f - 550.f;
	m_fY = g_iWinCY * 0.5f - 280.f;
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;
	m_iDepth = 6;
	return __super::NativeConstruct(pArg);
}

_int CUI_PlayerState::Tick(_float fTimeDelta)
{
	return __super::Tick(fTimeDelta);
}

_int CUI_PlayerState::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_PlayerState::Render()
{
	if (true == g_bCameraDirecting || true == g_bColossusClear)
		return S_OK;

	m_pVIBufferCom->Base_Setting();
	return __super::Render();
}

HRESULT CUI_PlayerState::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerState"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CUI_PlayerState * CUI_PlayerState::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerState*	pInstance = new CUI_PlayerState(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CAim::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_PlayerState::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_PlayerState(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Aim in CAim::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerState::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
