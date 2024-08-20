#include "stdafx.h"
#include "..\public\UI_StartText.h"

#include "GameInstance.h"

CUI_StartText::CUI_StartText(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_StartText::CUI_StartText(const CUI_StartText & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_StartText::NativeConstruct_Prototype()
{
	//m_iUIDepth = 3;

	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_StartText::NativeConstruct(void * pArg)
{
	//m_iUIDepth = 3;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameEndX = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameStartY = 0.f;

	m_tFrame.iFrameHeight = 128.f;
	m_tFrame.iFrameWidth = 128.f;
	m_tFrame.iTexHeight = 512.f;
	m_tFrame.iTexWidth = 512.f;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 128.f;
	m_fSizeY = 128.f;
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
	m_iDepth = 0;

	return __super::NativeConstruct(pArg);
}

_int CUI_StartText::Tick(_float fTimeDelta)
{
	//m_iUIDepth = 3;

	m_fSpriteSpeed = 1.f;

	Go_Frame(m_fSpriteSpeed, fTimeDelta);
	return __super::Tick(fTimeDelta);
}

_int CUI_StartText::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_StartText::Render()
{
	if (FAILED(Go_Sprites())) {
		MSGBOX("Failed to call Go_Sprites in CEffect_CatchArrow");
		return E_FAIL;
	}
	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CBrain::Render");
		return E_FAIL;
	}
	return __super::Render();
}
HRESULT CUI_StartText::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_forMainMenu"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_StartText::SetUp_RenderState()
{
	/*
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(1,255,255,255));
	
	*/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
	return S_OK;
}

HRESULT CUI_StartText::Go_Sprites(void)
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame))) {
		MSGBOX("m_pVIBufferCom->Go_Sprites(m_tFrame) in CUI_StartText");
		return E_FAIL;
	}
	return S_OK;
}

CUI_StartText * CUI_StartText::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_StartText*	pInstance = new CUI_StartText(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_LoadingIcon::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_StartText::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_StartText(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_StartText::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
