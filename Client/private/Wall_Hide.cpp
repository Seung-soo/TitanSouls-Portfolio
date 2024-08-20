#include "stdafx.h"
#include "..\public\Wall_Hide.h"
#include "GameInstance.h"
#include "VIBuffer_Wall.h"

CWall_Hide::CWall_Hide(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CWall_Hide::CWall_Hide(const CWall_Hide & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWall_Hide::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CWall_Hide::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWall_Hide::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CWall_Hide::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CWall_Hide::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian((*(POSANGLESCALE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLESCALE*)pArg).vPos);


	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = (*(POSANGLESCALE*)pArg).fScale;
	RectDesc.fY = (*(POSANGLESCALE*)pArg).fScale;
	m_pColliderCom->Set_RectDesc(RectDesc);

	return S_OK;
}

_int CWall_Hide::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CWall_Hide::Tick");
		return -1;
	}

	return _int();
}

_int CWall_Hide::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CWall_Hide::LateTick");
		return -1;
	}
	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CField01_Grass::LateTick");
		return -1;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CWall_Hide::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CField01_Grass::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CField01_Grass::Render");
		return E_FAIL;
	}
	//m_pVIBufferCom->Render();
	
	return S_OK;
}

HRESULT CWall_Hide::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWall_Hide::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWall_Hide::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_VIBuffer_Wall_Field"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWall_Hide::SetUp_Components(VIBuffer_Terrain)");
		return E_FAIL;
	}

	CCollider_Rect::RECTDESC RectDesc;
	RectDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	RectDesc.fX = 32.f;
	RectDesc.fY = 32.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &RectDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CWall::SetUp_Components(Collider_Rect)");
		return E_FAIL;
	}

	
	return S_OK;
}

CWall_Hide * CWall_Hide::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall_Hide*	pInstance = new CWall_Hide(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CWall_Hide in CField01_Grass::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWall_Hide::Clone(void * pArg)
{
	CWall_Hide*	pInstance = new CWall_Hide(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CWall_Hide in CWall_Hide::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWall_Hide::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
