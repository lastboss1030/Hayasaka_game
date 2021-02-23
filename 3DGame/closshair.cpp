//=============================================================================
//
// �N���X�w�A���� [Closshair.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "closshair.h"							//�C���N���[�h�t�@�C��
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX (1)									//�e�N�X�`���ő吔
#define MAX_VERTEX (4)								//���_��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClosshair = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureClosshair[MAX_TEX];	//�e�N�X�`���̃|�C���^
D3DXCOLOR g_colorClosshair;							//�F

//=============================================================================
// �N���X�w�A�̂̏���������
//=============================================================================
HRESULT InitClosshair(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	g_colorClosshair = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/closs.png", &g_apTextureClosshair[0]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffClosshair,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffClosshair->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntClosshair = 0; nCntClosshair < MAX_TEX; nCntClosshair++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureClosshair(nCntClosshair);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffClosshair->Unlock();

	return S_OK;
}

//=============================================================================
// �N���X�w�A�̏I������
//=============================================================================
void UninitClosshair(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffClosshair != NULL)
	{
		g_pVtxBuffClosshair->Release();
		g_pVtxBuffClosshair = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureClosshair[nCntTexture] != NULL)
		{
			g_apTextureClosshair[nCntTexture]->Release();
			g_apTextureClosshair[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �N���X�w�A�̍X�V����
//=============================================================================
void UpdateClosshair(void)
{

}

//=============================================================================
// �N���X�w�A�̕`�揈��
//=============================================================================
void DrawClosshair(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffClosshair, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureClosshair[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureClosshair[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}


//=============================================================================
// �N���X�w�A�̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureClosshair(int nCntClosshair)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffClosshair->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntClosshair * 4;

	if (nCntClosshair == 0)	//�u�[�X�g�Q�[�W
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(920, 180, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(920, 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1000, 180, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1000, 100, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffClosshair->Unlock();
}