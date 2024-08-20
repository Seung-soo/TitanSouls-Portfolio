#include "..\public\Collider_Rect_Z.h"

CCollider_Rect_Z::CCollider_Rect_Z(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CCollider_Rect_Z::CCollider_Rect_Z(const CCollider_Rect_Z & rhs)
	: CCollider(rhs)
{
}

HRESULT CCollider_Rect_Z::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CCollider_Rect_Z::NativeConstruct_Prototype");
		return E_FAIL;
	}

	ZeroMemory(&m_tRectDesc, sizeof(RECTDESC));
	return S_OK;
}

HRESULT CCollider_Rect_Z::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CCollider_Rect_Z::NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
		memcpy(&m_tRectDesc, (RECTDESC*)pArg, sizeof(RECTDESC));
	return S_OK;
}

CCollider_Rect_Z * CCollider_Rect_Z::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Rect_Z*	pInstance = new CCollider_Rect_Z(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Collider_Rect_Z in CCollider_Rect_Z::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Rect_Z::Clone(void * pArg)
{
	CCollider_Rect_Z*	pInstance = new CCollider_Rect_Z(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Collider_Rect_Z in CCollider_Rect_Z::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Rect_Z::Free()
{
	__super::Free();
}