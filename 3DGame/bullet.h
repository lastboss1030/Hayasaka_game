//=============================================================================
//
// �e�̏��� [bullet.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//=============================================================================
// �e�̎��
//=============================================================================
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//�v���C���[�̒e
	BULLETTYPE_ENEMY,		//�G�̒e
	BULLETTYPE_EFFECT,
	BULLETTYPE_MAX
}BULLETTYPE;

//=============================================================================
// �e�̍\����
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//�o���ʒu
	D3DXVECTOR3 move;		//�ړ���
	BULLETTYPE type;		//���
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}BULLET;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos,		//�o���ʒu
				D3DXVECTOR3 move,	//�ړ���
				float fWidth,		//��
				float fHeight,		//����
				BULLETTYPE type);	//���
void FollowBullet(void);

BULLET *GetBullet(void);

#endif
