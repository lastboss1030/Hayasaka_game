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
// �\���̂̒�`
//=============================================================================
typedef struct
{
	LPD3DXMESH pMeshModel;			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel;		//�}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMatModel = 0;			//�}�e���A���̐�
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				//���S���W
	D3DXVECTOR3 rot;				//��]�p
	D3DXVECTOR3 rotDest;			//�ړI�̉�]�p
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 moverot;			//��]��
	D3DXVECTOR3 vtxMinEnemy;		//���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxEnemy;		//���W�̍ő�l
	int nIdx;						//���Ԗڂ̉e��
	D3DXVECTOR3 aPos[4];			//������4�_
	D3DXVECTOR3 aVecA[4];			//�����̃x�N�g��
	D3DXVECTOR3 aVecB[4];			//�����̃x�N�g��
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Enemy;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitEnemy(void);				//�G�̏���������
void UninitEnemy(void);					//�G�̏I������
void UpdateEnemy(void);					//�G�̍X�V����
void DrawEnemy(void);					//�G�̕`�揈��

void MoveEnemy(int nCntEnemy);			//�G�̈ړ�����
void SetEnemy(D3DXVECTOR3 pos);			//�G�̐ݒu
void TexEnemy(int nCntCoin);			//�G�̃e�N�X�`��
void VecEnemy(int nCntCoin);			//�G�̒��_���W

bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax);// �����蔻��

Enemy *GetEnemy(void);

#endif