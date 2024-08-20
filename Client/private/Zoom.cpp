#include "stdafx.h"
#include "..\public\Zoom.h"
#include "GameInstance.h"
//#include "Level_Loading.h"
//#include "Player.h"

CZoom::CZoom(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CZoom::CZoom(const CZoom & rhs)
	: CUI(rhs)
{
}

HRESULT CZoom::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CZoom::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CZoom::NativeConstruct(void * pArg)
{

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;
	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f));


	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	return S_OK;
}

_int CZoom::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fFrame += 7.0f * fTimeDelta;

	if (m_fFrame >= 7.0f)
	{
		m_fFrame = 1.f;
		m_bCnt = false;
	}

	return _int();
}

_int CZoom::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CZoom::LateTick");
		return -1;
	}

	//if (nullptr == m_pRendererCom)
	//{
	//	MSGBOX("m_pRendererCom is nullptr in CZoom::LateTick");
	//	return -1;
	//}

	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CZoom::Render()
{
	if (0 > __super::Render())
	{
		MSGBOX("0 > __super::Render in CZoom::Render");
		return -1;
	}

	m_pVIBufferCom->Base_Setting();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CZoom::Render");
		return E_FAIL;
	}

	//_float4x4		ViewMatrix;
	//D3DXMatrixIdentity(&ViewMatrix);
	//m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	//m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;


	//알파 테스팅

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);


	m_pVIBufferCom->Render();


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

HRESULT CZoom::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0;
	TransformDesc.fRotationPerSec = D3DXToRadian(0.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CZoom::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CZoom::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CZoom::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Zoom"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CZoom::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CZoom * CZoom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CZoom*	pInstance = new CZoom(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Aim in CZoom::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CZoom::Clone(void * pArg)
{
	CGameObject*	pInstance = new CZoom(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CZoom in CZoom::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZoom::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
