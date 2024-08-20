#pragma once
#include "Client_Defines.h"
#include "UI.h"
BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Skill_Empty final : public CUI
{
private:
	explicit CUI_Skill_Empty(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Skill_Empty(const CUI_Skill_Empty& rhs);
	virtual ~CUI_Skill_Empty() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
public:
	//void	Ready_Level_UI(LEVEL eCurrentLevel, _tchar* pLayerTag);
	//HRESULT Ready_BossName_UI(LEVEL eCurrentLevel, _tchar* pLayerTag);
private:
	CTransform*			m_pTransformCom = nullptr;
	
	_int				m_iInterval = 0;
	_int				m_iDirCnt = 0;
	LEVEL				m_eCurrentLevel;
private:
	virtual		HRESULT SetUp_Components(void);
public:
	static CUI_Skill_Empty*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CUI*		Clone(void* pArg) override;
	virtual void		Free() override;
};
END