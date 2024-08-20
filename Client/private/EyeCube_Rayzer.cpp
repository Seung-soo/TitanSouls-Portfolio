#include "stdafx.h"
#include "..\Public\EyeCube_Rayzer.h"
#include "GameInstance.h"
#include "VIBuffer_Cube_Rayzer.h"
#include "Player.h"

CEyeCube_Rayzer::CEyeCube_Rayzer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEyeCube_Rayzer::CEyeCube_Rayzer(const CEyeCube_Rayzer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEyeCube_Rayzer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CEyeCube_Rayzer::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEyeCube_Rayzer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CEyeCube_Rayzer::NativeConstruct");
		return E_FAIL;
	}

	// 컴포넌트 세팅
	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CEyeCube_Rayzer::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform* m_pEyeCubeTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Transform"));
	_float3 EyeCubePos = m_pEyeCubeTransform->Get_State(CTransform::STATE_POSITION);
	_float3 EyeLook = m_pEyeCubeTransform->Get_State(CTransform::STATE_LOOK);
	EyeCubePos += (EyeLook * 5);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, EyeCubePos);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, EyeLook);
	m_pTransformCom->LookAt(m_pEyeCubeTransform->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CEyeCube_Rayzer::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
	{
		MSGBOX("0 > __super::Tick in CEyeCube_Rayzer::Tick");
		return -1;
	}
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* m_pEyeCubeTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Transform"));
	_float3 EyePos = m_pEyeCubeTransform->Get_State(CTransform::STATE_POSITION);
	_float3 EyeLook = m_pEyeCubeTransform->Get_State(CTransform::STATE_LOOK);
	EyePos += (EyeLook * 5);
	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, EyePos);
	m_fCount++;

	if (m_fCount < 10) {
		m_fScale += 0.1f;
	}
	else if (m_fCount <= 40) {
		m_fScale -= 0.1f;
	}
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale,2.f));


	if (m_fCount > 40)
	{
	RELEASE_INSTANCE(CGameInstance);

		return -1;
	}
	RELEASE_INSTANCE(CGameInstance);


	return _int();
}

_int CEyeCube_Rayzer::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CEyeCube_Rayzer::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CEyeCube_Rayzer::LateTick");
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
	m_fAlphaZ = vPos.z;
	///////////////////////////////////
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	Collision();
	return _int();
}

HRESULT CEyeCube_Rayzer::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CEyeCube_Rayzer::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CEyeCube_Rayzer::Render");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CEyeCube_Rayzer::Render");
		return E_FAIL;
	}
	if(m_fCount % 2 == 0)
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEyeCube_Rayzer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Rayzer::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Rayzer::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_VIBuffer_Cube_Rayzer"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Rayzer::SetUp_Components(VIBuffer_Cube_Rayzer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_EYECUBE, TEXT("Prototype_Component_Texture_EyeCube_Rayzer"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CEyeCube_Rayzer::SetUp_Components(Texture)");
		return E_FAIL;
	}

	return S_OK;
}





HRESULT CEyeCube_Rayzer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CEyeCube_Rayzer::SetUp_RenderState");
		return E_FAIL;
	}

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//알파블렌딩을 하겠다
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//색을 더한다
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



	return S_OK;
}

HRESULT CEyeCube_Rayzer::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CEyeCube_Rayzer::Release_RenderState");
		return E_FAIL;
	}
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

void CEyeCube_Rayzer::Collision()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vPos = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_EYECUBE, TEXT("Layer_EyeCube"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	_float fMaxLook = vLook.x;
	_int iNumLook = 0;
	if (fabsf(fMaxLook) < fabsf(vLook.y))
	{
		fMaxLook = vLook.y;
		iNumLook = 1;
	}
	if (fabsf(fMaxLook) < fabsf(vLook.z))
	{
		fMaxLook = vLook.z;
		iNumLook = 2;
	}
	_float3 pPlayerPos = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Front(LEVEL_STATIC, TEXT("Layer_Player"));
	switch (iNumLook)
	{
	case 0:
		if (fMaxLook > 0)
		{
			if (vPos.x > pPlayerPos.x)
				if (vPos.y - 2 < pPlayerPos.y && vPos.y + 2 > pPlayerPos.y)
					if (vPos.z - 1 < pPlayerPos.z && vPos.z + 1 > pPlayerPos.z)
						pPlayer->Set_Die(true);
		}
		else
		{
			if (vPos.x <= pPlayerPos.x)
				if (vPos.y - 2 < pPlayerPos.y && vPos.y + 2 > pPlayerPos.y)
					if (vPos.z - 1 < pPlayerPos.z && vPos.z + 1 > pPlayerPos.z)
						pPlayer->Set_Die(true);
		}
		break;
	case 1:
		if (fMaxLook > 0)
		{
			if (vPos.y > pPlayerPos.y)
				if (vPos.x - 1 < pPlayerPos.x && vPos.x + 1 > pPlayerPos.x)
					if (vPos.z - 1 < pPlayerPos.z && vPos.z + 1 > pPlayerPos.z)
						pPlayer->Set_Die(true);
		}
		else
		{
			if (vPos.y <= pPlayerPos.y)
				if (vPos.x - 1 < pPlayerPos.x && vPos.x + 1 > pPlayerPos.x)
					if (vPos.z - 1 < pPlayerPos.z && vPos.z + 1 > pPlayerPos.z)
						pPlayer->Set_Die(true);
		}
		break;
	case 2:
		if (fMaxLook > 0)
		{
			if (vPos.z > pPlayerPos.z)
				if (vPos.y - 2 < pPlayerPos.y && vPos.y + 2 > pPlayerPos.y)
					if (vPos.x - 1 < pPlayerPos.x && vPos.x + 1 > pPlayerPos.x)
						pPlayer->Set_Die(true);
		}
		else
		{
			if (vPos.z <= pPlayerPos.z)
				if (vPos.y - 2 < pPlayerPos.y && vPos.y + 2 > pPlayerPos.y)
					if (vPos.x - 1 < pPlayerPos.x && vPos.x + 1 > pPlayerPos.x)
						pPlayer->Set_Die(true);
		}
		break;
	}
	RELEASE_INSTANCE(CGameInstance);

}

CEyeCube_Rayzer * CEyeCube_Rayzer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEyeCube_Rayzer*	pInstance = new CEyeCube_Rayzer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CEyeCube_Rayzer in CEyeCube_Rayzer::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEyeCube_Rayzer::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CEyeCube_Rayzer*	pInstance = new CEyeCube_Rayzer(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CEyeCube_Rayzer in CEyeCube_Rayzer::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}



void CEyeCube_Rayzer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
