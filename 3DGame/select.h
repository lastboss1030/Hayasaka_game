//=============================================================================
//
// �I����ʂ̏��� [select.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _SELECTL_H_
#define _SELECTL_H_

#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
void SetTextureSelect(int nCntSelect);

#endif

