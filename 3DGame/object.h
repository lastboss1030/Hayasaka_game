//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// ���f���̊e���W�̍ő�l�E�ŏ��l
	int nIdx;
} Object;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitObject(void);	
void UninitObject(void);	
void UpdateObject(void);	
void DrawObject(void);		

Object *GetObject(void);

#endif
