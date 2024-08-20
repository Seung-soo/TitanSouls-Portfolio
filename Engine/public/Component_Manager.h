#pragma once

#include "Base.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Rect_Z.h"
#include "VIBuffer_Ceiling.h"
#include "VIBuffer_Rect_Color.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider_Rect.h"
#include "Collider_Rect_Z.h"
#include "Collider_Sphere.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT			Reserve_Container(_uint iNumLevels);
	HRESULT			Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent*		Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	HRESULT			Clear_LevelComponent(_uint iLevelIndex);
public:
	_bool			Collision_Sphere(CCollider_Sphere* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereGuard(CCollider_Sphere* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereBounce(CCollider_Sphere* pDst, CCollider_Sphere* pSrc, _float3 vLook);
	_float3			Collision_SphereToRect(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRectArrowFull(CCollider_Rect * pDst, CCollider_Sphere * pSrc);
	_float3			Collision_SphereToRectArrow(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRect_Camera(CCollider_Rect* pDst, CCollider_Sphere* pSrc);
	_float3			Collision_SphereToRectZ(CCollider_Rect_Z* pDst, CCollider_Sphere* pSrc);
private:
	map<const _tchar*, CComponent*>*		m_pPrototypes = nullptr;
	typedef map<const _tchar*, CComponent*>	PROTOTYPES;
private:
	_uint			m_iNumLevels = 0;
private:
	CComponent*		Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);
public:
	virtual void	Free() override;
};

END