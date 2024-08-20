#pragma once

#include "VIBuffer.h"
#include "Collider_Sphere.h"

BEGIN(Engine)
//class CCollider_Sphere;
class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& rhs);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	void			MakeHitBox(CCollider_Sphere::SPHEREDESC tSphereDesc);
	void			BaseSetting();
public:
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END