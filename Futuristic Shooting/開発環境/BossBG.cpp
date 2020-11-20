//=============================================================================
//
// �{�X�w�i���� [BossBG.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "BossBG.h"									//�C���N���[�h�t�@�C��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossBG = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureBossBG;				//�e�N�X�`���̃|�C���^

//=============================================================================
// �{�X�w�i�̏���������
//=============================================================================
HRESULT InitBossBG(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Boss_BG000.jpg", &g_apTextureBossBG);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossBG,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBossBG->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//�e�N�X�`�����W
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossBG->Unlock();

	return S_OK;
}

//=============================================================================
// �{�X�w�i�̏I������
//=============================================================================
void UninitBossBG(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBossBG != NULL)
	{
		g_pVtxBuffBossBG->Release();
		g_pVtxBuffBossBG = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_apTextureBossBG != NULL)
	{
		g_apTextureBossBG->Release();
		g_apTextureBossBG = NULL;
	}
}

//=============================================================================
// �{�X�w�i�̍X�V����
//=============================================================================
void UpdateBossBG(void)
{

}

//=============================================================================
// �{�X�w�i�̕`�揈��
//=============================================================================
void DrawBossBG(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBossBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureBossBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}