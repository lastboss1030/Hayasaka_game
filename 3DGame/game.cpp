//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "game.h"
#include "Billboard.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "explosion.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "shadow.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "cooldown_UI.h"
#include "item_boost.h"
#include "enemy.h"
#include "time.h"
#include "score.h"
#include "ranking.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_WALLSIZE (1600.0f)
#define HEIGHT_SIZE	 (100.0f)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
bool g_bPause = false;		//�|�[�Y�����ǂ���
int g_nCntResult;		// ���U���g��ʂ܂ł̃J�E���g

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
HRESULT InitGame(void)
{
	//�J�E���g������
	g_nCntResult = 0;

	//���b�V���t�B�[���h�̏���������
	InitMeshfield();

	//��(���b�V��)�̏���������
	InitMeshwall();

	//�r���{�[�h�̏���������
//	InitBillboard();

	//�e�̏���������
	InitBullet();

	//�����̏���������
	InitExplosion();

	//�e�̏���������
	InitShadow();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//���f���̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//�u�[�X�g�A�C�e���̏���������
	InitItemBoost();

	//�|�[�Y�̏���������
	InitPause();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�N�[���_�E��UI�̏���������
	InitCooldown();

	//�^�C���̏���������
	InitTime();

	//�X�R�A�̏���������
	InitScore();

	//------------------------------------

	//�ǂ̔z�u
	SetWall();

	//------------------------------------

	//BGM
	PlaySound(SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitGame(void)
{
	//�T�E���h�X�g�b�v
	StopSound();

	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	//��(���b�V��)�̏I������
	UninitMeshwall();

	//�r���{�[�h�̏I������
//	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	//�e�̏I������
	UninitShadow();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//���f���̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//�u�[�X�g�A�C�e���̏I������
	UninitItemBoost();

	//�|�[�Y�̏I������
	UninitPause();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�N�[���_�E��UI�̏I������
	UninitCooldown();

	//�^�C���̏I������
	UninitTime();

	//�X�R�A�̏I������
	UninitScore();
}

//=============================================================================
// �Q�[����ʂ̍X�V����
//=============================================================================
void UpdateGame(void)
{
	//���̎擾
	int nFade = GetFade();
	int nTime = GetTime();

	//�|�[�Y
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		//���ʉ�
		PlaySound(SOUND_LABEL_SE_PAUSE);

		g_bPause = g_bPause ? false : true;	//1�sVer.
	}

	if (g_bPause == true)
	{
		//�|�[�Y�̍X�V����
		UpdatePause();
	}

	if (g_bPause == false)
	{
		//�^�C���̍X�V����
		UpdateTime();
		if (nTime != 0)
		{
			//���b�V���t�B�[���h�̍X�V����
			UpdateMeshfield();

			//��(���b�V��)�̍X�V����
			UpdateMeshwall();

			//�r���{�[�h�̍X�V����
//			UpdateBillboard();

			//�e�̍X�V����
			UpdateBullet();

			//�����̍X�V����
			UpdateExplosion();

			//�e�̍X�V����
			UpdateShadow();

			//�G�t�F�N�g�̍X�V����
			UpdateEffect();

			//���f���̍X�V����
			UpdatePlayer();

			//�G�̍X�V����
			UpdateEnemy();

			//�u�[�X�g�A�C�e���̍X�V����
			UpdateItemBoost();

			//�J�����̍X�V����
			UpdateCamera();

			//���C�g�̍X�V����
			UpdateLight();

			//�N�[���_�E��UI�̍X�V����
			UpdateCooldown();

			//�X�R�A�̍X�V����
			UpdateScore();
		}
	}

	// ���U���g��ʑJ��
	if ((nTime == 0))
	{
		//���U���g��ʂ܂ł̃J�E���g
		g_nCntResult++;
		if (g_nCntResult == 60)
		{
			//��ʃ��[�h�̏���������
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//=============================================================================
// �Q�[����ʂ̕`�揈��
//=============================================================================
void DrawGame(void)
{
	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshfield();

	//��(���b�V��)�̕`�揈��
	DrawMeshwall();

	//�r���{�[�h�̕`�揈��
//	DrawBillboard();

	//�e�̕`�揈��
	DrawBullet();

	//�����̕`�揈��
	DrawExplosion();

	//�e�̕`�揈��
	DrawShadow();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//���f���̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�u�[�X�g�A�C�e���̕`�揈��
	DrawItemBoost();

	//�N�[���_�E��UI�̕`�揈��
	DrawCooldown();

	//�^�C���̕`�揈��
	DrawTime();

	//�X�R�A�̕`�揈��
	DrawScore();


	//�|�[�Y�̕`�揈��
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//=============================================================================
// �ǂ̔z�u
//=============================================================================
void SetWall(void)
{
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, MAX_WALLSIZE), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI / -2, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);

	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);

	//�G�̐ݒu
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 600.0f));
}

//=============================================================================
// �|�[�Y�̐ݒ�
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}