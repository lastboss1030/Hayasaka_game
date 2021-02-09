//=============================================================================
//
// �u�[�X�g�A�C�e���̏��� [item_boost.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _ITEM_BOOST_H_
#define _ITEM_BOOST_H_

#include "main.h"

//=============================================================================
// �\���̂̒�`
//=============================================================================
typedef struct
{
	LPD3DXMESH pMeshItemBoost;			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatItemBoost;		//�}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMatItemBoost = 0;			//�}�e���A���̐�
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;					//���S���W
	D3DXVECTOR3 rot;					//��]�p
	D3DXVECTOR3 rotDest;				//�ړI�̉�]�p
	D3DXVECTOR3 move;					//�ړ���
	D3DXVECTOR3 moverot;				//��]��
	D3DXVECTOR3 vtxMinItemBoost;		//���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxItemBoost;		//���W�̍ő�l
	int nIdx;							//���Ԗڂ̉e��
	bool bUse;							//�g�p���Ă��邩�ǂ���
}ItemBoost;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitItemBoost(void);
void UninitItemBoost(void);	
void UpdateItemBoost(void);	
void DrawItemBoost(void);	

void SetItemBoost(D3DXVECTOR3 pos);		//�A�C�e���̐ݒu
void VecItemBoost(int nCntItemBoost);	//�A�C�e���̒��_���W

#endif
