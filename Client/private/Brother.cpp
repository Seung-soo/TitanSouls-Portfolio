#include "stdafx.h"
#include "..\public\Brother.h"
#include "GameInstance.h"

CBrother::CBrother(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBrother::CBrother(const CBrother & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBrother::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CBrother::NativeConstruct_Prototype");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CBrother::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CBrother::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CBrother::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.5f, 25.f));
	// 플레이어 크기 조절
	m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));


	return S_OK;
}

_int CBrother::Tick(_float fTimeDelta)
{


	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CBrother::Tick");
		return -1;
	}

	if (true == m_bGoAlpha)
	{
		--m_iAlpha;
		if (m_iAlpha <= 0)
		{
			g_bTimeSlow = false;
			return -1;
		}
	}

	return _int();
}

_int CBrother::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CBrother::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CBrother::LateTick");
		return -1;
	}


		if (FAILED(SetUp_OnTerrain()))
		{
			MSGBOX("SetUp_OnTerrain returned E_FAIL in CBrother::LateTick");
			return -1;
		}


	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->MatchIt_XZ(vCameraPos);

	RELEASE_INSTANCE(CGameInstance);

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
	m_fAlphaZ = vPos.z;
	///////////////////////////////////
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	Collision();

	return _int();
}

HRESULT CBrother::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CBrother::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CBrother::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CBrother::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CBrother::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CBrother::Render");
		return E_FAIL;
	}

	return S_OK;

}

HRESULT CBrother::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	TransformDesc.fRotationPerSec = D3DXToRadian(PLAYER_ROTATIONSPEED);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrother::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrother::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrother::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(g_eCurrentLevel, TEXT("Prototype_Component_Texture_Brother"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrother::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.3f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CBrother::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBrother::SetUp_OnTerrain()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);


	CVIBuffer_Terrain*	pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (nullptr == pVIBuffer_Terrain)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CBrother::SetUp_OnTerrain");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	_float3	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	CTransform* pTerrainTransform = (CTransform*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_Transform"));
	if (nullptr == pTerrainTransform)
	{
		MSGBOX("pTerrainTrasform is nullptr in CBrother::SetUp_OnTerrain");
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	_float4x4	WorldMatrixInverse = pTerrainTransform->Get_WorldMatrixInverse();

	_float3		vLocalPos;
	D3DXVec3TransformCoord(&vLocalPos, &vPosition, &WorldMatrixInverse);


	D3DXVec3TransformCoord(&vPosition, &vLocalPos, &pTerrainTransform->Get_WorldMatrix());
	vPosition.y = pVIBuffer_Terrain->Compute_Y(vLocalPos) + 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBrother::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CBrother::SetUp_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	//// 이건 알파 블랜딩 할 때 복붙해서 사용할 것
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CBrother::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CBrother::Release_RenderState");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

void CBrother::Collision()
{
}

CBrother * CBrother::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBrother*	pInstance = new CBrother(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CBrother in CBrother::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBrother::Clone(void * pArg)
{
	CBrother*	pInstance = new CBrother(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CBrother in CBrother::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBrother::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}