#include "stdafx.h"
#include "Slime01_Mist.h"
#include "GameInstance.h"

CSlime01_Mist::CSlime01_Mist(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSlime01_Mist::CSlime01_Mist(const CSlime01_Mist & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSlime01_Mist::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CSlime01_Mist::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime01_Mist::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CSlime01_Mist::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CSlime01_Light::NativeConstruct");
		return E_FAIL;
	}

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	m_pTransformCom->Scaled(_float3(30, 30, 1));

	return S_OK;
}

_int CSlime01_Mist::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CSlime01_Mist::Tick");
		return -1;
	}

	m_fFrame += 1.0f * fTimeDelta;

	if (m_fFrame >= 4.0f)
		m_fFrame = 0.f;

	return _int();
}

_int CSlime01_Mist::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSlime01_Mist::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSlime01_Mist::LateTick");
		return -1;
	}

	//// 바라보게 ++
	//_float4x4		ViewMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	////m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);
	//// ++

	//////////알파소팅Z값 주기///////////

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_fAlphaZ = vPos.y + 32.f;
	///////////////////////////////////

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CSlime01_Mist::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSlime01_Mist::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice CSlime01_Mist E_FAIL in CSlime01_Mist::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CSlime01_Mist::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CSlime01_Mist::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime01_Mist::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime01_Mist::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime01_Mist::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_VIBuffer_Rect_Slime"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime01_Mist::SetUp_Components(VIBuffer_Terrain)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Mist_Slime01"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime01_Mist::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime01_Mist::SetUp_RenderState()
{
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xa0ffffff);

	// 이건 알파 블랜딩 할 때 복붙해서 사용할 것
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//알파 테스팅
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CSlime01_Mist::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime01_Mist::Release_RenderState");
		return E_FAIL;
	}

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CSlime01_Mist * CSlime01_Mist::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlime01_Mist*	pInstance = new CSlime01_Mist(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CSlime01_Mist in CSlime01_Mist::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime01_Mist::Clone(void * pArg)
{
	CSlime01_Mist*	pInstance = new CSlime01_Mist(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CSlime01_Mist in CSlime01_Mist::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlime01_Mist::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
