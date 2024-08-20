#include "..\public\Timer_Manager.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_float CTimer_Manager::Get_TimeDelta(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	// Ÿ�̸Ӹ� �߰��Ϸ��µ� �̹� �±װ� ������ �߰�X
	if (nullptr != Find_Timer(pTimerTag))
	{
		MSGBOX("Timer already exists and cannot be added.");
		return E_FAIL;
	}

	// ���� �Ҵ��� ���� �ʾ����� �߰�X
	CTimer* pTimer = CTimer::Create();
	if (nullptr == pTimer)
	{
		MSGBOX("Timer is nullptr and cannot be added.");
		return E_FAIL;
	}

	m_Timers.emplace(pTimerTag, pTimer);

	return S_OK;
}

CTimer * CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto iter = find_if(m_Timers.begin(), m_Timers.end(), CTagFinder(pTimerTag));

	if (iter == m_Timers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& Pair : m_Timers)
		Safe_Release(Pair.second);

	m_Timers.clear();
}
