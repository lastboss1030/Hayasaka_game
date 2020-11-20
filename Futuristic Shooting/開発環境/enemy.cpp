//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"				//�C���N���[�h�t�@�C��
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include <stdlib.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TYPE_ENEMY (256)	//�G�̎��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posEnemy;										//�G�̈ʒu
D3DXVECTOR3 g_rotEnemy;										//�G�̉�]�p
D3DXVECTOR3 g_moveEnemy;									//�G�̈ړ���
ENEMY g_aEnemy[MAX_ENEMY];									//�G�̏��
float g_fLengthEnemy;										//�G�̑Ίp���̒���
float g_fAngleEnemy;										//�G�̑Ίp���̊p�x
int g_nCounterAnimEnemy;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnimEnemy;									//�A�j���[�V�����p�^�[��No
int g_nEnemyCnt;											//�G�̐�
int g_nCntEnemyDeath;										//�G���S���̃J�E���g

//=============================================================================
// �G�̏���������
//=============================================================================
HRESULT InitEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�A�j���[�V�����̏�����
	g_nCounterAnimEnemy = 0;
	g_nPatternAnimEnemy = 0;

	//�G�̐��̏�����
	g_nEnemyCnt = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy101.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy102.png", &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy103.png", &g_apTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &g_apTextureEnemy[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy005.png", &g_apTextureEnemy[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy006.png", &g_apTextureEnemy[6]);

	//������
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nLife = 0;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetVertexEnemy(nCntEnemy);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//=============================================================================
// �G�̏I������
//=============================================================================
void UninitEnemy(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntEnemy = 0; nCntEnemy < MAX_TYPE_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
	}
}

//=============================================================================
// �G�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	//�ϐ��錾
	g_nCounterAnimEnemy++;		//�A�j���[�V�����J�E���^�[�X�V
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	static int nCount;

	//pEnemy�̏�����
	pEnemy = &g_aEnemy[0];

	//�G�J�E���g
	nCount++;

	if ((g_nCounterAnimEnemy % 20) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimEnemy = (g_nPatternAnimEnemy + 1) % 2;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)	//�G���g�p����Ă���Ƃ�
		{
			//�G�̒e���o��
			if ((rand() % 120) == 0)
			{
				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f), 60, BULLETTYPE_ENEMY);
			}

			//�ʒu�̍X�V
			pEnemy->pos.x += pEnemy->move.x;

			//�G�̒��_���W�̐ݒ�
			SetVertexEnemy(nCntEnemy);

			//��ʊO�`�F�b�N
			if (pEnemy->pos.x < 20 || pEnemy->pos.x > SCREEN_WIDTH - 20)
			{
				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
				{
					//�[�ɂ�����t��
					g_aEnemy[nCnt].move *= -1.0f;
				}
			}
		}

		//�e�N�X�`�����W���X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimEnemy), 1.0f);		//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimEnemy), 0.0f);		//�A�j���[�V����
		pVtx[2].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimEnemy), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimEnemy), 0.0f);

		//�_���[�W�̐F�����ɖ߂�
		switch (g_aEnemy[nCntEnemy].state)
		{
		case ENEMYSTATE_DAMAGE:

			//�J�E���^�[�X�e�[�g�}�C�i�X
			g_aEnemy[nCntEnemy].nCounterState--;

			if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			{
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

				//���_�̐F��߂�
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		}
		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEnemy->Unlock();

	//�G��S���|������
	if (g_nEnemyCnt <= 0)
	{
		g_nCntEnemyDeath++;
		if (g_nCntEnemyDeath == 40)
		{
			SetFade(FADE_OUT, MODE_BOSSBATTLE);	//���U���g��ʂɐ؂�ւ�
			g_nEnemyCnt++;
		}
	}
}

//=============================================================================
// �G�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		//�S�i�߂�
		nCntVertex += 4;
	}
}

//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move)
{
	//�ϐ��錾
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;

	pEnemy = &g_aEnemy[0];

	//�G���v���X
	g_nEnemyCnt++;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//�G�̗̑�
			g_aEnemy[nCntEnemy].nLife = 3;

			//�ʒu��ݒ�
			pEnemy->pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - 30.0f, pEnemy->pos.y + 30.0f, 0.0f);	//Z��0.0�Œ�
			pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x - 30.0f, pEnemy->pos.y - 30.0f, 0.0f);	
			pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x + 30.0f, pEnemy->pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + 30.0f, pEnemy->pos.y - 30.0f, 0.0f);

			//�^�C�v�̐ݒ�
			pEnemy->nType = nType;

			//�G�̎g�p�̐ݒ�
			pEnemy->bUse = true;

			//�G�̈ړ��̐ݒ�
			pEnemy->move = move;

			break;
		}
		//�S�̒��_��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �G�̒��_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(int nIdx)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - 50.0f, g_aEnemy[nIdx].pos.y + 50.0f, 0.0f);	//�傫���ύX�͂���
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - 50.0f, g_aEnemy[nIdx].pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + 50.0f, g_aEnemy[nIdx].pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + 50.0f, g_aEnemy[nIdx].pos.y - 50.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
bool HitEnemy(int nIdx, int nDamage)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//�G���C�t�̃}�C�i�X
	g_aEnemy[nIdx].nLife -= nDamage;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEnemy[nIdx].nLife <= 0)
	{
		//������G�̍��W�ɏo��
		SetExplosion(g_aEnemy[nIdx].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 100.0f));

		//�������ʉ�(����)
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT);

		//�G���g��������
		g_aEnemy[nIdx].bUse = false;

		//�G��|�������_
		AddScore(200);

		//�G�̐��J�E���g�����炷
		g_nEnemyCnt--;

		return true;
	}
	else
	{
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 5;

		//�S�̒��_��i�߂�
		pVtx += nIdx * 4;
		
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}
//=============================================================================
// �G�̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}