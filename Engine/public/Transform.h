#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
#pragma region
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
public:
	typedef struct tagTransformDesc
	{
		_float	fSpeedPerSec;
		_float	fRotationPerSec;
	}TRANSFORMDESC;
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	_float3		Get_State(STATE eState) { return *(_float3*)&m_WorldMatrix.m[eState][0]; }
	_float3		Get_Scale();
	_float4x4	Get_WorldMatrixInverse();
	_float4x4	Get_WorldMatrix() { return m_WorldMatrix; }
	void		Set_WorldMatrix(_float4x4& worldMatrix) { m_WorldMatrix = worldMatrix; }

	TRANSFORMDESC	Get_TransformDesc() { return m_TransformDesc; }

	void		Set_State(STATE eState, const _float3& vState);
	void		Set_TransformDesc(const TRANSFORMDESC& TransformDesc);
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	void Go_Straight(_float fTimeDelta);
	void Go_BackWard(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Vector(_float3 vDir, _float fTimeDelta);

	void LookAt(_float3 vTargetPos);
	void LookAtUp(_float3 vTargetPos);
	void LookAtXZ(_float3 vTargetPos);
	void MatchIt_XZ(_float3 vTargetPos);
	void Turn(_float3 vAxis, _float fTimeDelta);
	void LookTurn(_float3 vAxis, _float3 vTarget, _float fTimeDelta);
	void Rotation(_float3 vAxis, _float fRadian);
	void Revolution(_float3 vAxis, _float3 vTarget, _float fTimeDelta);
	void Scaled(_float3 vScale);
public:
	HRESULT Bind_OnGraphicDevice();
#pragma endregion
private:
	_float4x4		m_WorldMatrix;
	TRANSFORMDESC	m_TransformDesc;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END