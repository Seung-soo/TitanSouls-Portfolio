#include "stdafx.h"
#include "..\public\Soul.h"
#include "GameInstance.h"
#include "Player.h"

CSoul::CSoul(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSoul::CSoul(const CSoul & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSoul::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CSoul::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSoul::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CSoul::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CSoul::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<CTransform*>(pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_EatSouls"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
	m_tSoulDesc = *(SOULDESC*)pArg;

	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player")));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CSoul::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CSoul::Tick");
		return -1;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObject(g_eCurrentLevel, TEXT("Layer_Soul_AfterImage"), TEXT("Prototype_GameObject_Soul_AfterImage"), this)))
	{
		MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSoul::Tick");
		return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);
	--m_iSlowTime;
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_iSlowTime > 0)
		m_pTransformCom->Revolution(m_tSoulDesc.vAxis, m_tSoulDesc.vCenterPosition, fTimeDelta);
	else
	{
		// 플레이어와 거리가 1.5가 되면 플레이어 중심으로 y축으로 공전
		_float3 vPlayerPos = static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
		if (false == m_bRevolution)
		{
			_float3 vDir = vPlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float	fLength = D3DXVec3Length(&vDir);
			if (1.5f > fLength)
			{
				m_bRevolution = true;

			}
		}

		if (true == m_bRevolution)
		{
			m_pTransformCom->Revolution(_float3(0.f, 1.f, 0.f), vPlayerPos, fTimeDelta * ((rand() % 3) + 2) * 2);
			//if (vPlayerPos.y < 5.f)
			//{
				static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")))->Go_Up(fTimeDelta * 0.02f);
				m_pTransformCom->Go_Vector(_float3(0.f, 1.f, 0.f), fTimeDelta * 0.2f / m_pTransformCom->Get_State(CTransform::STATE_POSITION).y);
//			}
			if (0.5f > D3DXVec3Length(&(vPlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION))))
				return -1;
			m_pTransformCom->LookAt(vPlayerPos);
			m_pTransformCom->Go_Straight(fTimeDelta * 0.04f);
			m_pPlayer->Set_Fly(true);
		}
		else
		{
			m_pTransformCom->Revolution(m_tSoulDesc.vAxis, m_tSoulDesc.vCenterPosition, fTimeDelta * 0.5f);
			//m_pTransformCom->LookAt(static_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION));
			//m_pTransformCom->Go_Straight(fTimeDelta);
			if ((-m_iSlowTime / 20) % 12 == 0)
				m_pTransformCom->Go_Right(fTimeDelta * 0.2f);
			else if ((-m_iSlowTime / 20) % 12 == 6)
				m_pTransformCom->Go_Left(fTimeDelta * 0.2f);

		}
	}

	//RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CSoul::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSoul::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSoul::LateTick");
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


	return _int();
}

HRESULT CSoul::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSoul::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CSoul::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice returned E_FAIL in CSoul::Render");
		return E_FAIL;
	}

	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CSoul::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CSoul::Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSoul::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Soul"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CPlayer::SetUp_Components(Texture)");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSoul::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSoul::Release_RenderState");
		return E_FAIL;
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//알파 테스팅
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 10);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CSoul::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSoul::Release_RenderState");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSoul * CSoul::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSoul*	pInstance = new CSoul(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CSoul in CSoul::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSoul::Clone(void * pArg)
{
	CGameObject*	pInstance = new CSoul(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CLightLeak in CSoul::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSoul::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
