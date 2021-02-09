//=============================================================================
//
// �e�̏��� [shadow.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "player.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_SHADOW (64)		//�e�̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pTextureShadow = NULL;		//�e�N�X�`���ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];						//�e�̏��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//������
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].fWidth = 1.0f;
		g_aShadow[nCntShadow].fDepth = 1.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//���_���W�̐ݒ�(Z�^)
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + 30.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + 30.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - 30.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - 30.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�S�i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();


	Player *pPlayer;
	pPlayer = GetPlayer();

	//�e�̔z�u
	SetShadow(pPlayer->pos, 10.0f, 10.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// �e�̐ݒ�
//==============================================================================
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	//�ϐ��錾
	int nShadow = -1;

	//�e�̐ݒ�
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			//�ʒu�̐ݒ�
			g_aShadow[nCntShadow].pos = pos;

			//���̐ݒ�
			g_aShadow[nCntShadow].fWidth = fWidth;

			//���s�̐ݒ�
			g_aShadow[nCntShadow].fDepth = fDepth;

			//�g���Ă��邩
			g_aShadow[nCntShadow].bUse = true;

			//�e�ɑ��
			nShadow = nCntShadow;

			break;
		}
	}

	return nShadow;	//�Ԃ�
}

//==============================================================================
// �e�̒Ǐ]
//==============================================================================
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//==============================================================================
// �e�̍폜
//==============================================================================
void DeleteShadow(int nIdx)
{
	// �e�̍\���̂̃|�C���^�ϐ�
	Shadow *pShadow = &g_aShadow[0];

	// ���Ԗڂ̉e��
	pShadow += nIdx;

	// �g�p��Ԃ�false�ɂ���
	pShadow->bUse = false;
}