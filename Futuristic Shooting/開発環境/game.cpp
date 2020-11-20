//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "game.h"			//�C���N���[�h�t�@�C��
#include "BG.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "pause.h"
#include "Time.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
bool g_bPause = false;		//�|�[�Y�����ǂ���

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
HRESULT InitGame(void)
{
	//�w�i�̏���������
	InitBG();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�|���S���̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//�e�̏���������
	InitBullet();

	//�����̏���������
	InitExplosion();

	//�X�R�A�̏���������
	InitScore();

	//�|�[�Y�̏���������
	InitPause();

	//�G�̐ݒ�
	SetEnemy(D3DXVECTOR3(600.0f, 150.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));	//1���
	SetEnemy(D3DXVECTOR3(720.0f, 150.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 150.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 150.0f, 0.0f), 3, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 150.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 150.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 150.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));

	SetEnemy(D3DXVECTOR3(600.0f, 250.0f, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));	//2���
	SetEnemy(D3DXVECTOR3(720.0f, 250.0f, 0.0f), 1, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 250.0f, 0.0f), 2, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 250.0f, 0.0f), 3, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 250.0f, 0.0f), 2, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 250.0f, 0.0f), 1, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 250.0f, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));

	SetEnemy(D3DXVECTOR3(600.0f, 350.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));	//3���
	SetEnemy(D3DXVECTOR3(720.0f, 350.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 350.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 350.0f, 0.0f), 3, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 350.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 350.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 350.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));

	//BGM
	PlaySound(SOUND_LABEL_BGM003);

	return S_OK;
}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitGame(void)
{
	//�T�E���h�X�g�b�v
	StopSound();

	//�w�i�̏I������
	UninitBG();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//�e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	//�X�R�A�̏I������
	UninitScore();

	//�|�[�Y�̏I������
	UninitPause();
}

//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame(void)
{
	//���̎擾
	int nFade = GetFade();

	//�|�[�Y
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;	//1�sVer.

		//�|�[�Y���ʉ�
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bPause == true)
	{
		//�|�[�Y�̍X�V����
		UpdatePause();
	}

	if (g_bPause == false)
	{
		//�w�i�̍X�V����
		UpdateBG();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�G�̍X�V����
		UpdateEnemy();

		//�e�̍X�V����
		UpdateBullet();

		//���j�̍X�V����
		UpdateExplosion();

		//�X�R�A�̍X�V����
		UpdateScore();
	}
}

//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBG();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�e�̕`�揈��
	DrawBullet();

	//�����̕`�揈��
	DrawExplosion();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�|�[�Y�̕`�揈��
	if (g_bPause ==true)
	{
		DrawPause();
	}
}

//=============================================================================
// �|�[�Y�̐ݒ�
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}