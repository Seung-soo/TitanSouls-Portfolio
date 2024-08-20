#include "stdafx.h"
#include "..\public\LightLeak.h"
#include "GameInstance.h"
#include "Effect_LightLeak.h"

CLightLeak::CLightLeak(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLightLeak::CLightLeak(const CLightLeak & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLightLeak::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CLightLeak::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLightLeak::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CLightLeak::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CLightLeak::NativeConstruct");
		return E_FAIL;
	}
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(16.f, 0.f, 5.f));
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_vAxis = static_cast<CTransform*>(pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Effect"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK);

	//m_pTransformCom->Set_WorldMatrix(static_cast<CTransform*>(pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Effect"), TEXT("Com_Transform")))->Get_WorldMatrix());
	D3DXVec3Normalize(&m_vAxis, &m_vAxis);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_vAxis * m_pTransformCom->Get_Scale().z);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(0.f, 0.1f, 0.f) * m_pTransformCom->Get_Scale().y);
	//_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 0.1f, 0.f), &m_vAxis);
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight* m_pTransformCom->Get_Scale().x);
	m_pTransformCom->Rotation(m_vAxis, D3DXToRadian((*(POSANGLE*)pArg).fAngle));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, (*(POSANGLE*)pArg).vPos);
	_float3 vCameraLook = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_LOOK);
	_float3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	_float3 vRight = *D3DXVec3Cross(&vRight, &vUp, &vCameraLook);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vCameraLook);
	if (0 == static_cast<CEffect_LightLeak*>(pGameInstance->Get_Front(g_eCurrentLevel, TEXT("Layer_Effect")))->Get_Dir() % 2)
		m_iDir = 1;
	else
		m_iDir = -1;

	//CTransform* pCameraTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Camera"), TEXT("Com_Transform"));
	//_float3 vCameraPos = pCameraTrans->Get_State(CTransform::STATE_POSITION);
	//m_pTransformCom->MatchIt_XZ(vCameraPos);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLightLeak::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CLightLeak::Tick");
		return -1;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Turn(m_vAxis, fTimeDelta * m_iDir);
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CLightLeak::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CLightLeak::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CLightLeak::LateTick");
		return -1;
	}

	//_float4x4		ViewMatrix; //뷰행렬을 저장할 변수

	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	////월드변환행렬로 만든다
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	////D3DXMatrixInverse
	////라업룩에 적용해준다

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);

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


	return _int();
}

HRESULT CLightLeak::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CLightLeak::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CLightLeak::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CLightLeak::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CLightLeak::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLightLeak::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Color"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLightLeak::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CLightLeak::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetTexture(0, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CLightLeak::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CLightLeak::Release_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

CLightLeak * CLightLeak::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLightLeak*	pInstance = new CLightLeak(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CLightLeak in CLightLeak::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CLightLeak::Clone(void * pArg)
{
	CGameObject*	pInstance = new CLightLeak(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CLightLeak in CLightLeak::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightLeak::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
