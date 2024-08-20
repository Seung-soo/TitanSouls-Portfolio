#include "stdafx.h"
#include "Respawn02_IceWall.h"
#include "GameInstance.h"

CRespawn02_IceWall::CRespawn02_IceWall(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CRespawn02_IceWall::CRespawn02_IceWall(const CRespawn02_IceWall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRespawn02_IceWall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CRespawn02_IceWall::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRespawn02_IceWall::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CRespawn02_IceWall::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CRespawn02_IceWall::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(32, 32, 0));

	return S_OK;
}

_int CRespawn02_IceWall::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CRespawn02_IceWall::Tick");
		return -1;
	}

	return _int();
}

_int CRespawn02_IceWall::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CRespawn02_IceWall::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CRespawn02_IceWall::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CRespawn02_IceWall::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CRespawn02_IceWall::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CRespawn02_IceWall::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CRespawn02_IceWall::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CRespawn02_IceWall::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRespawn02_IceWall::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CRespawn02_IceWall::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CRespawn02_IceWall::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CRespawn02_IceWall::SetUp_Components(VIBuffer_Terrain)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_RESPAWN2, TEXT("Prototype_Component_Texture_IceWall_Respawn02"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CRespawn02_IceWall::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRespawn02_IceWall::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CRespawn02_IceWall::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CRespawn02_IceWall::Release_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CRespawn02_IceWall * CRespawn02_IceWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRespawn02_IceWall*	pInstance = new CRespawn02_IceWall(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Field01_Grass in CRespawn02_IceWall::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRespawn02_IceWall::Clone(void * pArg)
{
	CRespawn02_IceWall*	pInstance = new CRespawn02_IceWall(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Field01_Grass in CRespawn02_IceWall::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRespawn02_IceWall::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
