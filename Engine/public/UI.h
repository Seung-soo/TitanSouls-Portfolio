#pragma once

#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

class ENGINE_DLL CUI abstract : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT			Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** pOut, void* pArg = nullptr);
protected:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

protected:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
protected:
	virtual HRESULT SetUp_Components() = 0;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END