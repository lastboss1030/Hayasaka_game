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
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM_TITLE,		// BGM�^�C�g��
	SOUND_LABEL_BGM_END,		// BGM���U���g
	SOUND_LABEL_BGM_BOSS,		// BGM�{�X
	SOUND_LABEL_TUTORIAL,		// �`���[�g���A��
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_LASER,		// ���[�U�[��
	SOUND_LABEL_SE_LASER2,		// ���[�U�[��
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_EXPLOSION2,	// ������2
	SOUND_LABEL_SE_EXPLOSION3,	// �������{�X
	SOUND_LABEL_SE_PAUSE,		// �|�[�Y
	SOUND_LABEL_SE_START,		// �J�n
	SOUND_LABEL_SE_DECISION,	// ����
	SOUND_LABEL_SE_SELECT,		// �Z���N�g
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
