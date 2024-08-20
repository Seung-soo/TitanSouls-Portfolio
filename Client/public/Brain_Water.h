#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Brain_water_mini.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CBrain_Water final : public CGameObject
{
private:
	explicit CBrain_Water(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrain_Water(const CBrain_Water& rhs);
	virtual ~CBrain_Water() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect_Z*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*		m_pColliderCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCollCom = nullptr;

	CBrain_water_mini* m_SlimeInfo;
	_uint		iRandomNum = 0;
	_float		m_fScale = 1.5f;
	_uint m_iDeathCout = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // 플레이어를 터레인위에 올라가게 하는 함수
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Collision();
public:
	static CBrain_Water* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END