#pragma once

//=============================================================================
// Desc: CAllocateHierarchy.h
// ΢��ٷ�DirectX SDK Samples�еĹ���������
//=============================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"


//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: �̳���DXDXFRAME�ṹ�Ľṹ
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};


//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: �̳���D3DXMESHCONTAINER�ṹ�Ľṹ
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            //��������
	LPD3DXMESH           pOrigMesh;             //ԭʼ����
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                NumAttributeGroups;    //����������,������������
	DWORD                NumInfl;               //ÿ����������ܶ��ٹ�����Ӱ��
	LPD3DXBUFFER         pBoneCombinationBuf;   //������ϱ�
	D3DXMATRIX**         ppBoneMatrixPtrs;      //��Ź�������ϱ任����
	D3DXMATRIX*          pBoneOffsetMatrices;   //��Ź����ĳ�ʼ�任����
	DWORD                NumPaletteEntries;     //������������
	bool                 UseSoftwareVP;         //��ʶ�Ƿ�ʹ��������㴦��
};


//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: ����΢��ٷ�DirectX SDK Samples�еĹ��������࣬�����������.X�ļ����ؿ�ܲ�κ�����ģ������
// ���ĵ�:      #define STDMETHOD(method) virtual HRESULT STDMETHODCALLTYPE method 
//-----------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};




//-----------------------------------------------------------------------------
// Desc: ����΢��ٷ�DirectX SDK Samples�еĹ�������ȫ�ֺ���
//-----------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
