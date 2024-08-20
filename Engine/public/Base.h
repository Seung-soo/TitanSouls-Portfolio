#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	unsigned long	AddRef();	// ���۷��� ����
	unsigned long	Release();	// ����

private:
	unsigned long	m_dwRefCnt = 0;	// ���۷��� ī��Ʈ
public:
	virtual void	Free() = 0;
};

END