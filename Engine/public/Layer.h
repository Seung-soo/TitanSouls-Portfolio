#pragma once

#include "Base.h"


BEGIN(Engine)
class CComponent;

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
public:
	CComponent*					Get_Component(const _tchar* pComponentTag, _uint iIndex);
	list<class CGameObject*>	Get_Object() { return m_Objects; }		// 오브젝트 리스트 반환
public:
	HRESULT			Add_GameObject(class CGameObject* pGameObject);
	_int			Tick(_float fTimeDelta);
	_int			LateTick(_float fTimeDelta);
private:
	list<class CGameObject*>			m_Objects;
	typedef list<class CGameObject*>	OBJECTS;
public:
	static CLayer*	Create();
	virtual void	Free() override;
};

END