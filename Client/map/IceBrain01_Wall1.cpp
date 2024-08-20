#include "stdafx.h"
#include "IceBrain01_Wall1.h"
#include "GameInstance.h"
#include "VIBuffer_Wall.h"

CIceBrain01_Wall1::CIceBrain01_Wall1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CIceBrain01_Wall1::CIceBrain01_Wall1(const CIceBrain01_Wall1 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIceBrain01_Wall1::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CIceBrain01_Wall1::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CIceBrain01_Wall1::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CIceBrain01_Wall1::NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CIceBrain01_Wall1::NativeConstruct");
		return E_FAIL;
	}
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;
	m_pColliderCom->Set_RectDesc(RectDesc);

	return S_OK;
}

_int CIceBrain01_Wall1::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CIceBrain01_Wall1::Tick");
		return -1;
	}

	return _int();
}

_int CIceBrain01_Wall1::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CIceBrain01_Wall1::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CIceBrain01_Wall1::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CIceBrain01_Wall1::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CIceBrain01_Wall1::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CIceBrain01_Wall1::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (LEVEL_ENDING == g_eCurrentLevel)
		m_pGraphic_Device->SetTexture(0, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	m_pVIBufferCom->Render();


	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CIceBrain01_Wall1::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIceBrain01_Wall1::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIceBrain01_Wall1::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_VIBuffer_Wall1_IceBrain01"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIceBrain01_Wall1::SetUp_Components(VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_Wall1_IceBrain01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIceBrain01_Wall1::SetUp_Components(VIBuffer_Texture)");
		return E_FAIL;
	}

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CIceBrain01_Wall1::SetUp_Components(Collider_Rect)");
		return E_FAIL;
	}

	return S_OK;
}

CIceBrain01_Wall1 * CIceBrain01_Wall1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CIceBrain01_Wall1*	pInstance = new CIceBrain01_Wall1(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CWall in CIceBrain01_Wall1::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIceBrain01_Wall1::Clone(void * pArg)
{
	CIceBrain01_Wall1*	pInstance = new CIceBrain01_Wall1(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CWall in CIceBrain01_Wall1::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIceBrain01_Wall1::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
