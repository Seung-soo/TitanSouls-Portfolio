#include "..\public\Base.h"
using namespace Engine;

CBase::CBase()
{
}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release()
{
	if (0 != m_dwRefCnt)
		return m_dwRefCnt--;
	else
	{
		Free();
		delete this;
		return 0;
	}
}