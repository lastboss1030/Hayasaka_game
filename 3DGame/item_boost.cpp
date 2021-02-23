//=============================================================================
//
// �u�[�X�g�A�C�e���̏��� [item_boost.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "item_boost.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ITEMBOOST	(8)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
ItemBoost g_aItemBoost[MAX_ITEMBOOST];							// ���f�����
D3DXVECTOR3 g_vtxMinItemBoost, g_vtxMaxItemBoost;				// �e���_���W�̍ŏ��l�A�ő�l
LPDIRECT3DTEXTURE9 g_apTextureItemBoost[MAX_ITEMBOOST] = {};	// �e�N�X�`���ւ̃|�C���^
int g_nCntEffectGet = 0;

//=============================================================================
// �u�[�X�g�A�C�e���̏���������
//=============================================================================
HRESULT InitItemBoost(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ITEMBOOST; nCnt++)
	{
		//������
		g_aItemBoost[nCnt].pMeshItemBoost = NULL;
		g_aItemBoost[nCnt].pBuffMatItemBoost = NULL;
		g_aItemBoost[nCnt].nNumMatItemBoost = NULL;
		g_aItemBoost[nCnt].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aItemBoost[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].rotDest = g_aItemBoost[nCnt].rot;
		g_aItemBoost[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].nIdx = -1;
		g_aItemBoost[nCnt].bUse = false;

		//X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/healdrone.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aItemBoost[nCnt].pBuffMatItemBoost,
			NULL,
			&g_aItemBoost[nCnt].nNumMatItemBoost,
			&g_aItemBoost[nCnt].pMeshItemBoost)))
		{
			return E_FAIL;
		}
	}
	//�A�C�e���̐ݒu
	SetItemBoost(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	SetItemBoost(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));
	SetItemBoost(D3DXVECTOR3(0.0f, 0.0f, 200.0f));
	SetItemBoost(D3DXVECTOR3(0.0f, 0.0f, -200.0f));
	SetItemBoost(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	SetItemBoost(D3DXVECTOR3(-100.0f, 0.0f, 100.0f));
	SetItemBoost(D3DXVECTOR3(100.0f, 0.0f, -100.0f));
	SetItemBoost(D3DXVECTOR3(-100.0f, 0.0f, -100.0f));

	return S_OK;
}

//=============================================================================
// �u�[�X�g�A�C�e���̏I������
//=============================================================================
void UninitItemBoost(void)
{
	for (int nCnt = 0; nCnt < MAX_ITEMBOOST; nCnt++)
	{
		//���b�V���̔j��
		if (g_aItemBoost[nCnt].pBuffMatItemBoost != NULL)
		{
			g_aItemBoost[nCnt].pBuffMatItemBoost->Release();
			g_aItemBoost[nCnt].pBuffMatItemBoost = NULL;
		}

		//�}�e���A���̔j��
		if (g_aItemBoost[nCnt].pMeshItemBoost != NULL)
		{
			g_aItemBoost[nCnt].pMeshItemBoost->Release();
			g_aItemBoost[nCnt].pMeshItemBoost = NULL;
		}
	}
}

//=============================================================================
// �u�[�X�g�A�C�e���̍X�V����
//=============================================================================
void UpdateItemBoost(void)
{
	Player *pPlayer = GetPlayer();
	ItemBoost *pItemBoost = &g_aItemBoost[0];

	// �ϐ��錾
	int nCntTrue = 0;

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++, pItemBoost++)
	{
		if (pItemBoost->bUse == true)
		{
			//�v���C���[���������������
			if ((pPlayer->pos.x - pPlayer->minVecPlayer.x) < (pItemBoost->pos.x + pItemBoost->vtxMaxItemBoost.x) &&
				(pPlayer->pos.x + pPlayer->maxVecPlayer.x) > (pItemBoost->pos.x - pItemBoost->vtxMinItemBoost.x) &&
				(pPlayer->pos.z - pPlayer->minVecPlayer.z) < (pItemBoost->pos.z + pItemBoost->vtxMaxItemBoost.z) &&
				(pPlayer->pos.z + pPlayer->maxVecPlayer.z) > (pItemBoost->pos.z - pItemBoost->vtxMinItemBoost.z))
			{
				if (pPlayer->boost < 200)
				{
					//�u�[�X�g�c�ʑ���
					pPlayer->boost = 200;

					//���ʉ�
					PlaySound(SOUND_LABEL_SE_ITEM);

					//�g��������
					pItemBoost->bUse = false;

					//�e������
					DeleteShadow(pItemBoost->nIdx);

					g_nCntEffectGet++;

					if (g_nCntEffectGet % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							//�p�x�̐ݒ�
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							//�G�t�F�N�g�̐ݒ�
							SetEffect(pItemBoost->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.2f);
						}
					}
				}
			}
			//��]������
			pItemBoost->rot.y += 0.1f;

			//�g���Ă���J�E���g
			nCntTrue++;
		}
	}
}

//=============================================================================
//�u�[�X�g�A�C�e���̕`�揈��
//=============================================================================
void DrawItemBoost(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	pDevice = GetDevice();

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++)
	{
		if (g_aItemBoost[nCntItemBoost].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aItemBoost[nCntItemBoost].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItemBoost[nCntItemBoost].rot.y, g_aItemBoost[nCntItemBoost].rot.x, g_aItemBoost[nCntItemBoost].rot.z);
			D3DXMatrixMultiply(&g_aItemBoost[nCntItemBoost].mtxWorld, &g_aItemBoost[nCntItemBoost].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aItemBoost[nCntItemBoost].pos.x, g_aItemBoost[nCntItemBoost].pos.y, g_aItemBoost[nCntItemBoost].pos.z);
			D3DXMatrixMultiply(&g_aItemBoost[nCntItemBoost].mtxWorld, &g_aItemBoost[nCntItemBoost].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aItemBoost[nCntItemBoost].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aItemBoost[nCntItemBoost].pBuffMatItemBoost->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aItemBoost[nCntItemBoost].nNumMatItemBoost; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureItemBoost[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aItemBoost[nCntItemBoost].pMeshItemBoost->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �u�[�X�g�A�C�e���̐ݒu
//=============================================================================
void SetItemBoost(D3DXVECTOR3 pos)
{
	ItemBoost *pItemBoost = &g_aItemBoost[0];

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++)
	{
		if (g_aItemBoost[nCntItemBoost].bUse == false)
		{
			//�A�C�e���̒��_
			VecItemBoost(nCntItemBoost);

			//���W�̐ݒ�
			g_aItemBoost[nCntItemBoost].pos = pos;

			//�g���Ă��邩�ǂ���
			g_aItemBoost[nCntItemBoost].bUse = true;

			//�e�̔z�u
			g_aItemBoost[nCntItemBoost].nIdx = SetShadow(D3DXVECTOR3(g_aItemBoost[nCntItemBoost].pos.x, 0.0f, g_aItemBoost[nCntItemBoost].pos.z), 20.0f, 20.0f);

			break;
		}
	}
}

//=============================================================================
// �u�[�X�g�A�C�e���̒��_���W
//=============================================================================
void VecItemBoost(int nCntItemBoost)
{
	//�ϐ��錾
	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_aItemBoost[nCntItemBoost].pMeshItemBoost->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aItemBoost[nCntItemBoost].pMeshItemBoost->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aItemBoost[nCntItemBoost].pMeshItemBoost->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�S�Ă̒��_���r�����f���̍ŏ��l�A�ő�l�𔲂��o��
		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.x > vtx.x)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.x = vtx.x;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < vtx.x)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x = vtx.x;
		}

		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.y > vtx.y)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.y = vtx.y;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < vtx.y)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y = vtx.y;
		}

		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.z > vtx.z)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.z = vtx.z;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < vtx.z)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z = vtx.z;
		}
		
		pVtxBuff += sizeFVF;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_aItemBoost[0].pMeshItemBoost->UnlockVertexBuffer();

	//0.0f��菬�����Ƃ�-1.0�{����
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x *= -1.0f;
	{
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < 0.0f)
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z *= -1.0f;
	}
}