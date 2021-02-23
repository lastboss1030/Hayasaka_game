//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY	(8)
#define MOVE_ENEMY (1.5f)
#define ENEMY_LIFE (100)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];							// ���f�����
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;				// �e���_���W�̍ŏ��l�A�ő�l
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};	// �e�N�X�`���ւ̃|�C���^

int g_nCntEffectGetE = 0;
int g_nCntEffectDeathE = 0;							//���S�G�t�F�N�g
int nCntShootEnemy = 0;								//���˃J�E���g
int	g_nEnemyCnt = 0;								//�G��
int nCntWait = 0;									//�ҋ@����

//=============================================================================
// �G�̏���������
//=============================================================================
HRESULT InitEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//������
		g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		g_aEnemy[nCntEnemy].nNumMatEnemy = NULL;
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = g_aEnemy[nCntEnemy].rot;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE;
		g_aEnemy[nCntEnemy].nIdx = -1;
		g_aEnemy[nCntEnemy].bUse = false;

		//X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/enemy.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCntEnemy].pBuffMatEnemy,
			NULL,
			&g_aEnemy[nCntEnemy].nNumMatEnemy,
			&g_aEnemy[nCntEnemy].pMeshEnemy)))
		{
			return E_FAIL;
		}
	}
	//�G�̐ݒu
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 600.0f));

	return S_OK;
}

//=============================================================================
// �G�̏I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���b�V���̔j��
		if (g_aEnemy[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		}

		//�}�e���A���̔j��
		if (g_aEnemy[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pMeshEnemy->Release();
			g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		}
	}
}

//=============================================================================
// �G�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = &g_aEnemy[0];
	BULLET *pBullet = GetBullet();

	// �ϐ��錾
	int nCntTrue = 0;
	int nFade = GetFade();

	nCntShootEnemy++;
	nCntWait++;

	if (nFade == FADE_NONE && pPlayer->bUse == true)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, nCntEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				MoveEnemy(nCntEnemy);

				//�v���C���[���������������
				if ((pPlayer->pos.x - pPlayer->minVecPlayer.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
					(pPlayer->pos.x + pPlayer->maxVecPlayer.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
					(pPlayer->pos.z - pPlayer->minVecPlayer.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
					(pPlayer->pos.z + pPlayer->maxVecPlayer.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
				{
					if (pPlayer->Life <= 500)
					{
						//�v���C���[����
						HitPlayer(100);
					}
				}

				if (pEnemy->nLife <= 0)
				{
					//�g��������
					pEnemy->bUse = false;

					//�e������
					DeleteShadow(pEnemy->nIdx);

					//���ʉ�
					PlaySound(SOUND_LABEL_SE_DEATH);

					g_nCntEffectDeathE++;

					if (g_nCntEffectDeathE % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 100; g_nCntEffect++)
						{
							//�p�x�̐ݒ�
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							//�G�t�F�N�g�̐ݒ�
							SetEffect(pEnemy->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.2f);
						}
					}
				}

				//�e����
				if ((nCntShootEnemy % 50) == 0)
				{
					SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + 75.0f, pEnemy->pos.z),
						D3DXVECTOR3(sinf(pEnemy->rot.y) * 10.0f, 0.0f, cosf(pEnemy->rot.y) * 10.0f),
						30.0f, 30.0f,
						BULLETTYPE_ENEMY);
				}

				//�g���Ă���J�E���g
				nCntTrue++;
			}
		}
	}

	if (pEnemy->nLife <= 0)
	{
		//�G���J�E���g-
		g_nEnemyCnt--;
	}

	//�G��S���|������
	if (g_nEnemyCnt <= 0)
	{
		g_nCntEffectDeathE++;
		if (g_nCntEffectDeathE == 40)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//���U���g��ʂɐ؂�ւ�
			g_nEnemyCnt++;
		}
	}
}

//=============================================================================
//�G�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aEnemy[nCntEnemy].pMeshEnemy->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �G�̈ړ�����
//=============================================================================
void MoveEnemy(int nCntEnemy)
{
	//�ϐ��錾
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 EnemyV;		//�v���C���[�Ɍ���

	//�ړI�̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}

	//�����̍X�V
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.05f;

	//���݂̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//�G���v���C���[������悤�ɂ���
	EnemyV = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	//�p�x�����߂�
	float fAngle = (float)atan2(EnemyV.x, EnemyV.z);

	//���@�ƒ��S�_�̑Ίp���̒���
	float fLength = sqrtf((EnemyV.x * EnemyV.x) + (EnemyV.z * EnemyV.z));

	//�v���C���[�̒ǔ�
	g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
	g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

	//�����̍X�V
	g_aEnemy[nCntEnemy].rotDest.y = fAngle;

	//�ʒu�̍X�V
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
	g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;

	//�e�̈ړ�
	SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//=============================================================================
// �G�̐ݒu
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos)
{
	Enemy *pEnemy = &g_aEnemy[0];

	//�G���J�E���g+
	g_nEnemyCnt++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//�A�C�e���̒��_
			VecEnemy(nCntEnemy);

			//���W�̐ݒ�
			g_aEnemy[nCntEnemy].pos = pos;

			//�g���Ă��邩�ǂ���
			g_aEnemy[nCntEnemy].bUse = true;

			//�e�̔z�u
			g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), 20.0f, 20.0f);

			break;
		}
	}
}

//=============================================================================
// �G�̒��_���W
//=============================================================================
void VecEnemy(int nCntEnemy)
{
	//�ϐ��錾
	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�S�Ă̒��_���r�����f���̍ŏ��l�A�ő�l�𔲂��o��
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}

		pVtxBuff += sizeFVF;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_aEnemy[0].pMeshEnemy->UnlockVertexBuffer();

	//0.0f��菬�����Ƃ�-1.0�{����
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
		{
		}
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
			g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
bool HitEnemy(int nDamage)
{
	Enemy *pEnemy = &g_aEnemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (pEnemy->nLife <= ENEMY_LIFE)
		{
			//�v���C���[�_���[�W�̃}�C�i�X
			pEnemy->nLife -= nDamage;
			return true;
		}
	}

	return false;
}

//=============================================================================
// ���f���̈ʒu���̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}