#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayer;
class CSoul final : public CGameObject
{
public:
	typedef struct tagSoulDesc
	{
		_float3 vAxis;
		_float3 vCenterPosition;
	}SOULDESC;
private:
	explicit CSoul(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSoul(const CSoul& rhs);
	virtual ~CSoul() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
private:
	SOULDESC			m_tSoulDesc;
	_int				m_iSlowTime = 30;
	_bool				m_bRevolution = false;

	CPlayer*			m_pPlayer = nullptr;
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
public:
	static CSoul*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END