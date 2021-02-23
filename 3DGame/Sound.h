//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		//�^�C�g��
	SOUND_LABEL_BGM_TUTORIAL,		//�`���[�g���A��
	SOUND_LABEL_BGM_SELECT,			//�Z���N�g
	SOUND_LABEL_BGM_GAME,			//�Q�[��
	SOUND_LABEL_BGM_RESULT,			//���U���g
	SOUND_LABEL_BGM_RANKING,		//�����L���O
	SOUND_LABEL_SE_DEATH,			//���S
	SOUND_LABEL_SE_ENTER,			//����
	SOUND_LABEL_SE_SELECT,			//�I��
	SOUND_LABEL_SE_SHOOT_GOLEM,		//�V���b�g�S�[����
	SOUND_LABEL_SE_SHOOT_LEO,		//�V���b�g���I
	SOUND_LABEL_SE_SHOOT_STALKER,	//�V���b�g�X�g�[�J�[
	SOUND_LABEL_SE_HIT,				//�q�b�g
	SOUND_LABEL_SE_ITEM,			//�A�C�e��
	SOUND_LABEL_SE_PAUSE,			//�|�[�Y
	SOUND_LABEL_SE_TIMEUP,			//���Ԑ؂�
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

