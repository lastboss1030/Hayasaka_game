//=============================================================================
//
// �v���C���[�̏��� [model.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "select.h"
#include "enemy.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MOVE_MODEL		(1.0f)			//���f���ړ���
#define MOVE_BULLET		(5.0f)			//�e�ړ���
#define HIT_WALL		(750.0f)		//��
#define MAX_TEX			(10)			//�e�N�X�`��
#define MAX_BOOST		(200)			//�u�[�X�g
#define PLAYER_LIFE		(200)			//���C�t�

#define VTX_MINP	(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// �����_�ŏ��l
#define	VTX_MAXP	(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// �����_�ő�l

#define GRAVITY	(0.5f)				//�d��
#define HEIGHT_LIMIT	(150.0f)	//�������	

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPD3DXMESH g_pMeshModel = NULL;						//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;				//�}�e���A��(�ގ����)�ւ̃|�C���^
DWORD g_nNumMatModel = 0;							//�}�e���A���̐�
D3DXVECTOR3 g_vtxMinPlayer, g_vtxMaxPlayer;			//���f���̍ŏ��A�ő�
LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
Player g_player;									//���f�����

D3DXVECTOR3 aCollisionPos[2];						//�����蔻�胉�C��

int g_nStateP;										//�X�e�[�g
int g_nShootCount = 0;								//���˃J�E���g
int g_nEffect = 0;									//�G�t�F�N�g
int g_life = 0;										//���C�t
float g_move;										//�ړ����x

int g_nCntDeath;									//���S���̑ҋ@
int g_nCntEffectDeath = 0;							//���S�G�t�F�N�g

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���C���[�̍\���̂̏�����
	g_player.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.Life = PLAYER_LIFE;
	g_player.boost = MAX_BOOST;
	g_player.nNumModel = MAX_USE_MODEL;
	g_player.bUse = true;

	//�ϐ�������
	g_vtxMinPlayer = VTX_MINP;
	g_vtxMaxPlayer = VTX_MAXP;

	aCollisionPos[0] = D3DXVECTOR3(-200, 0, 200);
	aCollisionPos[1] = D3DXVECTOR3(200, 0, 200);

	g_nCntDeath = 0;
	
	//���[�V�����̏�����
	InitMotion();

	if (g_player.playertype == PLAYERTYPE_GOLEM)
	{
		//�ړ����x
		g_move = 1.4f;

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/golem.x",	//�S�[����
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/kang_dae.x",	//�L���m��
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//�e�p�[�c�̊K�w�\���̐ݒ�
		g_player.aModel[0].nIdxModelParent = -1;	//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 75.0f, 5.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//�ʒu
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	}

	if (g_player.playertype == PLAYERTYPE_LEO)
	{
		//�ړ����x
		g_move = 1.1f;

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/leo.x",	//���I
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/gekko.x",	//����
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//�e�p�[�c�̊K�w�\���̐ݒ�
		g_player.aModel[0].nIdxModelParent = -1;	//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 80.0f, 10.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//�ʒu
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	}	

	if (g_player.playertype == PLAYERTYPE_STALKER)
	{
		//�ړ����x
		g_move = 1.8f;

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/stalker.x",	//�X�g�[�J�[
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/punishert000.x",	//�p�j�b�V���[
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//�e�p�[�c�̊K�w�\���̐ݒ�
		g_player.aModel[0].nIdxModelParent = -1;	//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//�e�̃C���f�b�N�X��ݒ�
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 68.0f, 10.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//�ʒu
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	}
	
	//�ϐ��錾
	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//���_�����擾
		nNumVtx = g_player.aModel[nCntModel].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[nCntModel].pMesh->GetFVF());

		//���_�o�b�t�@�����b�N
		g_player.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		//���_���̔����o��
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//�S�Ă̒��_���r�����f���̍ŏ��l�A�ő�l�𔲂��o��
			// �e���W�̍ő�l�̔�r
			if (g_player.minVecPlayer.x > vtx.x)
			{
				// X���W�̍ŏ��l
				g_player.minVecPlayer.x = vtx.x;
			}
			else if (g_player.maxVecPlayer.x < vtx.x)
			{
				// X���W�̍ő�l
				g_player.maxVecPlayer.x = vtx.x;
			}

			if (g_player.minVecPlayer.y > vtx.y)
			{
				// Y���W�̍ŏ��l
				g_player.minVecPlayer.y = vtx.y;
			}
			else if (g_player.maxVecPlayer.y < vtx.y)
			{
				// Y���W�̍ő�l
				g_player.maxVecPlayer.y = vtx.y;
			}

			if (g_player.minVecPlayer.z > vtx.z)
			{
				// Z���W�̍ŏ��l
				g_player.minVecPlayer.z = vtx.z;
			}
			else if (g_player.maxVecPlayer.z < vtx.z)
			{
				// Z���W�̍ő�l
				g_player.maxVecPlayer.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		// 0.0f��菬�����Ƃ�-1.0�{����
		if (g_player.minVecPlayer.x < 0.0f)
		{
			g_player.minVecPlayer.x *= -1.0f;
		}
		if (g_player.minVecPlayer.y < 0.0f)
		{
			g_player.minVecPlayer.y *= -1.0f;
		}
		if (g_player.minVecPlayer.z < 0.0f)
		{
			g_player.minVecPlayer.z *= -1.0f;
		}

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		//�e�N�X�`��
		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
			}
		}

		//���_�o�b�t�@���A�����b�N
		g_player.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < MAX_USE_MODEL; nCntModel++)
	{
		//���b�V���̔j��
		if (g_player.aModel[nCntModel].pMesh != NULL)
		{
			g_player.aModel[nCntModel].pMesh->Release();
			g_player.aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_player.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.aModel[nCntModel].pBuffMat->Release();
			g_player.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	//�e�N�X�`���̊J��
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		if (g_apTextureModel[nCntMat] != NULL)
		{
			g_apTextureModel[nCntMat]->Release();
			g_apTextureModel[nCntMat] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	//���˃J�E���g��i�߂�
	g_nShootCount++;

	g_player.posOld = g_player.pos;

	//�ʒu�̍X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	//�ړ��ʂ̌���
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	//�d��
	if (g_player.pos.y > 0.0f)
	{
		g_player.move.y -= GRAVITY;
	}

	//�n�ʂ߂荞�ݖ߂�
	if (g_player.pos.y < 0.0f)
	{
		g_player.pos.y = 0.0f;
	}

	//�u�[�X�g�c�ʂ̒ǉ�
	if (g_player.pos.y == 0.0f)
	{
		if (g_player.boost < MAX_BOOST)
		{
			if ((g_nShootCount % 3) == 0)
			{
				g_player.boost++;
			}
		}
	}


	//�������
	if (g_player.pos.y > HEIGHT_LIMIT)
	{
		g_player.pos.y = HEIGHT_LIMIT;
	}


	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			// �������
			g_player.move.x -= cosf(pCamera->rot.y + D3DX_PI / 4) * g_move;
			g_player.move.z += sinf(pCamera->rot.y + D3DX_PI / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			// �E�����
			g_player.move.x += cosf(pCamera->rot.y - D3DX_PI / 4) * g_move;
			g_player.move.z -= sinf(pCamera->rot.y - D3DX_PI / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{
			// �����
			g_player.move.x += sinf(pCamera->rot.y) * g_move;
			g_player.move.z += cosf(pCamera->rot.y) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			// ��������
			g_player.move.x += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_move;
			g_player.move.z -= sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			// �E������
			g_player.move.x -= cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_move;
			g_player.move.z += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / -4;
		}
		else
		{
			// ������
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI) * g_move;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI) * g_move;
			g_player.rotDest.y = pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		// ������
		g_player.move.x -= cosf(pCamera->rot.y) * g_move;
		g_player.move.z += sinf(pCamera->rot.y) * g_move;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / 2);
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		// �E����
		g_player.move.x += cosf(pCamera->rot.y) * g_move;
		g_player.move.z -= sinf(pCamera->rot.y) * g_move;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / -2);
	}

	// �ړI�̉�]�p�̏��
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// �����̍X�V
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// ���݂̉�]�p�̏��
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	//�e�̔���
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		//�S�[�����I����
		if (g_player.playertype == PLAYERTYPE_GOLEM)
		{
			if ((g_nShootCount % 30) == 0)
			{
				if (g_player.boost > 40)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 85.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -15.0f, 0.0f, cosf(g_player.rot.y) * -15.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//���ʉ�
					PlaySound(SOUND_LABEL_SE_SHOOT_GOLEM);

					g_player.boost -= 25;
				}
			}
		}

		//���I�I����
		if (g_player.playertype == PLAYERTYPE_LEO)
		{
			if ((g_nShootCount % 60) == 0)
			{
				if (g_player.boost > 20)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 90.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -12.0f, 0.0f, cosf(g_player.rot.y) * -12.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//���ʉ�
					PlaySound(SOUND_LABEL_SE_SHOOT_LEO);

					g_player.boost -= 40;
				}
			}
		}

		//�X�g�[�J�[�I����
		if (g_player.playertype == PLAYERTYPE_STALKER)
		{
			if ((g_nShootCount % 10) == 0)
			{
				if (g_player.boost > 20)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 85.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -20.0f, 0.0f, cosf(g_player.rot.y) * -20.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//���ʉ�
					PlaySound(SOUND_LABEL_SE_SHOOT_STALKER);

					g_player.boost -= 10;
				}
			}
		}

	}

	//�e�̒Ǐ]
	SetPositionShadow(g_player.nShadow, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	//�ǂ̐ݒ�
	if (g_player.pos.x < -HIT_WALL)
	{
		//���[
		g_player.pos.x = -HIT_WALL;
	}
	if (g_player.pos.x > HIT_WALL)
	{
		//�E�[
		g_player.pos.x = HIT_WALL;
	}
	if (g_player.pos.z > HIT_WALL)
	{
		//��
		g_player.pos.z = HIT_WALL;
	}
	if (g_player.pos.z < -HIT_WALL)
	{
		//��O
		g_player.pos.z = -HIT_WALL;
	}

	//���[�V����
	MotionPlayer();

	//�G�t�F�N�g
	EffectPlayer();

	if (g_player.Life <= 0)
	{
		//�g��������
		g_player.bUse = false;

		//�e������
		DeleteShadow(g_player.nShadow);

		g_nCntEffectDeath++;

		if (g_nCntEffectDeath % 1 == 0)
		{
			for (int g_nCntEffect = 0; g_nCntEffect < 1; g_nCntEffect++)
			{
				//�p�x�̐ݒ�
				float fAngle = ((float)(rand() % 800)) / 100.0f;
				float fmove = (float)(rand() % 1 + 1);

				//�G�t�F�N�g�̐ݒ�
				SetEffect(g_player.pos,
					D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					5.0f,
					0.01f,
					0.1f);
			}
		}

		g_nCntDeath++;
		if (g_nCntDeath == 20)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//���U���g��ʂɐ؂�ւ�
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_player.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_player.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p�}�g���b�N�X
			D3DXMATRIX mtxParent;					//�e�̃}�g���b�N�X

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

			//�e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

			//�e�p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//�e�p�[�c�̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

			//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
			if (g_player.aModel[nCntModel].nIdxModelParent != -1)
			{
				mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_player.mtxWorld;	//�v���C���[�̃}�g���b�N�X��ݒ�
			}

			//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld,
				&g_player.aModel[nCntModel].mtxWorld,
				&mtxParent);

			//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

				//���f���̕`��
				g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// ���[�V�����̏�����
//=============================================================================
void InitMotion(void)
{
	//g_player.aMotionInfo[0].bLoop = false;
	//g_player.aMotionInfo[0].nNumKey = 6;
	//g_player.aMotionInfo[0].aKeyInfo[0].nFrame = 8;			
	//														//pos��			//rot��
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[0] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[1] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[2] = KeyPosRot(0.0, 0.0, 0.0,   0.0, -0.25f, 0.79f);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[3] = KeyPosRot(0.0, 0.0, 0.0,   0.0, -0.53f, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[4] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.25f, -0.79f);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[5] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.53f, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[6] = KeyPosRot(0.0, 0.0, 0.0,   0.06f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[7] = KeyPosRot(0.0, 0.0, 0.0,   -0.18f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[8] = KeyPosRot(0.0, 0.0, 0.0,   0.06f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[9] = KeyPosRot(0.0, 0.0, 0.0,   -0.18f, 0.0, 0.0);
}

//=============================================================================
// ���[�V����
//=============================================================================
void MotionPlayer(void)
{
	if (GetKeyboardPress(DIK_RETURN) == true)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
		{
		
		}
	}
}

//=============================================================================
// �L�[���
//=============================================================================
KEY KeyPosRot(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	KEY key;

	key.fPosX = posX;
	key.fPosY = posY;
	key.fPosZ = posZ;

	key.fRotX = rotX;
	key.fRotY = rotY;
	key.fRotZ = rotZ;

	return key;
}

//=============================================================================
// �G�t�F�N�g
//=============================================================================
void EffectPlayer(void)
{
	g_nEffect++;

	if (g_nEffect % 1 == 0)
	{
		for (int g_nEffect = 0; g_nEffect < 1; g_nEffect++)
		{
			// �p�x�̐ݒ�
			float fAngle = ((float)(rand() % 800)) / 100.0f;
			float fmove = (float)(rand() % 1 + 1);

			SetEffect(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 40.0f, g_player.pos.z),
				D3DXVECTOR3(sinf(fAngle) * fmove, 0, cosf(fAngle) * fmove / 2),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				10.0f,
				0.2f,
				0.25f);
		}
	}
}

//=============================================================================
// �v���C���[�̓����蔻��
//=============================================================================
bool HitPlayer(int nDamage)
{
	if (g_player.Life <= PLAYER_LIFE)
	{
		//�v���C���[�_���[�W�̃}�C�i�X
		g_player.Life -= nDamage;
		return true;
	}
	return false;
}

//=============================================================================
// ���f���̈ʒu���̎擾
//=============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}