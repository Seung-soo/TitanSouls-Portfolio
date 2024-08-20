#include "..\public\AlphaUI.h"
#include "GameInstance.h"

CAlphaUI::CAlphaUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CAlphaUI::CAlphaUI(const CAlphaUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAlphaUI::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CAlphaUI::NativeConstruct(void * pArg)
{
	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)ViewPortDesc.Width, (_float)ViewPortDesc.Height, 0.0f, 1.f);

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - ViewPortDesc.Width * 0.5f, -m_fY + ViewPortDesc.Height * 0.5f, 0.f));

	return __super::NativeConstruct(pArg);
}

_int CAlphaUI::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CAlphaUI::Tick");
		return -1;
	}

	return _int();
}

_int CAlphaUI::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CAlphaUI::LateTick");
		return -1;
	}

	////////////알파소팅Z값 주기///////////
	//_float4x4 WorldMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &WorldMatrix);
	//D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);
	//_float3 vRight;
	//_float3 vUp = _float3(0.f, 1.f, 0.f);
	//_float3 vLook = *(_float3*)WorldMatrix.m[2];
	//vLook.y = 0.f;
	//D3DXVec3Normalize(&vLook, &vLook);

	//memcpy(&WorldMatrix.m[1][0], &vUp, sizeof(_float3));
	//memcpy(&WorldMatrix.m[2][0], &vLook, sizeof(_float3));
	//D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);

	//_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//D3DXVec3TransformCoord(&vPos, &vPos, &WorldMatrix);
	//m_fAlphaZ = vPos.z;
	/////////////////////////////////////

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return __super::LateTick(fTimeDelta);
}

HRESULT CAlphaUI::Render()
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

HRESULT CAlphaUI::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** pOut, void * pArg)
{
	if (FAILED(__super::Add_Component(iLevelIndex, pPrototypeTag, pComponentTag, pOut, pArg)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CUI::Add_Component");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CAlphaUI::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);


	return S_OK;
}

HRESULT CAlphaUI::Release_RenderState()
{
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, FALSE);

	return S_OK;
}

void CAlphaUI::Free()
{
	__super::Free();

}
