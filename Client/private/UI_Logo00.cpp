#include "stdafx.h"
#include "..\public\UI_Logo00.h"
#include "GameInstance.h"
CUI_Logo00::CUI_Logo00(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CAlphaUI(pGraphic_Device)
{
}

CUI_Logo00::CUI_Logo00(const CUI_Logo00 & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_Logo00::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Logo00::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = 256.f;
	m_fSizeY = 256.f;
	m_iDepth = 0;


	return __super::NativeConstruct(pArg);
}

_int CUI_Logo00::Tick(_float fTimeDelta)
{
	if (!m_bEnd) {
		if (!m_bOff) {
			m_iAlpha += 3;
			if (255 <= m_iAlpha) {
				m_bOff = true;
			}
		}
		else {
			m_iAlpha -= 3;
			if (0 >= m_iAlpha)
			{
				m_bEnd = true;
				m_bPlay = false;
			}

		}
	}
	
	return __super::Tick(fTimeDelta);
}

_int CUI_Logo00::LateTick(_float fTimeDelta)
{

	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_Logo00::Render()
{
	m_pVIBufferCom->Base_Setting();
	
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	return __super::Render();
}
HRESULT CUI_Logo00::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI00"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}
HRESULT CUI_Logo00::SetUp_RenderState()
{

	
	return S_OK;
}

CUI_Logo00 * CUI_Logo00::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Logo00*	pInstance = new CUI_Logo00(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo00::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Logo00::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Logo00(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo00::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo00::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
