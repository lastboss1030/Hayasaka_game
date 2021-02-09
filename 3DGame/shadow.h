//=============================================================================
//
// �e�̏��� [shadow.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//=============================================================================
// �\���̂̒�`
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	float fWidth;			//��
	float fDepth;			//���s
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Shadow;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
void DeleteShadow(int nIdx);

#endif
