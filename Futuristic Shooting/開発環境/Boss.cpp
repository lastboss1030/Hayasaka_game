//=============================================================================
//
// �{�X�̏��� [Boss.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "Boss.h"				//�C���N���[�h�t�@�C��
#include "explosion.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include <stdlib.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TYPE_BOSS (1)	//�G�̎��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureBoss = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posBoss;									//�{�X�̈ʒu
D3DXVECTOR3 g_rotBoss;									//�{�X�̉�]�p
D3DXVECTOR3 g_moveBoss;									//�{�X�̈ړ���
BOSS g_aBoss		;									//�{�X�̏��
float g_fLengthBoss;									//�{�X�̑Ίp���̒���
float g_fAngleBoss;										//�{�X�̑Ίp���̊p�x
int g_nBossCnt;											//�{�X�̐�
int g_nCntBossDeath;									//�{�X���S���̃J�E���g
int g_nCounterAnimBoss;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBoss;									//�A�j���[�V�����p�^�[��No

//=============================================================================
// �{�X�̏���������
//=============================================================================
HRESULT InitBoss(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntBoss = NULL;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�A�j���[�V�����̏�����
	g_nCounterAnimBoss = 0;
	g_nPatternAnimBoss = 0;

	//�{�X�̐��̏�����
	g_nBossCnt = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Boss000.png", &g_apTextureBoss);

	//������
	g_aBoss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBoss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBoss.bUse = false;
	g_aBoss.nCounterState = 0;
	g_aBoss.state = BOSSSTATE_NORMAL;
	g_aBoss.nLife = 0;
	

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);	//�e�N�X�`�����W
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_���W�̐ݒ�
	SetVertexBoss(nCntBoss);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoss->Unlock();

	return S_OK;
}

//=============================================================================
// �{�X�̏I������
//=============================================================================
void UninitBoss(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_apTextureBoss != NULL)
	{
		g_apTextureBoss->Release();
		g_apTextureBoss = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_aBoss.bUse = false;
	}
}

//=============================================================================
// �{�X�̍X�V����
//=============================================================================
void UpdateBoss(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	BOSS *pBoss;
	static int nCount;
	int nCntBoss1 = NULL;

	//pBoss�̏�����
	pBoss = &g_aBoss;

	//�G�J�E���g
	nCount++;

	//�A�j���[�V�����J�E���^�[�X�V
	g_nCounterAnimBoss++;		

	if ((g_nCounterAnimBoss % 30) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimBoss = (g_nPatternAnimBoss + 1) % 2;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (pBoss->bUse == true)	//�{�X���g�p����Ă���Ƃ�
	{
		//�{�X�̒e���o��
		if ((rand() % 15) == 0)
		{
			SetBullet(g_aBoss.pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(1.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(-1.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(2.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(-2.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
		}

		//�ʒu�̍X�V
		pBoss->pos.x += pBoss->move.x;

		//�G�̒��_���W�̐ݒ�
		SetVertexBoss(nCntBoss1);

		//��ʊO�`�F�b�N
		if (pBoss->pos.x < 200 || pBoss->pos.x > SCREEN_WIDTH - 200)
		{
			//�[�ɂ�����t��
			g_aBoss.move *= -1.0f;
		}
	}

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimBoss), 1.0f);		//�e�N�X�`�����W
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimBoss), 0.0f);		//�A�j���[�V����
	pVtx[2].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimBoss), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimBoss), 0.0f);

		//�_���[�W�̐F�����ɖ߂�
	switch (g_aBoss.state)
	{
	case BOSSSTATE_DAMAGE:

		//�J�E���^�[�X�e�[�g�}�C�i�X
		g_aBoss.nCounterState--;

		if (g_aBoss.nCounterState <= 0)
		{
			g_aBoss.state = BOSSSTATE_NORMAL;

			//���_�̐F��߂�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBoss->Unlock();

	//�{�X��|������
	if (g_nBossCnt <= 0)
	{
		g_nCntBossDeath++;
		if (g_nCntBossDeath == 100)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//���U���g��ʂɐ؂�ւ�
			g_nBossCnt++;
		}
	}
}

//=============================================================================
// �{�X�̕`�揈��
//=============================================================================
void DrawBoss(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureBoss);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �{�X�̐ݒ�
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	//�ϐ��錾
	BOSS *pBoss;
	VERTEX_2D *pVtx;

	pBoss = &g_aBoss;

	//�G���v���X
	g_nBossCnt++;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (pBoss->bUse == false)
	{
		//�{�X�̗̑�
		g_aBoss.nLife = 180;

		//�ʒu��ݒ�
		pBoss->pos = pos;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pBoss->pos.x - 30.0f, pBoss->pos.y + 30.0f, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(pBoss->pos.x - 30.0f, pBoss->pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBoss->pos.x + 30.0f, pBoss->pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBoss->pos.x + 30.0f, pBoss->pos.y - 30.0f, 0.0f);

		//�{�X�̎g�p�̐ݒ�
		pBoss->bUse = true;

		//�{�X�̈ړ��̐ݒ�
		pBoss->move = move;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// �{�X�̒��_���W�̐ݒ�
//=============================================================================
void SetVertexBoss(int nIdx)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBoss.pos.x - 200.0f, g_aBoss.pos.y + 200.0f, 0.0f);	//�傫���ύX�͂���
	pVtx[1].pos = D3DXVECTOR3(g_aBoss.pos.x - 200.0f, g_aBoss.pos.y - 200.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBoss.pos.x + 200.0f, g_aBoss.pos.y + 200.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBoss.pos.x + 200.0f, g_aBoss.pos.y - 200.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// �{�X�̓����蔻��
//=============================================================================
bool HitBoss(int nDamage)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//�{�X���C�t�̃}�C�i�X
	g_aBoss.nLife -= nDamage;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aBoss.nLife <= 0)
	{
		//�������{�X�̍��W�ɏo��
		SetExplosion(g_aBoss.pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 100.0f));

		//�������ʉ�(����)
		PlaySound(SOUND_LABEL_SE_EXPLOSION3);

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT);

		//�{�X���g��������
		g_aBoss.bUse = false;

		//�G�̐��J�E���g�����炷
		g_nBossCnt--;

		return true;
	}
	else
	{
		g_aBoss.state = BOSSSTATE_DAMAGE;
		g_aBoss.nCounterState = 5;

		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}
//=============================================================================
// �{�X�̎擾
//=============================================================================
BOSS *GetBoss(void)
{
	return &g_aBoss;
}