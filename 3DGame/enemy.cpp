//=============================================================================
//
// �G�̏��� [enemy.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY	(8)			//�G�ő吔
#define MOVE_ENEMY	(2.0f)		//�G�̈ړ���
#define HIT_WALL	(760.0f)	//�ǂ̐ݒ�

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];								//���f�����
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};	//�e�N�X�`���ւ̃|�C���^
int g_nShootEnemy = 0;									//���˃J�E���g

//=============================================================================
// �G�̏���������
//=============================================================================
HRESULT InitEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//���̏�����
		g_aEnemy[nCnt].pMeshModel = NULL;
		g_aEnemy[nCnt].pBuffMatModel = NULL;
		g_aEnemy[nCnt].nNumMatModel = NULL;
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rotDest = g_aEnemy[nCnt].rot;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nIdx = -1;
		g_aEnemy[nCnt].bUse = false;

		//X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/enemy.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCnt].pBuffMatModel,
			NULL,
			&g_aEnemy[nCnt].nNumMatModel,
			&g_aEnemy[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		//�G�̒��_���W
		VecEnemy(nCnt);
	}

	//�G�̔z�u
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 500.0f));

	return S_OK;
}

//=============================================================================
// �G�̏I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//���b�V���̔j��
		if (g_aEnemy[nCnt].pBuffMatModel != NULL)
		{
			g_aEnemy[nCnt].pBuffMatModel->Release();
			g_aEnemy[nCnt].pBuffMatModel = NULL;
		}

		//�}�e���A���̔j��
		if (g_aEnemy[nCnt].pMeshModel != NULL)
		{
			g_aEnemy[nCnt].pMeshModel->Release();
			g_aEnemy[nCnt].pMeshModel = NULL;
		}
	}
}

//=============================================================================
// �G�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//�ϐ��錾
	int nCntTrue = 0;

	//���˃J�E���g��i�߂�
	g_nShootEnemy++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�G�̈ړ�����
			MoveEnemy(nCntEnemy);

			if ((g_nShootEnemy % 50) == 0)
			{
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 75, g_aEnemy[nCntEnemy].pos.z),
					D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y) * +8.0f, 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y) * + 8.0f),
					30.0f, 30.0,
					BULLETTYPE_ENEMY);
			}
		}

		//�ǂ̐ݒ�
		if (g_aEnemy[nCntEnemy].pos.x < -HIT_WALL)
		{
			//���[
			g_aEnemy[nCntEnemy].pos.x = -HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.x > HIT_WALL)
		{
			//�E�[
			g_aEnemy[nCntEnemy].pos.x = HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.z > HIT_WALL)
		{
			//��
			g_aEnemy[nCntEnemy].pos.z = HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.z < -HIT_WALL)
		{
			//��O
			g_aEnemy[nCntEnemy].pos.z = -HIT_WALL;
		}
	}


}

//=============================================================================
// �G�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();

	for (int nCntCoin = 0; nCntCoin < MAX_ENEMY; nCntCoin++)
	{
		if (g_aEnemy[nCntCoin].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntCoin].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntCoin].rot.y, g_aEnemy[nCntCoin].rot.x, g_aEnemy[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntCoin].mtxWorld, &g_aEnemy[nCntCoin].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntCoin].pos.x, g_aEnemy[nCntCoin].pos.y, g_aEnemy[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntCoin].mtxWorld, &g_aEnemy[nCntCoin].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntCoin].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aEnemy[nCntCoin].pMeshModel->DrawSubset(nCntMat);
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
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//���W�̐ݒ�
			g_aEnemy[nCntEnemy].pos = pos;

			//�e�N�X�`���̐ݒ�
			TexEnemy(nCntEnemy);

			//�g�p��Ԃ�true�ɂ���
			g_aEnemy[nCntEnemy].bUse = true;

			//�e�̔z�u
			g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//=============================================================================
// �G�̃e�N�X�`��
//=============================================================================
void TexEnemy(int nCntCoin)
{
	//�ϐ��錾
	D3DXMATERIAL *pMat;			//�}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�ϐ��̎󂯓n��
	pDevice = GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureEnemy[nCntMat]);
		}
	}
}

//=============================================================================
// �G�̒��_���W
//=============================================================================
void VecEnemy(int nCntCoin)
{
	//�ϐ��錾
	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_aEnemy[nCntCoin].pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntCoin].pMeshModel->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aEnemy[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���_���r���ă��f���̍ŏ��l�ő�l�𔲂��o��
		if (g_aEnemy[nCntCoin].vtxMinEnemy.x > vtx.x)
		{
			//X���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.x < vtx.x)
		{
			//X���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntCoin].vtxMinEnemy.y > vtx.y)
		{
			//Y���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.y < vtx.y)
		{
			//Y���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntCoin].vtxMinEnemy.z > vtx.z)
		{
			//Z���W�̍ŏ��l
			g_aEnemy[nCntCoin].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.z < vtx.z)
		{
			//Z���W�̍ő�l
			g_aEnemy[nCntCoin].vtxMinEnemy.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_aEnemy[0].pMeshModel->UnlockVertexBuffer();
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	//�ϐ��錾
	Enemy *pEnemy = &g_aEnemy[0];
	bool bCollision = false;		//�����������ǂ���

	for (int nCntCoin = 0; nCntCoin < MAX_ENEMY; nCntCoin++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//�x�N�g��
			pEnemy->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pEnemy->aPos[0].x, 0.0f, pPos->z - pEnemy->aPos[0].z);
			pEnemy->aVecB[1] = D3DXVECTOR3(pPos->x - pEnemy->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pEnemy->aPos[1].z);
			pEnemy->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pEnemy->aPos[2].x, 0.0f, pPos->z - pEnemy->aPos[2].z);
			pEnemy->aVecB[3] = D3DXVECTOR3(pPos->x - pEnemy->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pEnemy->aPos[3].z);
		}
	}

	return bCollision;
}

//=============================================================================
// �G�̈ʒu���̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}