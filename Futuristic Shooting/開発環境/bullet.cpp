//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"									//�C���N���[�h�t�@�C��
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "Boss.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�̃e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//�e�̒��_�o�b�t�@�̃|�C���^
BULLET g_aBullet[MAX_BALLET];						//�e�̏��

//=============================================================================
// �e�̏���������
//=============================================================================
HRESULT InitBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Bullet000.png", &g_pTextureBullet);

	//������
	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BALLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(100, 200, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(100, 400, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(200, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200, 400, 0.0f);

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
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//=============================================================================
// �e�̏I������
//=============================================================================
void UninitBullet(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet ->Release();
		g_pVtxBuffBullet = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet ->Release();
		g_pTextureBullet = NULL;
	}
}

//=============================================================================
// �e�̍X�V����
//=============================================================================
void UpdateBullet(void)
{
	//�ϐ��錾
	BULLET *pBullet;
	ENEMY *pEnemy;
	BOSS *pBoss;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;

	//pBullet�̏�����
	pBullet = &g_aBullet[0];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)	//�e���g�p����Ă�����
		{
			//�ʒu�̍X�V
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - 15.0f, pBullet->pos.y + 15.0f, 0.0f);	//Z��0.0�Œ�
			pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - 15.0f, pBullet->pos.y - 15.0f, 0.0f);	//�傫���ύX�͂���
			pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + 15.0f, pBullet->pos.y + 15.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + 15.0f, pBullet->pos.y - 15.0f, 0.0f);

			// ��ʊO�`�F�b�N
			if (pBullet->pos.y <= 0)			//��ʏ�
			{
				pBullet->bUse = false;
			}
			if (pBullet->pos.y > SCREEN_HEIGHT)	//��ʉ�
			{
				pBullet->bUse = false;
			}

			if (pBullet->type == BULLETTYPE_PLAYER)	//�v���C���[�̒e��������
			{
				//�G�t�F�N�g
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

				//�G�Ƃ̓����蔻��
				pEnemy = GetEnemy();
				pBoss = GetBoss();

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)	//�G���g�p����Ă�����
					{
						//�e�ƓG���d�Ȃ���
						if (pEnemy->pos.x - 40.0f < g_aBullet[nCntBullet].pos.x &&
							pEnemy->pos.x + 40.0f > g_aBullet[nCntBullet].pos.x &&
							pEnemy->pos.y - 40.0f < g_aBullet[nCntBullet].pos.y &&
							pEnemy->pos.y + 40.0f > g_aBullet[nCntBullet].pos.y)
						{
							//�G�̓����蔻��
							HitEnemy(nCntEnemy, 1);

							//�e���g��������
							pBullet->bUse = false;
						}

						//�����`�F�b�N
						g_aBullet[nCntBullet].nLife--;
						if (g_aBullet[nCntBullet].nLife < 0)
						{
							//�����Ŕ���
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

							//�������ʉ�(����)
							PlaySound(SOUND_LABEL_SE_EXPLOSION2);

							//�e��false��
							g_aBullet[nCntBullet].bUse = false;
						}
					}
				}

				if (pBoss->bUse == true)
				{
					//�e�ƓG���d�Ȃ���
					if (pBoss->pos.x - 180.0f < g_aBullet[nCntBullet].pos.x &&
						pBoss->pos.x + 180.0f > g_aBullet[nCntBullet].pos.x &&
						pBoss->pos.y - 180.0f < g_aBullet[nCntBullet].pos.y &&
						pBoss->pos.y + 180.0f > g_aBullet[nCntBullet].pos.y)
					{
						//�G�̓����蔻��
						HitBoss(1);

						//�e���g��������
						pBullet->bUse = false;
					}
				}
			}

			if (pBullet->type == BULLETTYPE_ENEMY)	//�G�̒e��������
			{
				//�G�t�F�N�g
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

				//�v���C���[�Ƃ̓����蔻��
				pPlayer = GetPlayer();

				if (pPlayer->state == PLAYERSTATE_NORMAL)	//�v���C���[���g�p����Ă�����
				{
					//�e�ƃv���C���[���d�Ȃ���
					if (pPlayer->pos.x - 40.0f < g_aBullet[nCntBullet].pos.x &&
						pPlayer->pos.x + 40.0f > g_aBullet[nCntBullet].pos.x &&
						pPlayer->pos.y - 40.0f < g_aBullet[nCntBullet].pos.y &&
						pPlayer->pos.y + 40.0f > g_aBullet[nCntBullet].pos.y)
					{
						//�v���C���[�̓����蔻��
						HitPlayer(1);

						//�e���g��������
						pBullet->bUse = false;
					}
				}

				//�����`�F�b�N
				g_aBullet[nCntBullet].nLife--;
				if (g_aBullet[nCntBullet].nLife < 0)
				{
					//�����Ŕ���
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));

					//�������ʉ�(����)
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					//�e��false��
					g_aBullet[nCntBullet].bUse = false;
				}
			}
		}
		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �e�̕`�揈��
//=============================================================================
void DrawBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntVertex, 2);
		}
		//�S�i�߂�
		nCntVertex += 4;
	}
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,BULLETTYPE type)
{
	//�ϐ��錾
	BULLET *pBullet;
	pBullet = &g_aBullet[0];
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//�ʒu��ݒ�
			pBullet->pos = pos;

			//���_���W�̐ݒ�
			SetVertexBullet(nCntBullet);

			//�S�̒��_��i�߂�
			pVtx += 4;

			//�ړ��ʂ�ݒ�
			pBullet->move = move;

			//������ݒ�
			pBullet->nLife = nLife;

			//�e�̎g�p
			pBullet->bUse = true;

			//�e�̃^�C�v
			pBullet->type = type;

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �e�̒��_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(int nIdx)
{
	//�ϐ��錾
	BULLET *pBullet;
	VERTEX_2D *pVtx;
	pBullet = &g_aBullet[0];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//�S�̒��_��i�߂�
	pVtx += nIdx * 4;

	pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - 10.0f, pBullet->pos.y + 10.0f, 0.0f);	//Z��0.0�Œ�
	pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - 10.0f, pBullet->pos.y - 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + 10.0f, pBullet->pos.y + 10.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + 10.0f, pBullet->pos.y - 10.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}