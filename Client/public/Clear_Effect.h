#include "Client_Defines.h"
#include "AlphaUI.h"

BEGIN(Client)

class CClear_Effect final : public CAlphaUI
{
private:
	explicit CClear_Effect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CClear_Effect(const CClear_Effect& rhs);
	virtual ~CClear_Effect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void			Set_Alpha(_int iAlpha) { m_iAlpha = iAlpha; }
private:
	_int			m_iAlpha = 0;
protected:
	virtual HRESULT SetUp_Components();
public:
	static CClear_Effect*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END