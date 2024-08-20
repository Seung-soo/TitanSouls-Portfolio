#include "stdafx.h"
#include "..\public\UI_LoadingText.h"
#include "GameInstance.h"

CUI_LoadingText::CUI_LoadingText(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_LoadingText::CUI_LoadingText(const CUI_LoadingText & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LoadingText::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_LoadingText::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 3.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartY = 2.f;

	m_tFrame.iFrameHeight = 128.f;
	m_tFrame.iFrameWidth = 128.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;

	m_fX = g_iWinCX * 0.5f + 35;
	m_fY = g_iWinCY * 0.5f +100;

	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	/*
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 9.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartY = 3.f;

	m_tFrame.iFrameHeight = 32.f;
	m_tFrame.iFrameWidth = 32.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;
	*/
	m_iDepth = 1;

	return __super::NativeConstruct(pArg);
}

_int CUI_LoadingText::Tick(_float fTimeDelta)
{
	m_fSpriteSpeed = 1.f;

	Go_Frame(m_fSpriteSpeed, fTimeDelta);
	return __super::Tick(fTimeDelta);
}

_int CUI_LoadingText::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingText::Render()
{
	if (FAILED(Go_Sprites())) {
		MSGBOX("Failed to call Go_Sprites in CEffect_CatchArrow");
		return E_FAIL;
	}
	return __super::Render();
}
HRESULT CUI_LoadingText::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);
	//사본 컴포넌트 추가, 
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	//사본 텍스쳐 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forLoading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_LoadingText::Go_Sprites(void)
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame))) {
		MSGBOX("m_pVIBufferCom->Go_Sprites(m_tFrame) in CUI_LoadingIcon");
		return E_FAIL;
	}
	return S_OK;
}

CUI_LoadingText * CUI_LoadingText::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_LoadingText*	pInstance = new CUI_LoadingText(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_LoadingIcon::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_LoadingText::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_LoadingText(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LoadingText::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
