//=============================================================================
//
// �|���S������ [meshfield.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//=============================================================================
// �n�ʂ̍\����
//=============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �����ʒu
	D3DXVECTOR3	posMove;	// �����ʒu����̈ړ������ʒu
	D3DXVECTOR3 rot;		// ����
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p���
} Meshfield;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);

void SetMeshfield(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);			// ���b�V�����̕`�揈��


#endif

