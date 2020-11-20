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
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

//=============================================================================
// �����̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�����̈ʒu
	D3DXCOLOR col;		//�����̐F
	int nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPattenAnim;	//�A�j���[�V�����p�^�[��
	bool bUse;			//�g�p���Ă��邩�ǂ���
}EXPLOSION;

#endif