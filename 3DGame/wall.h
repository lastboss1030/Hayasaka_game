//=============================================================================
//
// �ǂ̏��� [wall.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//=============================================================================
// �ǂ̍\����
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Wall;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
Wall *GetWall(void);

#endif

