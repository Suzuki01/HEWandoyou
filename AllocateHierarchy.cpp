//=============================================================================
// Desc: AllocateHierarchyClass.cpp
// ΢��ٷ�DirectX SDK Samples�еĹ���������
//=============================================================================

#include "AllocateHierarchy.h"

//--------------------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Ϊ�������������Ƶ��ַ��������ڴ�
//--------------------------------------------------------------------------------------
HRESULT AllocateName( LPCSTR Name, LPSTR* pNewName )
{
	UINT cbLength;

	if( Name != NULL )
	{
		cbLength = ( UINT )strlen( Name ) + 1;
		*pNewName = new CHAR[cbLength];
		memcpy( *pNewName, Name, cbLength * sizeof( CHAR ) );
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GenerateSkinnedMesh
// Desc: ������Ƥ����ģ�͡��ú����жϵ�ǰ���������Ƿ��������Ƥ��Ϣ�������ǰ����ģ��
//       �в�������Ƥ��Ϣ����ֱ���˳��ú�����������ȷ������Ҫ�ľ����ɫ������������
//       ��ID3DXSkinInfo::ConvertToIndexedBlendedMesh()��������������Ƥ����ģ��
//--------------------------------------------------------------------------------------
HRESULT GenerateSkinnedMesh( IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer )
{
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps( &d3dCaps );

	if( pMeshContainer->pSkinInfo == NULL )
		return S_OK;

	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );


	if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
		pMeshContainer->pOrigMesh,								//ԭʼ���� 
		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
		pMeshContainer->pAdjacency,
		NULL, NULL, NULL,
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups,
		&pMeshContainer->pBoneCombinationBuf,
		&pMeshContainer->MeshData.pMesh)))
		return E_FAIL;
	return S_OK;
}



//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: �������, �����Ƿ����ڴ�ͳ�ʼ��,��û�ж����Ա������ʵ�ֵ
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME* ppNewFrame )
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED *pFrame;

	*ppNewFrame = NULL;

	// Ϊ���ָ������
	pFrame = new D3DXFRAME_DERIVED;  // ������ܽṹ����
	if (FAILED(AllocateName(Name, (LPSTR*)&pFrame->Name)))
	{
		delete pFrame;
		return hr;
	}

	// ��ʼ��D3DXFRAME_DERIVED�ṹ������Ա����
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer   = NULL;
	pFrame->pFrameSibling    = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	return hr;
}


//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: ������Ƥ�����������Լ�����Ƥ��Ϣ
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer( LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
	HRESULT hr;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if( pMeshData->Type != D3DXMESHTYPE_MESH )
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if( pMesh->GetFVF() == 0 )
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	memset( pMeshContainer, 0, sizeof( D3DXMESHCONTAINER_DERIVED ) );

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName( Name, &pMeshContainer->Name );
	if( FAILED( hr ) )
		goto e_Exit;

	pMesh->GetDevice( &pd3dDevice );
	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if( !( pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh );
		if( FAILED( hr ) )
			goto e_Exit;

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals( pMesh, NULL );
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	pMeshContainer->NumMaterials = max( 1, NumMaterials );
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];		//��������
	pMeshContainer->pAdjacency = new DWORD[NumFaces*3];

	memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof( DWORD ) * NumFaces*3 );
	memset( pMeshContainer->ppTextures, 0, sizeof( LPDIRECT3DTEXTURE9 ) * pMeshContainer->NumMaterials );	//��������

	// if materials provided, copy them
	if( NumMaterials > 0 )
	{
		memcpy( pMeshContainer->pMaterials, pMaterials, sizeof( D3DXMATERIAL ) * NumMaterials );

		for( iMaterial = 0; iMaterial < NumMaterials; iMaterial++ )
		{
			if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL )
			{
				if( FAILED( D3DXCreateTextureFromFileA( pd3dDevice, pMeshContainer->pMaterials[iMaterial].pTextureFilename,
					&pMeshContainer->ppTextures[iMaterial] ) ) )
					pMeshContainer->ppTextures[iMaterial] = NULL;

				// don't remember a pointer into the dynamic memory, just forget the name after loading
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else // if no materials provided, use a default one
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset( &pMeshContainer->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// if there is skinning information, save off the required data and then setup for HW skinning
	if( pSkinInfo != NULL )
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;				//ԭʼ����
		pMesh->AddRef();

		// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];

		// get each of the bone offset matrices so that we don't need to get them later
		for( iBone = 0; iBone < cBones; iBone++ )
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *( pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( iBone ) );
		}

		// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
		hr = GenerateSkinnedMesh( pd3dDevice, pMeshContainer );
		if( FAILED( hr ) )
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

e_Exit:
	SAFE_RELEASE( pd3dDevice );

	// call Destroy function to properly clean up the memory allocated 
	if( pMeshContainer != NULL )
	{
		DestroyMeshContainer( pMeshContainer );
	}

	return hr;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: �ͷŹ������
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	SAFE_DELETE_ARRAY( pFrameToFree->Name );
	SAFE_DELETE( pFrameToFree );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: �ͷ���������
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = ( D3DXMESHCONTAINER_DERIVED* )pMeshContainerBase;

	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

	// release all the allocated textures
	if( pMeshContainer->ppTextures != NULL )
	{
		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++ )
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
		}
	}

	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->pOrigMesh );		//ԭʼ����
	SAFE_DELETE( pMeshContainer );
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: ���úø�����ܵ���ϱ任����
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot )
{
	if( pFrameBase->pMeshContainer != NULL )
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: ������Ƥ�����е���Ƥ����
//--------------------------------------------------------------------------------------
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase )
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = ( D3DXMESHCONTAINER_DERIVED* )pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = ( D3DXFRAME_DERIVED* )pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps( &d3dCaps );

	// first check for skinning
	if( pMeshContainer->pSkinInfo != NULL )
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer() );

		// Draw using default vtx processing of the device (typically HW)
		for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++ )
		{
			NumBlend = 0;
			for( DWORD i = 0; i < pMeshContainer->NumInfl; ++i )
			{
				if( pBoneComb[iAttrib].BoneId[i] != UINT_MAX )
				{
					NumBlend = i;
				}
			}

			if( d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1 )
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for( DWORD i = 0; i < pMeshContainer->NumInfl; ++i )
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if( iMatrixIndex != UINT_MAX )
					{
						D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
						pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp );
					}
				}

				pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, NumBlend );

				// lookup the material used for this subset of faces
				if( ( AttribIdPrev != pBoneComb[iAttrib].AttribId ) || ( AttribIdPrev == UNUSED32 ) )
				{
					pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
					pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
			}
		}
		pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, 0 );
	}
	else  // standard mesh, just draw it after setting material properties
	{
		pd3dDevice->SetTransform( D3DTS_WORLD, &pFrame->CombinedTransformationMatrix );

		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++ )
		{
			pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
			pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
			pMeshContainer->MeshData.pMesh->DrawSubset( iMaterial );
		}
	}
}


//--------------------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: ���ƹ���
//--------------------------------------------------------------------------------------
void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame )
{
	if (pFrame == NULL) return;
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = pFrame->pMeshContainer;                    // ȡ����������
	while( pMeshContainer != NULL )                      
	{
		DrawMeshContainer(pd3dDevice, pMeshContainer, pFrame);  // ���Ʒǿ���Ƥ����
		pMeshContainer = pMeshContainer->pNextMeshContainer;    // ����������������
	}

	DrawFrame(pd3dDevice, pFrame->pFrameSibling);               // �����ֵܿ��
	DrawFrame(pd3dDevice, pFrame->pFrameFirstChild);            // �����ӿ��
}

//--------------------------------------------------------------------------------------
// Name: UpdateFrameMatrics()
// Desc: ���¿���еı任����
//--------------------------------------------------------------------------------------
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
	if (pFrameBase == NULL || pParentMatrix == NULL) return;
	D3DXFRAME_DERIVED* pFrame = ( D3DXFRAME_DERIVED* )pFrameBase;

	// ����ǰ����������ڸ�������ƫ�ƾ������ۻ�����
	D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);

	UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);                              // �����ֵܹ���
	UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);   // �����ӹ���
}
