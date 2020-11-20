//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col);

//=============================================================================
// �G�t�F�N�g�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�G�t�F�N�g�̈ʒu
	D3DXCOLOR col;		//�G�t�F�N�g�̐F
	bool bUse;			//�g�p���Ă��邩�ǂ���
}EFFECT;

#endif