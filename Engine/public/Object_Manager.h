#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Layer.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	CComponent*	Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
	size_t		Get_Size(_uint iLevelIndex, const _tchar* pLayerTag) { return Find_Layer(iLevelIndex, pLayerTag)->Get_Object().size(); }
	CGameObject* Get_Front(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	HRESULT	Clear_LevelObject(_uint iLevelIndex);
	HRESULT	Release_LayerObject(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	_int	Tick(_float fTimeDelta);
	_int	LateTick(_float fTimeDelta);
private:
	map<const _tchar*, CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, CLayer*>*			m_pLayers;
	typedef map<const _tchar*, CLayer*>	LAYERS;
private:
	_uint			m_iNumLevels = 0;
public:
	CGameObject*	Find_Prototype(const _tchar* pPrototypeTag);
	CLayer*			Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	virtual void	Free() override;
};

END