#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;
public:
	_float			Get_TimeDelta();	// 델타 타임 반환
private:
	LARGE_INTEGER	m_CurrentTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_OriginTime;
	LARGE_INTEGER	m_CpuTick;

	_float			m_fTimeDelta;
public:
	static CTimer*	Create();
	virtual void	Free() override;
};

END