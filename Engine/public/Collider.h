#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	virtual HRESULT		NativeConstruct_Prototype();
	virtual HRESULT		NativeConstruct(void* pArg);
public:
	virtual CComponent*	Clone(void* pArg = nullptr) = 0;
	virtual void	Free() override;
};

END