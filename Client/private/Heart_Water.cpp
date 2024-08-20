#include "stdafx.h"
#include "..\Public\Heart_Water.h"
#include "GameInstance.h"
CHeart_Water::CHeart_Water(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CHeart_Water::CHeart_Water(const CHeart_Water & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHeart_Water::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CHeart_Water::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CHeart_Water::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CHeart_Water::NativeConstruct");
		return E_FAIL;
	}
	iRandomNum = rand() % 1000;

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CHeart_Water::NativeConstruct");
		return E_FAIL;
	}

	m_SlimeInfo = (CHeart_water_mini*)pArg;
	_float3 a = m_SlimeInfo->Get_TransForm()->Get_State(CTransform::STATE_POSITION);
	//a.y += iRandomNum;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, a);
	m_fScale = (rand() % 10) * 0.1f + 1;
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	
	//_float3* vpPos = (_float3*)pArg;
	////m_SlimeInfo = (SLIMEINFO*)pArg;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, *vpPos);

	return S_OK;
}

_int CHeart_Water::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CHeart_Water::Tick");
		return -1;
	}
	m_iDeathCout++;
	if (m_iDeathCout > 100) {
		m_fScale -= 0.01f;
		m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	}
	if (m_fScale < 0) {
		return -1;
	}

	return _int();
}

_int CHeart_Water::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CHeart_Water::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CHeart_Water::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CHeart_Water::LateTick");
		return -1;
	}

	__super::Compute_CamDistance(m_pTransformCom);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	m_fAlphaZ = -m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
	return _int();
}

HRESULT CHeart_Water::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CHeart_Water::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CHeart_Water::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CHeart_Water::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CHeart_Water::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CHeart_Water::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CHeart_Water::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CHeart_Water::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CHeart_Water::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CHeart_Water::SetUp_Components(VIBuffer_RectZ)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Heart_Water"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CHeart_Water::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}


HRESULT CHeart_Water::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CHeart_Water::SetUp_OnTerrain");
		return E_FAIL;
	}
	//플레이어의 위치를 얻어와서
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + iRandomNum * 0.0002f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CHeart_Water::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CHeart_Water::SetUp_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



	return S_OK;
}

HRESULT CHeart_Water::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CHeart_Water::Release_RenderState");
		return E_FAIL;
	}
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

CHeart_Water * CHeart_Water::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHeart_Water*	pInstance = new CHeart_Water(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime_Water in CHeart_Water::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHeart_Water::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CHeart_Water*	pInstance = new CHeart_Water(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime_Water in CHeart_Water::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CHeart_Water::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
