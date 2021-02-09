//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "object.h"
#include "input.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX (10)

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPD3DXMESH g_pMeshObject = NULL;			// ���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatObject = NULL;		// �}�e���A��(�ގ����)�ւ̃|�C���^
DWORD g_nNumMatObject = 0;					// �}�e���A���̐�					
LPDIRECT3DTEXTURE9 g_apTextureObject[TEX] = {};	//�e�N�X�`���ւ̃|�C���^
Object g_object;

//==============================================================================
// ����������
//==============================================================================
HRESULT InitObject(void)
{
	//���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/robot000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject,
		NULL,
		&g_nNumMatObject,
		&g_pMeshObject);

	//�ϐ��錾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = g_pMeshObject->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject->GetFVF());

	//�ϐ��̏�����
	g_object.pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	g_object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_object.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_object.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�����b�N
	g_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_���W�̔�r
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �e���W�̍ő�l�̔�r
		if (g_object.vtxMaxObject.x < vtx.x)
		{
			g_object.vtxMaxObject.x = vtx.x;
		}
		if (g_object.vtxMaxObject.y < vtx.y)
		{
			g_object.vtxMaxObject.y = vtx.y;
		}
		if (g_object.vtxMaxObject.z < vtx.z)
		{
			g_object.vtxMaxObject.z = vtx.z;
		}

		//�e���W�̍ŏ��l�̔�r
		if (g_object.vtxMinObject.x > vtx.x)
		{
			g_object.vtxMinObject.x = vtx.x;
		}
		if (g_object.vtxMinObject.y > vtx.y)
		{
			g_object.vtxMinObject.y = vtx.y;
		}
		if (g_object.vtxMinObject.z > vtx.z)
		{
			g_object.vtxMinObject.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshObject->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void UninitObject(void)
{
	//���b�V���̔j��
	if (g_pMeshObject != NULL)
	{
		g_pMeshObject->Release();
		g_pMeshObject = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatObject != NULL)
	{
		g_pBuffMatObject->Release();
		g_pBuffMatObject = NULL;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void UpdateObject(void)
{

}

//==============================================================================
// �`�揈��
//==============================================================================
void DrawObject(void)
{
	//���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_object.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object.rot.y, g_object.rot.x, g_object.rot.z);
	D3DXMatrixMultiply(&g_object.mtxWorld, &g_object.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_object.pos.x, g_object.pos.y, g_object.pos.z);
	D3DXMatrixMultiply(&g_object.mtxWorld, &g_object.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_object.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//���f��(�p�[�c)�̕`��
		g_pMeshObject->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//=============================================================================
// �I�u�W�F�N�g�̎擾
//=============================================================================
Object *GetObject(void)
{
	return &g_object;
}