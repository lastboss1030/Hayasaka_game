//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"									//�C���N���[�h�t�@�C��
#include "player.h"
#include "explosion.h"
#include "effect.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_BULLET		(256)	//�e�̍ő吔
#define HIT_WALL		(1200)
#define MAX_VERTEX		(4)
#define BULLET_SIZE_P	(70.0f)
#define BULLET_SIZE_E	(30.0f)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�̃e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//�e�̒��_�o�b�t�@�̃|�C���^
BULLET g_aBullet[MAX_BULLET];						//�e�̏��
int g_nCntEffect;

//=============================================================================
// �e�̏���������
//=============================================================================
HRESULT InitBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	//������
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].fWidth = 5.0f;
		g_aBullet[nCntBullet].fHeight = 5.0f;
		g_aBullet[nCntBullet].bUse = false;
	}

	//������
	g_nCntEffect = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);

		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

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
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
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
	Enemy *pEnemy = GetEnemy();
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 BulletV;

	//pBullet�̏�����
	pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->type == BULLETTYPE_PLAYER)	//�v���C���[�̒e��������
		{

			if (pBullet->bUse == true)	//�e���g�p����Ă�����
			{
				//�ʒu�̍X�V
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.52f, 0.811f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.07f,
					0.0f);

				// ��ʊO�`�F�b�N
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					//// �����A�j���[�V�����̔z�u
					//SetExplosion(pBullet->pos);

					g_nCntEffect++;

					if (g_nCntEffect % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							// �p�x�̐ݒ�
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							// �G�t�F�N�g�̐ݒ�
							SetEffect(pBullet->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.3f);
						}
					}

					// �e�̏�Ԃ�false�ɂ���
					pBullet->bUse = false;
				}
			}
		}


		if (pBullet->type == BULLETTYPE_ENEMY)	//�v���C���[�̒e��������
		{
			if (pBullet->bUse == true)	//�e���g�p����Ă�����
			{
				//�ʒu�̍X�V
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f),
					0.0f,
					10.0f,
					0.05f,
					0.0f);

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				// ��ʊO�`�F�b�N
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					//// �����A�j���[�V�����̔z�u
					//SetExplosion(pBullet->pos);

					g_nCntEffect++;

					if (g_nCntEffect % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							// �p�x�̐ݒ�
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							// �G�t�F�N�g�̐ݒ�
							SetEffect(pBullet->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.3f);
						}
					}

					// �e�̏�Ԃ�false�ɂ���
					pBullet->bUse = false;
				}
			}
		}
		if (pBullet->type == BULLETTYPE_EFFECT)	//�G�t�F�N�g�p
		{

			if (pBullet->bUse == true)	//�e���g�p����Ă�����
			{
				//�ʒu�̍X�V
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f,
					5.0f,
					0.05f,
					0.0f);

				//�G�t�F�N�g
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				// ��ʊO�`�F�b�N
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					// �e�̏�Ԃ�false�ɂ���
					pBullet->bUse = false;
				}
			}
		}

		for (int nCntEnemy = 0; nCntEnemy < 1; nCntEnemy++, pEnemy++)
		{
			if (pBullet->type == BULLETTYPE_PLAYER)
			{
				if (pBullet->bUse == true)
				{
					if (pEnemy->bUse == true)
					{
						if (pBullet->pos.x + BULLET_SIZE_P > pEnemy->pos.x - pEnemy->vtxMinEnemy.x &&
							pBullet->pos.x - BULLET_SIZE_P < pEnemy->pos.x + pEnemy->vtxMaxEnemy.x &&
 							pBullet->pos.z + BULLET_SIZE_P > pEnemy->pos.z - pEnemy->vtxMinEnemy.z &&
							pBullet->pos.z - BULLET_SIZE_P < pEnemy->pos.z + pEnemy->vtxMaxEnemy.z)
						{

							if (pPlayer->playertype == PLAYERTYPE_GOLEM)
							{
								HitEnemy(5);					//�S�[������
								pBullet->bUse = false;			//�g��������
								PlaySound(SOUND_LABEL_SE_HIT);	//���ʉ�
							}
							if (pPlayer->playertype == PLAYERTYPE_LEO)
							{
								HitEnemy(10);					//���I�I����
								pBullet->bUse = false;			//�g��������
								PlaySound(SOUND_LABEL_SE_HIT);	//���ʉ�
							}
							if (pPlayer->playertype == PLAYERTYPE_STALKER)
							{
								HitEnemy(3);					//�X�g�[�J�[�I����
								pBullet->bUse = false;			//�g��������
								PlaySound(SOUND_LABEL_SE_HIT);	//���ʉ�
							}

							g_nCntEffect++;

							if (g_nCntEffect % 10 == 0)
							{
								for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
								{
									// �p�x�̐ݒ�
									float fAngle = ((float)(rand() % 800)) / 100.0f;
									float fmove = (float)(rand() % 1 + 1);

									// �G�t�F�N�g�̐ݒ�
									SetEffect(pEnemy->pos,
										D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
										D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
										1.0f,
										5.0f,
										0.01f,
										0.3f);
								}
							}
						}
					}
				}
			}

			if (pBullet->type == BULLETTYPE_ENEMY)
			{
				if (pBullet->bUse == true)
				{
					if (pPlayer->bUse == true)
					{
						if (pBullet->pos.x + BULLET_SIZE_E > pPlayer->pos.x - pPlayer->minVecPlayer.x &&
							pBullet->pos.x - BULLET_SIZE_E < pPlayer->pos.x + pPlayer->maxVecPlayer.x &&
							pBullet->pos.z + BULLET_SIZE_E > pPlayer->pos.z - pPlayer->minVecPlayer.z &&
							pBullet->pos.z - BULLET_SIZE_E < pPlayer->pos.z + pPlayer->maxVecPlayer.z)
						{
							g_nCntEffect++;

							if (g_nCntEffect % 10 == 0)
							{
								//�v���C���[�Ƀ_���[�W
								HitPlayer(10);

								//���ʉ�
								PlaySound(SOUND_LABEL_SE_HIT);	

								//�X�R�A�}�C�i�X
								AddScore(-100);

								//�g��������
								pBullet->bUse = false;

								for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
								{
									// �p�x�̐ݒ�
									float fAngle = ((float)(rand() % 800)) / 100.0f;
									float fmove = (float)(rand() % 1 + 1);

									// �G�t�F�N�g�̐ݒ�
									SetEffect(pBullet->pos,
										D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
										D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
										1.0f,
										5.0f,
										0.01f,
										0.3f);
								}
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// �e�̕`�揈��
//=============================================================================
void DrawBullet(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aBullet[nCnt].mtxWorld._41 = 0.0f;
			g_aBullet[nCnt].mtxWorld._42 = 0.0f;
			g_aBullet[nCnt].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * MAX_VERTEX, 2);
		}
	}

	// �A���t�@�e�X�g�����̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�e�X�g�߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BULLETTYPE type)
{
	//�ϐ��錾
	BULLET *pBullet;
	pBullet = &g_aBullet[0];
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//�ʒu��ݒ�
			pBullet->pos = pos;

			//�ړ��ʂ�ݒ�
			pBullet->move = move;

			//���̐ݒ�
			pBullet->fWidth = fWidth;

			//�����̐ݒ�
			pBullet->fHeight = fHeight;

			//�e�̎��
			pBullet->type = type;

			//�e�̎g�p
			pBullet->bUse = true;

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �e�̈ʒu���̎擾
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}