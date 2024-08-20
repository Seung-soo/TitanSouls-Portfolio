#include "stdafx.h"
#include "..\Public\ColossusFist_Left_Shadow.h"
#include "GameInstance.h"

CColossusFist_Left_Shadow::CColossusFist_Left_Shadow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CColossusFist_Left_Shadow::CColossusFist_Left_Shadow(const CColossusFist_Left_Shadow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CColossusFist_Left_Shadow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColossusFist_Left_Shadow::NativeConstruct(void * pArg)
{
	if (true == g_bColossusClear)
		return -1;
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	/*CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* m_pYetiTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_YETI, TEXT("Layer_Yeti"), TEXT("Com_Transform"));*/
	//_float3 YetiPos = m_pYetiTransform->Get_State(CTransform::STATE_POSITION);
	m_pColossus_Fist = (CColossus_Fist*)pArg;
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));

	//YetiPos.y += 2.f;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, YetiPos);
	//m_pTransformCom->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CColossusFist_Left_Shadow::Tick(_float fTimeDelta)
{
	if (true == g_bColossusClear)
		return -1;
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//CTransform* m_pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));

	m_fFrame += 90.0f * fTimeDelta; // 1�ʿ� ��� �������� ������ŭ ����� �ٲ۴�

	if (m_fFrame >= 90.0f) // ���� �������� �� ���Ҵٸ� ó������ �ǵ�����
		m_fFrame = 0.f;


	_float3 ShadowPos = m_pColossus_Fist->Get_TransForm()->Get_State(CTransform::STATE_POSITION);
	ShadowPos.y = 0.01f;
	m_fFistPos = m_pColossus_Fist->Get_TransForm()->Get_State(CTransform::STATE_POSITION);
	_float lengh = D3DXVec3Length(&(m_fFistPos - ShadowPos));
	m_pTransformCom->Scaled(_float3(m_fScale / (lengh * 0.5f), m_fScale / (lengh * 0.5f), m_fScale / (lengh * 0.5f)));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, ShadowPos);


	/*_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vpos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);*/

	//m_pTransformCom->Go_Straight(fTimeDelta * 7.f);




	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CColossusFist_Left_Shadow::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CColossusFist_Left_Shadow::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CColossusFist_Left_Shadow::LateTick");
		return -1;
	}

	/*if (FAILED(SetUp_OnTerrain()))
	{
	MSGBOX("SetUp_OnTerrain returned E_FAIL in CColossusFist_Left_Shadow::LateTick");
	return -1;
	}*/

	/*if (FAILED(SetUp_OnTerrain()))
	return -1;*/
	_float4x4		ViewMatrix; //������� ������ ����

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//���庯ȯ��ķ� �����
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//����迡 �������ش�
	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);
	//_float3 ShadowPos = m_pYeti->Get_TransForm()->Get_State(CTransform::STATE_POSITION);

	//_float lengh = D3DXVec3Length(&(m_fYetiPos - ShadowPos));

	//m_pTransformCom->Scaled(_float3(m_fScale / (lengh * 0.5f), m_fScale / (lengh * 0.5f), m_fScale / (lengh * 0.5f)));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CColossusFist_Left_Shadow::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CColossusFist_Left_Shadow::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CColossusFist_Left_Shadow::Render");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CColossusFist_Left_Shadow::Render");
		return E_FAIL;
	}


	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CColossusFist_Left_Shadow::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CYeti::Render");
		return E_FAIL;
	}
	////////////////////
	return S_OK;
}

HRESULT CColossusFist_Left_Shadow::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossusFist_Left_Shadow::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossusFist_Left_Shadow::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Z"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CColossusFist_Left_Shadow::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_COLOSUSS, TEXT("Prototype_Component_Texture_Colossus_Fist_Shadow"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CColossusFist_Left_Shadow::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));


	if (m_pVIBuffer_Terrain == nullptr)
		return E_FAIL;


	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition);
	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			m_bJump = false;
			m_fTime = 0;
		}
	}
	if (m_bJump == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CColossusFist_Left_Shadow::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// �����׽�Ʈ�� �ϰڴ�
																   //�ش������� ���ؼ� �񱳸� �� ����
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*ũ�ٸ�*/);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, TRUE);


	return S_OK;
}

HRESULT CColossusFist_Left_Shadow::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CColossusFist_Left_Shadow * CColossusFist_Left_Shadow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CColossusFist_Left_Shadow*	pInstance = new CColossusFist_Left_Shadow(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CColossusFist_Left_Shadow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CColossusFist_Left_Shadow::Clone(void* pArg)
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CColossusFist_Left_Shadow*	pInstance = new CColossusFist_Left_Shadow(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CColossusFist_Left_Shadow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColossusFist_Left_Shadow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}