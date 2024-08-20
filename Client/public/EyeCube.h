#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CCollider_Sphere;
END

BEGIN(Client)
class CCamera_Player;
class CEyeCube final : public CGameObject
{
private:
	explicit CEyeCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEyeCube(const CEyeCube& rhs);
	virtual ~CEyeCube() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CTransform* Get_TransForm() {
		return m_pTransformCom;
	}

	_bool Get_Jump() {
		return m_bJump;
	}
	_bool	Get_Clear() { return m_bClear; }
	void	Set_Clear(_bool bClear) { m_bClear = bClear; }
private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;
	CCollider_Sphere*	m_pColliderCom = nullptr;
	CCollider_Sphere*	m_pWeaknessCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCollCom = nullptr;
private:
	_bool btest = false;
	_float m_fFrame = 0.f;
	_float m_fPetternTimer = 0;

	_bool m_bBlackTileRayzer = false;
	_bool m_bWhiteTileRayzer = false;

	_bool  m_bJump = false;
	_float m_fJumpPower = 10.f;
	_float m_fJumpy;
	_float m_fTime = 0;

	_float3 RevelPos = { 0,0,0 };
	_bool* m_bDead = false;


	//������ �ӵ�
	_float m_fRollingSpeed = 2.f;
	_uint m_iRollingTimer = 0;
	//����Ƚ�� üũ
	_uint m_iRollingCount = 0;;
	//��ġ �˻��
	_float3 m_fCompaerCubePos;
	_float3 m_fCompaerPlayerPos;

	//�̵���
	_bool m_bGoZUP;
	_bool m_bGoZDown;
	_bool m_bGoXRight;
	_bool m_bGoXLeft;

	_bool m_bRayzerPattern = false;
	_float m_fPatternTime = 0;
	_bool m_bRayzer_Jump = false;
	_float3 m_fRayerUsePos;
	// save��
	_float3 m_fDir;
	_float m_fDir_Length;
	//�׽� �˻��
	_float3 m_fDir_Check;
	_float m_fDir_Length_Check;

	_bool TestStop = false;
	_bool m_bClear = false;

	_bool m_bStart = false;

	CCamera_Player*		m_pCamera = nullptr;
	_int	m_iSoulOutTime = 120;
	_bool	m_iOutSoul = false;

	RAYZERPOS m_tRayPos;
	_float	fSoundTimer = 0.f;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(); // �÷��̾ �ͷ������� �ö󰡰� �ϴ� �Լ�
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CEyeCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END