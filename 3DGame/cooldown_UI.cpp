//=============================================================================
//
// �N�[���_�E��UI���� [cooldownUI.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "cooldown_UI.h"							//�C���N���[�h�t�@�C��
#include "input.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX (6)									//�e�N�X�`���ő吔
#define MAX_VERTEX (4)								//���_��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCooldown = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureCooldown[MAX_TEX];	//�e�N�X�`���̃|�C���^
D3DXCOLOR g_colorCooldown;							//�F
int g_nCounterAnimC;								//�A�j���[�V�����J�E���^�[
int g_nPatternAnimC;								//�A�j���[�V�����p�^�[��No
bool bUse;

//=============================================================================
// �N�[���_�E��UI�̂̏���������
//=============================================================================
HRESULT InitCooldown(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	g_colorCooldown = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bUse = false;

	//�A�j���[�V�����̏�����
	g_nCounterAnimC = 0;
	g_nPatternAnimC = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown001.png", &g_apTextureCooldown[0]);	//��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown100.png", &g_apTextureCooldown[1]);	//�u�[�X�g�g��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown002.png", &g_apTextureCooldown[2]);	//�I�����W
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown101.png", &g_apTextureCooldown[3]);	//Hp�g��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown003.png", &g_apTextureCooldown[4]);	//�s���N
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown.png", &g_apTextureCooldown[5]);		//�GHP�g��
			
	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCooldown,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntCooldown = 0; nCntCooldown < MAX_TEX; nCntCooldown++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureCooldown(nCntCooldown);

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
	g_pVtxBuffCooldown->Unlock();

	return S_OK;
}

//=============================================================================
// �N�[���_�E��UI�̏I������
//=============================================================================
void UninitCooldown(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffCooldown != NULL)
	{
		g_pVtxBuffCooldown->Release();
		g_pVtxBuffCooldown = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureCooldown[nCntTexture] != NULL)
		{
			g_apTextureCooldown[nCntTexture]->Release();
			g_apTextureCooldown[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �N�[���_�E��UI�̍X�V����
//=============================================================================
void UpdateCooldown(void)
{
	VERTEX_2D *pVtx;
	Player *pPlayer;
	Enemy *pEnemy;
	pPlayer = GetPlayer();
	pEnemy = GetEnemy();

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);

	//�u�[�X�g
	pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Z��0.0�Œ�
	pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pPlayer->boost * 3, 80, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pPlayer->boost * 3, 20, 0.0f);

	//HP
	pVtx[8].pos = D3DXVECTOR3(20, 150, 0.0f);	//Z��0.0�Œ�
	pVtx[9].pos = D3DXVECTOR3(20, 90, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(pPlayer->Life * 3, 150, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(pPlayer->Life * 3, 90, 0.0f);

	//�GHP
	pVtx[16].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Z��0.0�Œ�
	pVtx[17].pos = D3DXVECTOR3(1120, 20, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(1120 +pEnemy->nLife * 3.7f, 80, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(1120 +pEnemy->nLife * 3.7f, 20, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCooldown->Unlock();
}

//=============================================================================
// �N�[���_�E��UI�̕`�揈��
//=============================================================================
void DrawCooldown(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCooldown, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
		{
			pDevice->SetTexture(0, g_apTextureCooldown[nCntTexture]);
		}

		//�|���S���̕`��
		for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
		{
			pDevice->SetTexture(0, g_apTextureCooldown[nCntTexture]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
		}
	}
}


//=============================================================================
// �N�[���_�E��UI�̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureCooldown(int nCntCooldown)
{
	VERTEX_2D *pVtx;
	Player *pPlayer;
	pPlayer = GetPlayer();

	bUse = true;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntCooldown * 4;

	if (nCntCooldown == 0)	//�u�[�X�g�Q�[�W
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 20, 0.0f);
	}
	else if (nCntCooldown == 1)	//�g��
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 20, 0.0f);
	}
	else if (nCntCooldown == 2)	//HP�Q�[�W
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(20, 150, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(20, 90, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 150, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 90, 0.0f);
	}
	else if (nCntCooldown == 3)	//�g��
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(20, 150, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(20, 90, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 150, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 90, 0.0f);
	}
	else if (nCntCooldown == 4)	//�GHP�Q�[�W
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(1120, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1500, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1500, 20, 0.0f);
	}
	else if (nCntCooldown == 5)	//�g��
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(1120, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1500, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1500, 20, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCooldown->Unlock();
}