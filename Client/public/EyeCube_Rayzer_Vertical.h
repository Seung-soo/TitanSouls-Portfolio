#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)
class CVIBuffer_Cube_Rayzer;
class CEyeCube_Rayzer_Vertical final : public CGameObject
{
private:
	explicit CEyeCube_Rayzer_Vertical(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEyeCube_Rayzer_Vertical(const CEyeCube_Rayzer_Vertical& rhs);
	virtual ~CEyeCube_Rayzer_Vertical() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Cube_Rayzer*		m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;

private:
	_uint m_fCount = 0;
	_float m_fScale = 4.f;
	RAYZERPOS* m_tPOS;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Collision();
public:
	static CEyeCube_Rayzer_Vertical* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END