#include "..\public\Collider_Sphere.h"

CCollider_Sphere::CCollider_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere & rhs)
	: CCollider(rhs)
{
}

HRESULT CCollider_Sphere::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CCollider_Sphere::NativeConstruct_Prototype");
		return E_FAIL;
	}

	ZeroMemory(&m_tSphereDesc, sizeof(SPHEREDESC));
	return S_OK;
}

HRESULT CCollider_Sphere::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CCollider_Sphere::NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
		memcpy(&m_tSphereDesc, (SPHEREDESC*)pArg, sizeof(SPHEREDESC));

	//m_vPosition = *(_float3*)m_tSphereDesc.ObjectMatrix.m[3] + m_tSphereDesc.fDist_From_Obj;
	D3DXVec3TransformCoord(&m_vPosition, &(m_tSphereDesc.fDist_From_Obj), &m_tSphereDesc.ObjectMatrix);
	return S_OK;
}

_int CCollider_Sphere::Tick()
{
	//D3DXVec3Normalize((_float3*)m_tSphereDesc.ObjectMatrix.m[0], (_float3*)m_tSphereDesc.ObjectMatrix.m[0]);
	//D3DXVec3Normalize((_float3*)m_tSphereDesc.ObjectMatrix.m[1], (_float3*)m_tSphereDesc.ObjectMatrix.m[1]);
	//D3DXVec3Normalize((_float3*)m_tSphereDesc.ObjectMatrix.m[2], (_float3*)m_tSphereDesc.ObjectMatrix.m[2]);

	D3DXVec3TransformCoord(&m_vPosition, &(m_tSphereDesc.fDist_From_Obj), &m_tSphereDesc.ObjectMatrix);
	return _int();
}

CCollider_Sphere * CCollider_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Sphere*	pInstance = new CCollider_Sphere(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Collider_Sphere in CCollider_Sphere::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Sphere::Clone(void * pArg)
{
	CCollider_Sphere*	pInstance = new CCollider_Sphere(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Collider_Sphere in CCollider_Sphere::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Sphere::Free()
{
	__super::Free();
}
