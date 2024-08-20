#include "stdafx.h"
#include "..\public\UI_Logo01.h"
#include "GameInstance.h"
#include "Level_Loading.h"
CUI_Logo01::CUI_Logo01(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CAlphaUI(pGraphic_Device)
{
}

CUI_Logo01::CUI_Logo01(const CUI_Logo01 & rhs)
	: CAlphaUI(rhs)
{
}

HRESULT CUI_Logo01::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Logo01::NativeConstruct(void * pArg)
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

_int CUI_Logo01::Tick(_float fTimeDelta)
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
			if (0 >= m_iAlpha){
				m_bEnd = true;
				m_bPlay = false;
			}
		}
	}

	
	return __super::Tick(fTimeDelta);
}

_int CUI_Logo01::LateTick(_float fTimeDelta)
{

	return __super::LateTick(fTimeDelta);
}

HRESULT CUI_Logo01::Render()
{
	m_pVIBufferCom->Base_Setting();
	
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	return __super::Render();
}
HRESULT CUI_Logo01::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_LogoUI01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		return E_FAIL;
	}
	return S_OK;
}
HRESULT CUI_Logo01::SetUp_RenderState()
{

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, (0x00ffffff) + (alpha << 24));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
	/*
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//그릴려고 하는 픽셀의 색상을 섞을때 소스 칼라의 알파를 섞겠다
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));



	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);
	*/
	return S_OK;
}

CUI_Logo01 * CUI_Logo01::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Logo01*	pInstance = new CUI_Logo01(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo01::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Logo01::Clone(void * pArg)
{
	CGameObject*	pInstance = new CUI_Logo01(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Logo in CUI_Logo01::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Logo01::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
