#include "stdafx.h"
#include "EyeCube_Wall2.h"
#include "GameInstance.h"
#include "VIBuffer_Wall.h"

CEyeCube_Wall2::CEyeCube_Wall2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEyeCube_Wall2::CEyeCube_Wall2(const CEyeCube_Wall2 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEyeCube_Wall2::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEyeCube_Wall2::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEyeCube_Wall2::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEyeCube_Wall2::NativeConstruct");
		return E_FAIL;
	}
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEyeCube_Wall2::NativeConstruct");
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

_int CEyeCube_Wall2::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEyeCube_Wall2::Tick");
		return -1;
	}

	return _int();
}

_int CEyeCube_Wall2::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEyeCube_Wall2::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CEyeCube_Wall2::LateTick");
		return -1;
	}
	//////////알파소팅Z값 주기///////////
	_float4x4 WorldMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &WorldMatrix);
	D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);
	_float3 vRight;
	_float3 vUp = _float3(0.f, 1.f, 0.f);
	_float3 vLook = *(_float3*)WorldMatrix.m[2];
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	memcpy(&WorldMatrix.m[1][0], &vUp, sizeof(_float3));
	memcpy(&WorldMatrix.m[2][0], &vLook, sizeof(_float3));
	D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrix);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	D3DXVec3TransformCoord(&vPos, &vPos, &WorldMatrix);
	m_fAlphaZ = vPos.z + 32.f;
	///////////////////////////////////
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CEyeCube_Wall2::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CEyeCube_Wall2::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CEyeCube_Wall2::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

HRESULT CEyeCube_Wall2::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Wall2::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Wall2::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Wall1_EyeCube01"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Wall2::SetUp_Components(VIBuffer_Wall)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_Wall2_EyeCube01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Wall2::SetUp_Components(VIBuffer_Texture)");
		return E_FAIL;
	}

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Wall2::SetUp_Components(Collider_Rect)");
		return E_FAIL;
	}

	return S_OK;
}

CEyeCube_Wall2 * CEyeCube_Wall2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEyeCube_Wall2*	pInstance = new CEyeCube_Wall2(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CWall in CEyeCube_Wall2::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEyeCube_Wall2::Clone(void * pArg)
{
	CEyeCube_Wall2*	pInstance = new CEyeCube_Wall2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CWall in CEyeCube_Wall2::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEyeCube_Wall2::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
