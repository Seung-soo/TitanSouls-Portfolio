#include "stdafx.h"
#include "..\public\WhiteScreen.h"
#include "GameInstance.h"

CWhiteScreen::CWhiteScreen(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaUI(pGraphic_Device)
{
}

CWhiteScreen::CWhiteScreen(const CWhiteScreen & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CWhiteScreen::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();

}

HRESULT CWhiteScreen::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_iDepth = 0;

	if (false == g_bColossusClear)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->StopSound(CSoundMgr::BGM);

		pGameInstance->LetsPlaySound(L"Kill.ogg", CSoundMgr::KILL_MONSTER, 1.f); // »ç¿îµå

		RELEASE_INSTANCE(CGameInstance);

	}

	return __super::NativeConstruct(pArg);
}

_int CWhiteScreen::Tick(_float fTimeDelta)
{
	if (false == g_bColossusClear)
		--m_iAlpha;
	if (m_iAlpha < 0)
		return -1;
	return __super::Tick(fTimeDelta);
}

_int CWhiteScreen::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CWhiteScreen::Render()
{
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	if (m_iAlpha > 255)
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	return __super::Render();
}

HRESULT CWhiteScreen::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWhiteScreen::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWhiteScreen::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWhiteScreen::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_White"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWhiteScreen::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CWhiteScreen * CWhiteScreen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWhiteScreen*	pInstance = new CWhiteScreen(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CWhiteScreen in CWhiteScreen::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWhiteScreen::Clone(void * pArg)
{
	CGameObject*	pInstance = new CWhiteScreen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CWhiteScreen in CWhiteScreen::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWhiteScreen::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
