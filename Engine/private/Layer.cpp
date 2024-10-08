#include "..\public\Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
	{
		MSGBOX("iIndex >= m_Objects.size in CLayer::Get_Component");
		return nullptr;
	}

	auto iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; i++)
		++iter;
	
	return (*iter)->Get_Component(pComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_float fTimeDelta)
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if (m_Objects.empty())
			break;

		_int iState = (*iter)->Tick(fTimeDelta);
		if (0 > iState)
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
			iter++;
	}
	return 0;
}

_int CLayer::LateTick(_float fTimeDelta)
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if (m_Objects.empty())
			break;

		_int iState = (*iter)->LateTick(fTimeDelta);
		if (0 > iState)
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
			iter++;
	}
	return 0;
}

CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}
