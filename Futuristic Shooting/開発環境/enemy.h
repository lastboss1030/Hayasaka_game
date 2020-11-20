//=============================================================================
//
// �G�̏��� [enemy.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY (256)	//�G�̍ő吔

//=============================================================================
// �G�̏��
//=============================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�ʏ���
	ENEMYSTATE_DAMAGE,		//�_���[�W���
	ENEMYSTATE_MAX			
}ENEMYSTATE;

//=============================================================================
// �G�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
	ENEMYSTATE state;		//���
	int nCounterState;		//��ԃJ�E���^�[
	int nLife;				//�̗�
}ENEMY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move);
ENEMY *GetEnemy(void);
void SetVertexEnemy(int nIdx);
bool HitEnemy(int Idx, int nDamage);

#endif