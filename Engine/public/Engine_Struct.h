#pragma once

namespace Engine
{

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR2			vTexUV;
	}VTXTEX;

	typedef struct tagVertex_Texture_Color
	{
		D3DXVECTOR3			vPosition;
		DWORD				dwColor;
		D3DXVECTOR2			vTexUV;
	}VTXTEX_COLOR;

	typedef struct tagVertex_CubeTexture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vTexUV;
	}VTXCUBETEX;

	typedef struct tagLineIndices32
	{
		unsigned long	_0, _1;
	}LINEINDICES32;


	typedef struct tagLineIndices16
	{
		unsigned short	_0, _1;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long	_0, _1, _2;
	}FACEINDICES32;


	typedef struct tagFaceIndices16
	{
		unsigned short	_0, _1, _2;
	}FACEINDICES16;

	// ��������Ʈ �̹��� ����� �� Ȱ���ϴ� ����ü
	typedef struct tagFrame		
	{
		float	iTexWidth;		// ���̴� �̹��� ��ü ���� ������(������� 512)
		float	iTexHeight;		// ���̴� �̹��� ��ü ���� ������(������� 512)
		float	iFrameWidth;	// �̹��� �ȿ��� ����� ���� ������(������� 16)
		float	iFrameHeight;	// �̹��� �ȿ��� ����� ���� ������(������� 16)
		float	iFrameStartX;	// ����� ������ ������
		float	iFrameCurrentX;	// ���� ��
		float	iFrameStartY;	// �� ����� ������ ������
		float	iFrameEndX;		// ������ ��(����������)
	}FRAME;

	typedef struct tagPosAngle
	{
		_float3 vPos;
		_float	fAngle;
	}POSANGLE;

	typedef struct tagPosAngleScale
	{
		_float3 vPos;
		_float	fAngle;
		_float	fScale;
	}POSANGLESCALE;

	typedef struct tagSceneChange
	{
		_bool bCrease;
		_uint iLevelIndex;
	}SCENECHANGE;

	typedef struct IceBrain_Dead
	{
		_bool		bIceDead;
		_bool		bBrainDead;
	}ICEDEAD;

	typedef struct Slime_Dead_Info
	{
		_bool		bBossSlimeDead;
		_bool		bHeartDead;
	}SLIMEDEAD;
	typedef struct Slime_Info
	{
		_float3		fvPos;
		_float		fScale;
		_uint		iSmallRank;
		SLIMEDEAD*   tSlimeDead;
	}SLIMEINFO;


	typedef struct Colossus_Info
	{
		_bool bLeftFistAttack;
		_bool bRightFistAttack;
		_bool bLeftAttacking;
		_bool bRightAttacking;
		_bool bStandUp;
		_bool bRightWallCrash;
		_bool bLeftWallCrash;
		_uint FistCount = 0;
		_bool bWallCrash;
	}COLOSSUSINFO;

	typedef struct Ice_Brain_Button
	{
		_bool bOnButton;
		_uint iButtonNum;
	}BUTTONINFO;

	typedef struct EyeCubeRayzerPos
	{
		_float Xpos;
		_float Ypos;
	}RAYZERPOS;

}
