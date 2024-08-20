#include "..\public\Collider_Rect.h"

CCollider_Rect::CCollider_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CCollider_Rect::CCollider_Rect(const CCollider_Rect & rhs)
	: CCollider(rhs)
{
}

HRESULT CCollider_Rect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct_Prototype returned E_FAIL in CCollider_Rect::NativeConstruct_Prototype");
		return E_FAIL;
	}

	ZeroMemory(&m_tRectDesc, sizeof(RECTDESC));
	return S_OK;
}

HRESULT CCollider_Rect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("__super::NativeConstruct returned E_FAIL in CCollider_Rect::NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
		memcpy(&m_tRectDesc, (RECTDESC*)pArg, sizeof(RECTDESC));

	return S_OK;
}

CCollider_Rect * CCollider_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Rect*	pInstance = new CCollider_Rect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Collider_Rect in CCollider_Rect::Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Rect::Clone(void * pArg)
{
	CCollider_Rect*	pInstance = new CCollider_Rect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create Collider_Rect in CCollider_Rect::Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Rect::Free()
{
	__super::Free();
}