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
// �|�[�Y���j���[
//=============================================================================
typedef enum
{
	SELECT_MENU_GOLEM = 0,	//�S�[����
	SELECT_MENU_LEO,			//���I
	SELECT_MENU_STALKER,		//�X�g�[�J�[
	SELECT_MENU_MAX
}SELECT_MENU;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
void SetTextureSelect(int nCntSelect);

#endif

