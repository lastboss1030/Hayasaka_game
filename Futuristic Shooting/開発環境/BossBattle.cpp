//=============================================================================
//
// �{�X�o�g���̏��� [BossBattle.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "BossBattle.h"			//�C���N���[�h�t�@�C��
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "effect.h"
#include "Time.h"
#include "BossBG.h"
#include "Boss.h"
#include "enemy.h"

//=============================================================================
// �{�X�o�g����ʂ̏���������
//=============================================================================
HRESULT InitBossBattle(void)
{
	//�w�i�̏���������
	InitBossBG();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�e�̏���������
	InitBullet();

	//�|���S���̏���������
	InitPlayer();

	//�{�X�̏���������
	InitBoss();

	//�����̏���������
	InitExplosion();

	//�{�X�̔z�u
	SetBoss(D3DXVECTOR3(960.0f, 200.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f));

	//BGM
	PlaySound(SOUND_LABEL_BGM_BOSS);

	return S_OK;
}

//=============================================================================
// �{�X�o�g����ʂ̏I������
//=============================================================================
void UninitBossBattle(void)
{
	//�T�E���h�X�g�b�v
	StopSound();

	//�w�i�̏I������
	UninitBossBG();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�e�̏I������
	UninitBullet();

	//�v���C���[�̏I������
	UninitPlayer();

	//�{�X�̏I������
	UninitBoss();

	//�����̏I������
	UninitExplosion();
}

//=============================================================================
// �{�X�o�g����ʂ̍X�V����
//=============================================================================
void UpdateBossBattle(void)
{
	//�w�i�̍X�V����
	UpdateBossBG();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�e�̍X�V����
	UpdateBullet();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�{�X�̍X�V����
	UpdateBoss();

	//���j�̍X�V����
	UpdateExplosion();
}

//=============================================================================
// �{�X�o�g����ʂ̕`�揈��
//=============================================================================
void DrawBossBattle(void)
{
	//�w�i�̕`�揈��
	DrawBossBG();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�e�̕`�揈��
	DrawBullet();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�{�X�̕`�揈��
	DrawBoss();

	//�����̕`�揈��
	DrawExplosion();
}
