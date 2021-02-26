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
// �}�N����`
//=============================================================================
#define MAX_SCORE		(8)		//����

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitScore(void);		
void UninitScore(void);			
void UpdateScore(void);			
void DrawScore(void);	
void AddScore(int nValue);		// �X�R�A�̉��Z
int GetScore(void);				// �X�R�A�̎擾

#endif