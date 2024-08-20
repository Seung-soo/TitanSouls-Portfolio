#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Rect final : public CCollider
{
public:
	typedef struct tagRecteDesc
{
	_float4x4	ObjectMatrix;
	_float		fX, fY;
}RECTDESC;
protected:
	explicit CCollider_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider_Rect(const CCollider_Rect& rhs);
	virtual ~CCollider_Rect() = default;
public:
	RECTDESC			Get_RectDesc() { return m_tRectDesc; }
	void				Set_RectDesc(RECTDESC tRectDesc) { m_tRectDesc = tRectDesc; }
public:
	virtual HRESULT		NativeConstruct_Prototype();
	virtual HRESULT		NativeConstruct(void* pArg);
private:
	RECTDESC			m_tRectDesc;
public:
	static CCollider_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr);
	virtual void	Free() override;
};
END