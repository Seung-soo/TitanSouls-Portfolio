
#include "Client_Defines.h"
#include "AlphaUI.h"

BEGIN(Client)

class CBlood final : public CAlphaUI
{
private:
	explicit CBlood(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBlood(const CBlood& rhs);
	virtual ~CBlood() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_Alpha(_int iAlpha) { m_iAlpha = iAlpha; }
private:
	_int			m_iAlpha = 280;
protected:
	virtual HRESULT SetUp_Components();
public:
	static CBlood*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END