
#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Color final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect_Color(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect_Color(const CVIBuffer_Rect_Color& rhs);
	virtual ~CVIBuffer_Rect_Color() = default;
public:
	virtual HRESULT				NativeConstruct_Prototype() override;
	virtual HRESULT				NativeConstruct(void* pArg) override;
public:
	static CVIBuffer_Rect_Color*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;
};

END