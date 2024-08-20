#include "stdafx.h"
#include "EyeCube_Ceiling.h"
#include "GameInstance.h"
#include "VIBuffer_Wall.h"

CEyeCube_Ceiling::CEyeCube_Ceiling(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEyeCube_Ceiling::CEyeCube_Ceiling(const CEyeCube_Ceiling & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEyeCube_Ceiling::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEyeCube_Ceiling::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEyeCube_Ceiling::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEyeCube_Ceiling::NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEyeCube_Ceiling::NativeConstruct");
		return E_FAIL;
	}
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;
	m_pColliderCom->Set_RectDesc(RectDesc);

	return S_OK;
}

_int CEyeCube_Ceiling::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEyeCube_Ceiling::Tick");
		return -1;
	}

	return _int();
}

_int CEyeCube_Ceiling::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEyeCube_Ceiling::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CEyeCube_Ceiling::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CEyeCube_Ceiling::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CEyeCube_Ceiling::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CEyeCube_Ceiling::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CEyeCube_Ceiling::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Ceiling::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Ceiling::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Wall1_EyeCube01"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Ceiling::SetUp_Components(VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Terrain_EyeCube01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Ceiling::SetUp_Components(VIBuffer_Texture)");
		return E_FAIL;
	}

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Ceiling::SetUp_Components(Collider_Rect)");
		return E_FAIL;
	}

	return S_OK;
}

CEyeCube_Ceiling * CEyeCube_Ceiling::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEyeCube_Ceiling*	pInstance = new CEyeCube_Ceiling(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CWall in CEyeCube_Ceiling::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEyeCube_Ceiling::Clone(void * pArg)
{
	CEyeCube_Ceiling*	pInstance = new CEyeCube_Ceiling(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CWall in CEyeCube_Ceiling::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEyeCube_Ceiling::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
