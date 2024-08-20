#include "stdafx.h"
#include "..\public\Clear_Effect.h"
#include "GameInstance.h"

CClear_Effect::CClear_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CClear_Effect::CClear_Effect(const CClear_Effect & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CClear_Effect::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();

}

HRESULT CClear_Effect::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	return __super::NativeConstruct(pArg);
}

_int CClear_Effect::Tick(_float fTimeDelta)
{
	return __super::Tick(fTimeDelta);
}

_int CClear_Effect::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CClear_Effect::Render()
{
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	return __super::Render();
}

HRESULT CClear_Effect::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_White"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CAim::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CClear_Effect * CClear_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CClear_Effect*	pInstance = new CClear_Effect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CClear_Effect in CClear_Effect::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CClear_Effect::Clone(void * pArg)
{
	CGameObject*	pInstance = new CClear_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CClear_Effect in CClear_Effect::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CClear_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
