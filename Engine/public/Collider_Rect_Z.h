#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Rect_Z final : public CCollider
{
public:
typedef struct tagRecteDesc
{
	_float4x4	ObjectMatrix;
	_float		fX, fZ;
	_float		fRadius;
}RECTDESC;
protected:
	explicit CCollider_Rect_Z(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider_Rect_Z(const CCollider_Rect_Z& rhs);
	virtual ~CCollider_Rect_Z() = default;
public:
	RECTDESC			Get_RectDesc() { return m_tRectDesc; }
	void				Set_RectDesc(RECTDESC tRectDesc) { m_tRectDesc = tRectDesc; }
public:
	virtual HRESULT		NativeConstruct_Prototype();
	virtual HRESULT		NativeConstruct(void* pArg);
private:
	RECTDESC			m_tRectDesc;
public:
	static CCollider_Rect_Z* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr);
	virtual void	Free() override;
};
END