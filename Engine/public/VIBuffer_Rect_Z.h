#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Z final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Rect_Z(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Rect_Z(const CVIBuffer_Rect_Z& rhs);
	virtual ~CVIBuffer_Rect_Z() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT						Go_Sprites(FRAME& tFrame);
	void						Go_Sprites_Reverse_Y(FRAME& tFrame);
	HRESULT						Base_Setting();
public:
	static CVIBuffer_Rect_Z* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END