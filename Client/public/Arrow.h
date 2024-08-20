#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Z;
class CCollider_Sphere;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CPlayer;
class CArrow final : public CGameObject
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_bool	Get_Fire() { return m_bFire; }
	_bool	Get_FireOff() { return m_bFireOff; }
	void	Set_Fire(_bool bFire, _bool bFireOff) { m_bFire = bFire; m_bFireOff = bFireOff; }
	_float3 Get_FirePos();
	void	Set_FullCharging(_bool bFullCharging) { m_bFullCharging = bFullCharging; }
	_bool	Get_FullCharging() { return m_bFullCharging; }
	//ArrowRect Ãß°¡
	CTransform* Get_TransForm() { return m_pTransformCom; }
public:
	_int Collision_Rect();
	void Gravity(_float fTimeDelta);
private:
	CTexture*			m_pTextureCom = nullptr;
	CVIBuffer_Rect_Z*	m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;

	CPlayer*			m_pPlayer = nullptr;
private:
	_float				m_fGravity = 0.f;
	_bool				m_bStart = true;
	_bool				m_bFire = false;
	_int				m_iArrowFireTime = 60;
	_bool				m_bFireOff = false;

	_bool				m_bArrow_Rect = true;
	_float				m_fRectSec = 0;

	_bool				m_bFullCharging = false;
	_bool				m_bArrowDrive = true;

	_int				m_iAlpha = 255;
	_bool				m_bGoAlpha = false;

	_bool				m_bSound = true;
	_bool				m_bColStart = true;
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();
public:
	static CArrow*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END