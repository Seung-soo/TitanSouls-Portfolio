#include "stdafx.h"
#include "ArrowLight.h"
#include "GameInstance.h"
#include "Arrow.h"

CArrowLight::CArrowLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{
}

CArrowLight::CArrowLight(const CArrowLight & rhs)
	: CEffect(rhs)
{
}

HRESULT CArrowLight::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CArrowLight::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CArrowLight::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CArrowLight::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CArrowLight::NativeConstruct");
		return E_FAIL;
	}
	m_tFrame.iFrameStartX = 0.f;
	m_tFrame.iFrameCurrentX = 0.f;
	m_tFrame.iFrameEndX = 4.f;
	m_tFrame.iFrameHeight = 256.f;
	m_tFrame.iFrameWidth = 256.f;
	m_tFrame.iFrameStartY = 0.f;
	m_tFrame.iTexHeight = 1536.f;
	m_tFrame.iTexWidth = 1280.f;

	m_pArrow = (CArrow*)pArg;
	m_fSpriteSpeed = 5.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pArrow->Get_TransForm()->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Scaled(_float3(2.f, 2.f, 2.f));
	return S_OK;
}

_int CArrowLight::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CArrowLight::Tick");
		return -1;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_Size(LEVEL_STATIC, TEXT("Layer_Arrow")) == 0)
	{
		RELEASE_INSTANCE(CGameInstance);
		return -1;
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pArrow->Get_TransForm()->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * 0.01f);
	RELEASE_INSTANCE(CGameInstance);

	if (m_pArrow->Get_FullCharging() == true)
	{
		m_fSpriteSpeed = 10.f;
		m_pTransformCom->Scaled(_float3(4.f, 4.f, 4.f));
	}
	
	return _int();
}

_int CArrowLight::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CArrowLight::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CArrowLight::LateTick");
		return -1;
	}

	_float4x4		ViewMatrix; //뷰행렬을 저장할 변수

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//월드변환행렬로 만든다
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//라업룩에 적용해준다

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);

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


	Go_FrameLight(m_fSpriteSpeed, fTimeDelta);
	return _int();
}

HRESULT CArrowLight::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CArrowLight::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CArrowLight::Render");
		return E_FAIL;
	}


	if (FAILED(Go_Sprites()))
	{
		MSGBOX("Go_Sprites returned E_FAIL in CArrowLight::Tick");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice returned E_FAIL in CArrowLight::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CArrowLight::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CArrowLight::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CArrowLight::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = PLAYER_MOVESPEED;
	TransformDesc.fRotationPerSec = D3DXToRadian(PLAYER_ROTATIONSPEED);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Transform)");
		return E_FAIL;
	}


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ArrowLight"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Texture)");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CArrowLight::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CArrowLight::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	return S_OK;
}

HRESULT CArrowLight::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CArrowLight::Release_RenderState");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

HRESULT CArrowLight::Go_Sprites()
{
	if (FAILED(m_pVIBufferCom->Go_Sprites(m_tFrame)))
	{
		MSGBOX("m_pVIBufferCom->Go_Sprites returned E_FAIL in CPlayer::Go_Sprites");
		return E_FAIL;
	}
	return S_OK;
}

void CArrowLight::Go_FrameLight(_float fSpeed, _float fTimeDelta)
{
	m_tFrame.iFrameCurrentX += (m_tFrame.iFrameEndX - m_tFrame.iFrameStartX + 1) * fTimeDelta * fSpeed;
	if (m_tFrame.iFrameCurrentX >= (m_tFrame.iFrameEndX + 1))
	{
		m_tFrame.iFrameCurrentX = m_tFrame.iFrameStartX;
		m_tFrame.iFrameStartY++;
		if (m_tFrame.iFrameStartY > 5)
			m_tFrame.iFrameStartY = 0;
	}
}

CArrowLight * CArrowLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArrowLight*	pInstance = new CArrowLight(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CArrowLight in CArrowLight::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CArrowLight::Clone(void * pArg)
{
	CGameObject*	pInstance = new CArrowLight(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CArrowLight in CArrowLight::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArrowLight::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
