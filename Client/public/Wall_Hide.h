#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CCollider_Rect;

class CRenderer;
END

BEGIN(Client)
class CVIBuffer_Wall;
class CWall_Hide final : public CGameObject
{
public:
	explicit CWall_Hide(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWall_Hide(const CWall_Hide& rhs);
	virtual ~CWall_Hide() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTransform*			m_pTransformCom = nullptr;
	CCollider_Rect*		m_pColliderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Wall*		m_pVIBufferCom = nullptr;
private:
	HRESULT SetUp_Components();

public:
	static	CWall_Hide*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END