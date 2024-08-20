#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

class ENGINE_DLL CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();
public:
	HRESULT			Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** pOut, void* pArg = nullptr);
protected:
	CTexture*		m_pTextureCom = nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
protected:
	virtual HRESULT SetUp_Components() = 0;
public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};

END