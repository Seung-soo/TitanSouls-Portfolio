#pragma once

#include "Base.h"
#include "Level.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	HRESULT			OpenLevel(_uint iNextLevelIndex, CLevel* pNextLevel);
	_int			Tick(_float fTimeDelta);
	_int			LateTick(_float fTimeDelta);
	HRESULT			Render();
private:
	_uint			m_iCurrentLevelIndex = 0;
	CLevel*			m_pCurrentLevel = nullptr;
public:
	virtual void	Free() override;
};

END