#include "stdafx.h"
#include "Terrain2_Field01.h"
#include "GameInstance.h"

CTerrain2_Field01::CTerrain2_Field01(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain2_Field01::CTerrain2_Field01(const CTerrain2_Field01 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain2_Field01::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CTerrain2_Field01::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain2_Field01::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CTerrain2_Field01::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CTerrain2_Field01::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(1, 1, 1));

	return S_OK;
}

_int CTerrain2_Field01::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CTerrain2_Field01::Tick");
		return -1;
	}

	return _int();
}

_int CTerrain2_Field01::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CTerrain2_Field01::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CTerrain2_Field01::LateTick");
		return -1;
	}



	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTerrain2_Field01::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CTerrain::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CTerrain2_Field01::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice returned E_FAIL in CTerrain2_Field01::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CTerrain2_Field01::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CTerrain2_Field01::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain2_Field01::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CTerrain2_Field01::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CTerrain2_Field01::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_FIELD, TEXT("Prototype_Component_VIBuffer_Terrain2_Field"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CTerrain2_Field01::SetUp_Components(VIBuffer_Terrain)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Terrain2_Field"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CTerrain2_Field01::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain2_Field01::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CTerrain2_Field01::SetUp_RenderState");
		return E_FAIL;
	}
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xa0ffffff);

	//// 이건 알파 블랜딩 할 때 복붙해서 사용할 것
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//알파 테스팅
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CTerrain2_Field01::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CTerrain2_Field01::Release_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CTerrain2_Field01 * CTerrain2_Field01::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain2_Field01*	pInstance = new CTerrain2_Field01(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Terrain_Field01 in CTerrain2_Field01::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain2_Field01::Clone(void * pArg)
{
	CTerrain2_Field01*	pInstance = new CTerrain2_Field01(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Terrain_Field01 in CTerrain2_Field01::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain2_Field01::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
