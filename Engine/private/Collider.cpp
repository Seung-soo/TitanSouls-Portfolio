#include "..\public\Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
{
}

HRESULT CCollider::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CCollider::Free()
{
	__super::Free();
}
