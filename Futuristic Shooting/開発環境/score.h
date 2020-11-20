//=============================================================================
//
// �X�R�A�̏��� [score.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetTextureScore(int nIdx, int nNumber);

#endif
