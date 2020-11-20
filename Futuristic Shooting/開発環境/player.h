//=============================================================================
//
// �v���C���[�̏��� [player.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "input.h"
#include "bullet.h"

//=============================================================================
// �v���C���[�̏��
//=============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//�o��
	PLAYERSTATE_NORMAL,		//�ʏ�
	PLAYERSTATE_DAMAGE,		//�_���[�W
	PLAYERSTATE_WAIT,		//�o���҂�
	PLAYERSTATE_DEATH,		//���S
	PLAYERSTATE_MAX
}PLAYERSTATE;

//=============================================================================
// �v���C���[�\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	PLAYERSTATE state;		//���
	int nLife;				//�̗�
	int nCounterState;		//�J�E���^�[
	int nCounterDisp;		//�_�ŃJ�E���^�[
	int nRemaining;			//�c�@
	bool bDisp;				//�_��
}	PLAYER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
bool HitPlayer(int nDamage);

#endif
