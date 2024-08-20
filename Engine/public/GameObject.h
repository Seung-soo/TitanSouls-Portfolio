#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	CComponent*		Get_Component(const _tchar* pComponentTag);
	_bool			Get_Dead() { return m_bObjectDead; }
	_float			Get_CamDistance() const { return m_fCamDistance; }
	_float			Get_AlphaZ() { return m_fAlphaZ; }
public:
	virtual HRESULT	NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	HRESULT			Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** pOut, void* pArg = nullptr);
	void			Go_Frame(_float fSpeed, _float fTimeDelta);
	void			Go_FrameEnd(_float fSpeed, _float fTimeDelta);
	_uint			Get_Depth() { return m_iDepth; }
protected:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	_float				m_fCamDistance;
protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*> COMPONENTS;
	FRAME									m_tFrame;
	_float									m_fAlphaZ = 0.f;
protected:
	_float									m_fSpriteSpeed = 0.f;
	_bool									m_bObjectDead = false;
	_uint									m_iDepth = 0;
protected:
	CComponent*		Find_Component(const _tchar* pComponentTag);
	HRESULT Compute_CamDistance(class CTransform* pTransform);
public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END