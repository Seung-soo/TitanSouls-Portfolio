#include "stdafx.h"
#include "..\Public\Fire_Button.h"
#include "GameInstance.h"

CFire_Button::CFire_Button(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CFire_Button::CFire_Button(const CFire_Button & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFire_Button::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CFire_Button::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CFire_Button::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CFire_Button::NativeConstruct");
		return E_FAIL;
	}
	//iRandomNum = rand() % 1000;

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CFire_Button::NativeConstruct");
		return E_FAIL;
	}

	m_buttoninfo = (BUTTONINFO*)pArg;
	m_buttoninfo->bOnButton = false;
	switch (m_buttoninfo->iButtonNum)
	{
	case 0:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.0001f, 5.1f));
		break;
	case 1:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(27.f, 0.0001f, 5.1f));

		break;
	case 2:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.0001f, 27.1f));

		break;
	case 3:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(27.f, 0.0001f, 27.1f));

		break;
	default:
		break;
	}
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	//m_pTransformCom->Set_State(_)

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_buttoninfo);
	//_float3* vpPos = (_float3*)pArg;
	////m_SlimeInfo = (SLIMEINFO*)pArg;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vpPos);

	return S_OK;
}

_int CFire_Button::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CFire_Button::Tick");
		return -1;
	}
	m_fFrame = 0;

	return _int();
}

_int CFire_Button::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CFire_Button::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CFire_Button::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CFire_Button::LateTick");
		return -1;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.4f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.2f, 0.f);
	m_pColliderCom->Set_SphereDesc(SphereDesc);
	m_pColliderCom->Tick();

	//__super::Compute_CamDistance(m_pTransformCom);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	m_fAlphaZ = -m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	Collision();

	return _int();
}

HRESULT CFire_Button::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CFire_Button::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CFire_Button::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CFire_Button::Render");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CFire_Button::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CFire_Button::Render");
		return E_FAIL;
	}

//#ifdef _DEBUG
//	CCollider_Sphere::SPHEREDESC tSphereDesc = m_pColliderCom->Get_SphereDesc();
//	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
//	m_pGraphic_Device->SetTexture(0, FALSE);
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pVIBufferCollCom->Render();
//	m_pVIBufferCollCom->BaseSetting();
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//#endif // _DEBUG

	return S_OK;
}

HRESULT CFire_Button::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CFire_Button::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CFire_Button::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CFire_Button::SetUp_Components(VIBuffer_RectZ)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_ICEBRAIN, TEXT("Prototype_Component_Texture_FireButton"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CFire_Button::SetUp_Components(Texture)");
		return E_FAIL;
	}

	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.4f;
	SphereDesc.fDist_From_Obj = _float3(0.f, -0.2f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(Collider_Sphere)");
		return E_FAIL;
	}

	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}


HRESULT CFire_Button::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CFire_Button::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	//vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + iRandomNum * 0.0002f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CFire_Button::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CFire_Button::SetUp_RenderState");
		return E_FAIL;
	}

	//알파 테스팅
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);



	return S_OK;
}

HRESULT CFire_Button::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CFire_Button::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pVIBufferCom->Base_Setting();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

void CFire_Button::Collision()
{

}

CFire_Button * CFire_Button::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFire_Button*	pInstance = new CFire_Button(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime_Water in CFire_Button::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFire_Button::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CFire_Button*	pInstance = new CFire_Button(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime_Water in CFire_Button::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CFire_Button::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCollCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
