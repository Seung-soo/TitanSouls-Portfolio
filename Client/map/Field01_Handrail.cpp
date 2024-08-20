#include "stdafx.h"
#include "Field01_Handrail.h"
#include "GameInstance.h"

CField01_Handrail::CField01_Handrail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CField01_Handrail::CField01_Handrail(const CField01_Handrail & rhs)
	: CGameObject(rhs)
{
}

HRESULT CField01_Handrail::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CField01_Handrail::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CField01_Handrail::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CField01_Handrail::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CField01_Handrail::NativeConstruct");
		return E_FAIL;
	}

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f,16.f,16.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(3, 3, 3));

	return S_OK;
}

_int CField01_Handrail::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CField01_Handrail::Tick");
		return -1;
	}

	return _int();
}

_int CField01_Handrail::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CField01_Handrail::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CField01_Handrail::LateTick");
		return -1;
	}
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	_float4x4	ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	return _int();
}

HRESULT CField01_Handrail::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CField01_Handrail::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CField01_Handrail::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice returned E_FAIL in CField01_Handrail::Render");
		return E_FAIL;
	}


	
	//if (FAILED(SetUp_RenderState()))
	//	return E_FAIL;

	m_pVIBufferCom->Render(); 
							  
	//if (FAILED(Release_RenderState()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CField01_Handrail::SetUp_Components()
{
	CTransform::TRANSFORMDESC	TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CField01_Handrail::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CField01_Handrail::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CField01_Handrail::SetUp_Components(Cube)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_FIELD, TEXT("Prototype_Component_Texture_Handrail"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CField01_Handrail::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CField01_Handrail::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSky::SetUp_RenderState");
		return E_FAIL;
	}

	////m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	////m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	////m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CField01_Handrail::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSky::Release_RenderState");
		return E_FAIL;
	}

	////m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	////m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	////m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);


	return S_OK;
}

CField01_Handrail * CField01_Handrail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CField01_Handrail*	pInstance = new CField01_Handrail(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Field01_Handrail in CField01_Handrail::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CField01_Handrail::Clone(void * pArg)
{
	CField01_Handrail*	pInstance = new CField01_Handrail(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Field01_Handrail in CField01_Handrail::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CField01_Handrail::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
