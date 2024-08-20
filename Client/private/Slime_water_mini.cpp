#include "stdafx.h"
#include "..\Public\Slime_water_mini.h"
#include "GameInstance.h"
#include <time.h>

CSlime_water_mini::CSlime_water_mini(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CSlime_water_mini::CSlime_water_mini(const CSlime_water_mini & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSlime_water_mini::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CSlime_water_mini::NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSlime_water_mini::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CSlime_water_mini::NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("SetUp_Components returned E_FAIL in CSlime_water_mini::NativeConstruct");
		return E_FAIL;
	}
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//srand(time(NULL));

	m_pSlime = (CSlime_Boss*)pArg;
	CTransform* a = m_pSlime->Get_TransForm();
	_float3 d = a->Get_State(CTransform::STATE_POSITION);
	//if(d.y)
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, a->Get_State(CTransform::STATE_POSITION));
	_float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	int b = rand() % 2;
	int c = rand() % 4;
	switch (c)
	{
	case 0:
		m_pTransformCom->LookAtXZ(_float3(pos.x - b, 0, pos.z + b));
		break;
	case 1:
		m_pTransformCom->LookAtXZ(_float3(pos.x + b, 0, pos.z + b));
		break;
	case 2:
		m_pTransformCom->LookAtXZ(_float3(pos.x - b, 0, pos.z - b));
		break;
	case 3:
		m_pTransformCom->LookAtXZ(_float3(pos.x + b, 0, pos.z - b));
		break;
	}


	/*if (pos.y < m_fScale * 0.5f) {
	pos.y = 0 + m_fScale + 0.1f;
	}
	*/
	//pos.y += 5.f;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, pos);
	m_pTransformCom->Scaled(_float3(m_fScale, m_fScale, m_fScale));
	//pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Shadow"), TEXT("Prototype_GameObject_Slime_Shadow"), this);
	//m_bJump = true;
	//m_pBossDead = (_bool*)pArg;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



_int CSlime_water_mini::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return -1;
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	m_pTransformCom->Go_Straight(fTimeDelta * 0.4f);

	if (m_bJump == false) {
		m_fJumpy = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
		m_bJump = true;
	}

	_float3 MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vpos.y = MyPos.y;
	if (m_bJump == true) {
		//m_pTransformCom->LookAt(vpos);
		MyPos.y = m_fJumpy + (m_fJumpPower * m_fTime - 5.8f * m_fTime * m_fTime * 0.5f);
		m_fTime += 0.025f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);
		m_pTransformCom->Go_Straight(fTimeDelta * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();

}

_int CSlime_water_mini::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	{
		MSGBOX("0 > __super::LateTick in CSlime_water_mini::LateTick");
		return -1;
	}

	if (nullptr == m_pRendererCom)
	{
		MSGBOX("m_pRendererCom is nullptr in CSlime_water_mini::LateTick");
		return -1;
	}

	if (FAILED(SetUp_OnTerrain()))
	{
		MSGBOX("SetUp_OnTerrain returned E_FAIL in CSlime_water_mini::LateTick");
		return -1;
	}
	CCollider_Sphere::SPHEREDESC SphereDesc;
	SphereDesc.ObjectMatrix = m_pTransformCom->Get_WorldMatrix();
	SphereDesc.fRadius = 0.5f;
	SphereDesc.fDist_From_Obj = _float3(0.f, 0.f, 0.f);
	//m_pColliderCom->Set_SphereDesc(SphereDesc);
	//m_pColliderCom->Tick();


	_float4x4		ViewMatrix; //������� ������ ����
								//ī�޶� �佺���̽��� ����� ���ؼ� �����Ѵ�
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//���庯ȯ��ķ� �����
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	//D3DXMatrixInverse
	//����迡 �������ش�
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	//m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);





	//////////���ļ���Z�� �ֱ�///////////
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

HRESULT CSlime_water_mini::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("m_pVIBufferCom is nullptr in CSlime_water_mini::Render")
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTransformCom->Bind_OnGraphicDevice returned E_FAIL in CSlime_water_mini::Render");
		return E_FAIL;
	}
	m_pVIBufferCom->Base_Setting();
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
	{
		MSGBOX("m_pTextureCom->Bind_OnGraphicDevice in CSlime_water_mini::Render");
		return E_FAIL;
	}

	////////////////////
	if (FAILED(SetUp_RenderState()))
	{
		MSGBOX("SetUp_RenderState returned E_FAIL in CSlime_water_mini::Render");
		return E_FAIL;
	}

	m_pVIBufferCom->Render(); // �̷��� ������� ������ ������ �� ���
							  // Ư���� �κп��� ������ �����ϰų� �� �� �ִ�
	if (FAILED(Release_RenderState()))
	{
		MSGBOX("Release_RenderState returned E_FAIL in CSlime_water_mini::Render");
		return E_FAIL;
	}
	////////////////////

//#ifdef _DEBUG
//	//CCollider_Sphere::SPHEREDESC tSphereDesc = m_pColliderCom->Get_SphereDesc();
//	//	m_pVIBufferCollCom->MakeHitBox(tSphereDesc);
//	m_pGraphic_Device->SetTexture(0, FALSE);
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pVIBufferCollCom->Render();
//	m_pVIBufferCollCom->BaseSetting();
//	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//#endif // _DEBUG
	return S_OK;
}

HRESULT CSlime_water_mini::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_water_mini::SetUp_Components(Transform)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_water_mini::SetUp_Components(Renderer)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_water_mini::SetUp_Components(VIBuffer_Rect)");
		return E_FAIL;
	}

	if (FAILED(__super::Add_Component(LEVEL_SLIME, TEXT("Prototype_Component_Texture_Slime"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_water_mini::SetUp_Components(Texture)");
		return E_FAIL;
	}



	//#ifdef _DEBUG
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Coll"), (CComponent**)&m_pVIBufferCollCom)))
	{
		MSGBOX("__super::Add_Component returned E_FAIL in CSlime_water_mini::SetUp_Components(VIBuffer_Cube)");
		return E_FAIL;
	}
	//#endif // DEBUG

	return S_OK;
}

HRESULT CSlime_water_mini::SetUp_OnTerrain()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CVIBuffer_Terrain* m_pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(g_eCurrentLevel, TEXT("Layer_Terrain"), TEXT("Com_VIBuffer"));
	if (m_pVIBuffer_Terrain == nullptr)
	{
		MSGBOX("pVIBuffer_Terrain is nullptr in CSlime_Boss::SetUp_OnTerrain");
		return E_FAIL;
	}
	//�÷��̾��� ��ġ�� ���ͼ�
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = m_pVIBuffer_Terrain->Compute_Y(vPosition) + m_fScale / 2;
	if (m_bJump == true) {
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y < vPosition.y) {
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_SLIME, TEXT("Layer_Slime_Water"), TEXT("Prototype_GameObject_Slime_Water"), this)))
			{
				MSGBOX("pGameInstance->Add_GameObject returned E_FAIL in CSlime_Boss::SetUp_OnTerrain");
				return E_FAIL;
			}
			m_bDead = true;
		}

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSlime_water_mini::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_water_mini::SetUp_RenderState");
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//���ĺ����� �ϰڴ�
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//���� ���Ѵ�
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	////�׸����� �ϴ� �ȼ��� ������ ������ �ҽ� Į���� ���ĸ� ���ڴ�
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*
	_float4		vSourColor, vDestColor;
	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);
	�ҽ��÷�.RGB * �ҽ��÷�.���� + ����Ʈ�ö�.RGB * 1.f - �ҽ��÷�.����
	���Ĵ� RGB�� 0~255�� 0���� 1���̷� �����Ѱ�
	�÷��̾ �׸����ϴµ� �̹� ����ġ�� �׷��� �ִ� ���� ����ƮĮ��
	*/
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	//�ٸ� ��� - ���� �׽�Ʈ
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);// �����׽�Ʈ�� �ϰڴ�
	//															   �ش������� ���ؼ� �񱳸� �� ����
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//�׸����� �ߴ� �̹����� ���İ��� ���� ������ ���̶�
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER/*ũ�ٸ�*/);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// �� ������ ����� �ֵ鸸 ȭ�鿡 �׸��ڴ�
	// ���ڵ��� ��� ���� �׸����� ���İ�(REF - 0)���� ũ�ٸ�(GREATER) �׸���

	// �ܼ��� ����� ����µ��� �����׽����� ���� ���������� �Ѱ谡 �ִ�
	// ������ ó���� �Ұ����ϱ� ����

	//���� �׽��ð� ������ ȥ������ �ʴ´� -> �������ѵ� �ǹ̰� �����
	return S_OK;
}

HRESULT CSlime_water_mini::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("m_pGraphic_Device is nullptr in CSlime_water_mini::Release_RenderState");
		return E_FAIL;
	}
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CSlime_water_mini * CSlime_water_mini::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlime_water_mini*	pInstance = new CSlime_water_mini(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Slime in CSlime_water_mini::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime_water_mini::Clone(void* pArg)
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CSlime_water_mini*	pInstance = new CSlime_water_mini(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Slime in CSlime_water_mini::Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSlime_water_mini::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCollCom);
	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}