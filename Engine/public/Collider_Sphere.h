#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Sphere final : public CCollider
{
public:
typedef struct tagSphereDesc
{
	_float4x4	ObjectMatrix;
	_float3		fDist_From_Obj;
	_float		fRadius;
}SPHEREDESC;
protected:
	explicit CCollider_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider_Sphere(const CCollider_Sphere& rhs);
	virtual ~CCollider_Sphere() = default;
public:
	SPHEREDESC			Get_SphereDesc() { return m_tSphereDesc; }
	void				Set_SphereDesc(SPHEREDESC& tSphereDesc)
	{
		m_tSphereDesc = tSphereDesc;
		D3DXVec3TransformCoord(&m_vPosition, &m_tSphereDesc.fDist_From_Obj, &m_tSphereDesc.ObjectMatrix);
	}
	_float3				Get_Position() { return m_vPosition; }
	void				Set_Radius(_float fRadius) { m_tSphereDesc.fRadius = fRadius; }
public:
	virtual HRESULT		NativeConstruct_Prototype();
	virtual HRESULT		NativeConstruct(void* pArg);
	_int				Tick();
private:
	SPHEREDESC			m_tSphereDesc;
	_float3				m_vPosition;
public:
	static CCollider_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr);
	virtual void	Free() override;
};
END