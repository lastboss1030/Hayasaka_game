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
#include "object.h"
#include "cooldown_UI.h"
#include "item_boost.h"
#include "enemy.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_WALLSIZE (800.0f)
#define HEIGHT_SIZE	 (100.0f)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
bool g_bPause = false;		//�|�[�Y�����ǂ���

//=============================================================================
// �Q�[����ʂ̏���������
//=============================================================================
HRESULT InitGame(void)
{
	//���b�V���t�B�[���h�̏���������
	InitMeshfield();

	//��(���b�V��)�̏���������
	InitMeshwall();

	//�r���{�[�h�̏���������
	InitBillboard();

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

	//�I�u�W�F�N�g�̏���������
//	InitObject();

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

	//------------------------------------

	//�ǂ̔z�u
	SetWall();

	//------------------------------------
	return S_OK;
}

//=============================================================================
// �Q�[����ʂ̏I������
//=============================================================================
void UninitGame(void)
{
	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	//��(���b�V��)�̏I������
	UninitMeshwall();

	//�r���{�[�h�̏I������
	UninitBillboard();

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

	//�I�u�W�F�N�g�̏I������
//	UninitObject();

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
		//���ʉ�

		g_bPause = g_bPause ? false : true;	//1�sVer.
	}

	if (g_bPause == true)
	{
		//�|�[�Y�̍X�V����
		UpdatePause();
	}

	if (g_bPause == false)
	{
		//���b�V���t�B�[���h�̍X�V����
		UpdateMeshfield();

		//��(���b�V��)�̍X�V����
		UpdateMeshwall();

		//�r���{�[�h�̍X�V����
		UpdateBillboard();

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

		//�I�u�W�F�N�g�̍X�V����
//		UpdateObject();

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
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//���ʉ�

			SetFade(FADE_OUT, MODE_RESULT);	//�^�C�g����ʂɐ؂�ւ�
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
	DrawBillboard();

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

	//�I�u�W�F�N�g�̕`�揈��
//	DrawObject();

	//�G�̕`�揈��
	DrawEnemy();

	//�u�[�X�g�A�C�e���̕`�揈��
	DrawItemBoost();

	//�N�[���_�E��UI�̕`�揈��
	DrawCooldown();


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
}

//=============================================================================
// �|�[�Y�̐ݒ�
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}