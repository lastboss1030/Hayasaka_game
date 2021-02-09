//=============================================================================
//
// �����̏��� [explosion.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//=============================================================================
// �\���̂̒�`
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	int nCounterAnimE;		// �A�j���[�V�����J�E���^�[
	int nPatternAnimE;		// �e�N�X�`���p�^�[���̃J�E���g
	bool bUse;				// �g�p���Ă��邩�ǂ���
} Explosion;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitExplosion(void);			// �����̏���������
void UninitExplosion(void);				// �����̏I������
void UpdateExplosion(void);				// �����̍X�V����
void DrawExplosion(void);				// �����̕`�揈��	

void SetExplosion(D3DXVECTOR3 pos);		// �����̐ݒ�

#endif
