#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Color;
END

BEGIN(Client)

class CLightLeak final : public CGameObject
{
private:
	explicit CLightLeak(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLightLeak(const CLightLeak& rhs);
	virtual ~CLightLeak() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect_Color* m_pVIBufferCom = nullptr;
private:
	_int			m_iDir = 0;
	_float3			m_vAxis;
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
public:
	static CLightLeak*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END