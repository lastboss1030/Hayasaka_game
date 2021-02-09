//=============================================================================
//
// �v���C���[�̏��� [model.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_USE_MODEL (2)	//���f���̍ő吔
#define MAX_PARTS (2)		//�p�[�c�ő吔
#define MAX_KEY (20)		//�L�[�̍ő吔
#define MAX_MOTION (5)		//���[�V�����ő吔

//=============================================================================
// ���[�V�����̎��
//=============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//�j���[�g����
	MOTIONTYPE_ATTACK,		//�U��
	MOTIONTYPE_MAX
}MOTIONTYPE;

//=============================================================================
// �L�[�̍\����
//=============================================================================
typedef struct
{
	float fPosX;	//�ʒuX
	float fPosY;	//�ʒuY
	float fPosZ;	//�ʒuZ

	float fRotX;	//����X
	float fRotY;	//����Y
	float fRotZ;	//����Z
}KEY;

//=============================================================================
// �L�[���̍\����
//=============================================================================
typedef struct
{
	int nFrame;				//�Đ��t���[��
	KEY aKey[MAX_PARTS];	//�e���f���̃L�[�v�f
}KEY_INFO;

//=============================================================================
// ���[�V�������̍\����
//=============================================================================
typedef struct
{
	bool bLoop;					//���[�v���邩�ǂ���
	int nNumKey;				//�L�[�̑���
	KEY_INFO aKeyInfo[MAX_KEY];	//�L�[���
}MOTION_INFO;

//=============================================================================
// ���f���̍\����
//=============================================================================
typedef struct
{
	LPD3DXMESH pMesh;		//���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A���̐�
	D3DXVECTOR3 pos;		//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxModelParent;	//�e���f���̃C���f�b�N�X
}Model;

//=============================================================================
// �v���C���[�̍\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//���S���W
	D3DXVECTOR3 rot;		//��]
	D3DXVECTOR3 rotDest;	//�ړI�̌���
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float boost;			//�W�����v�u�[�X�g
	bool bUse;				//�g�p���Ă��邩
	int nShadow;			//�e

	Model aModel[MAX_USE_MODEL];	//���f��(�p�[�c)
	int nNumModel;					//���f��(�p�[�c)��

	MOTION_INFO aMotionInfo[MAX_MOTION];	//���[�V�������
	int nNumMotion;							//���[�V������
	MOTIONTYPE motionType;					//���[�V�����^�C�v
	bool bLoopMotion;						//���[�v���邩�ǂ���
	int nNumKey;							//�L�[��
	int nKey;								//�L�[No.
	int nCounterMotion;						//���[�V�����̃J�E���^�[

	D3DXVECTOR3 minVecPlayer;		// ���_�̍ŏ��l
	D3DXVECTOR3 maxVecPlayer;		// ���_�̍ő�l
}Player;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void); 

void InitMotion(void);
void MotionPlayer(void);
void EffectPlayer(void);
void CollisionPlayer(void);

KEY KeyPosRot(float posX, float posY, float posZ,
				float rotX, float rotY, float rotZ);
Player *GetPlayer(void);

#endif
