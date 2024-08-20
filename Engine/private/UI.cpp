#include "..\public\UI.h"
#include "GameInstance.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI::NativeConstruct(void * pArg)
{
	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)ViewPortDesc.Width, (_float)ViewPortDesc.Height, 0.0f, 1.f);

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - ViewPortDesc.Width * 0.5f, -m_fY + ViewPortDesc.Height * 0.5f, 0.f));

	return __super::NativeConstruct(pArg);
}

_int CUI::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CUI::Tick");
		return -1;
	}

	return _int();
}

_int CUI::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CUI::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return __super::LateTick(fTimeDelta);
}

HRESULT CUI::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	if (FAILED(SetUp_RenderState()))
		return E_FAIL;
	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** pOut, void * pArg)
{
	if (FAILED(__super::Add_Component(iLevelIndex, pPrototypeTag, pComponentTag, pOut, pArg)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CUI::Add_Component");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI::SetUp_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);


	return S_OK;
}

HRESULT CUI::Release_RenderState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

}
