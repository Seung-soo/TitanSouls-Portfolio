#include "stdafx.h"
#include "Yeti01_Ice03.h"
#include "GameInstance.h"

CYeti01_Ice03::CYeti01_Ice03(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CYeti01_Ice03::CYeti01_Ice03(const CYeti01_Ice03 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CYeti01_Ice03::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CYeti01_Ice03::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CYeti01_Ice03::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CYeti01_Ice03::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CYeti01_Ice03::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(12.f, 10.f, 0));

	return S_OK;
}

_int CYeti01_Ice03::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CYeti01_Ice03::Tick");
		return -1;
	}

	return _int();
}

_int CYeti01_Ice03::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CYeti01_Ice03::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CYeti01_Ice03::LateTick");
		return -1;
	}
	m_pTransformCom->Scaled(_float3(12.f, 10.f, 0));

	// 바라보게 ++
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);
	// ++

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CYeti01_Ice03::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CYeti01_Ice03::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CYeti01_Ice03::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CYeti01_Ice03::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CYeti01_Ice03::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CYeti01_Ice03::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti01_Ice03::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti01_Ice03::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti01_Ice03::SetUp_Components(VIBuffer_Terrain)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_YETI, TEXT("Prototype_Component_Texture_Ice03_Yeti01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CYeti01_Ice03::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CYeti01_Ice03::SetUp_RenderState()
{
	//알파 테스팅
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CYeti01_Ice03::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CYeti01_Ice03::Release_RenderState");
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

CYeti01_Ice03 * CYeti01_Ice03::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CYeti01_Ice03*	pInstance = new CYeti01_Ice03(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Field01_Grass_Down in CYeti01_Ice03::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CYeti01_Ice03::Clone(void * pArg)
{
	CYeti01_Ice03*	pInstance = new CYeti01_Ice03(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Field01_Grass_Down in CYeti01_Ice03::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CYeti01_Ice03::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
