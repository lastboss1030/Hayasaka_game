//=============================================================================
//
// �����̏��� [explosion.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PATTERN     (8)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(8)					// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)					// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔
#define MAX_EXPLOSION	(64)				// �e�̍ő�l

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// �e�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// �e�̒��_�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];				// �e�̏��
int g_nCounterAnimE;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnimE;									//�A�j���[�V�����p�^�[��No

//=============================================================================
// �����̏���������
//=============================================================================
HRESULT InitExplosion(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion001.png", &g_pTextureExplosion);

	// �����̏��̏�����
	for (int nCntExprosion = 0; nCntExprosion < MAX_EXPLOSION; nCntExprosion++)
	{
		g_aExplosion[nCntExprosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExprosion].nCounterAnimE = 0;
		g_aExplosion[nCntExprosion].nPatternAnimE = 0;
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, g_aExplosion[nCntExplosion].pos.z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// �����̏I������
//=============================================================================
void UninitExplosion(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void UpdateExplosion(void)
{
	g_nCounterAnimE++;		//�A�j���[�V�����J�E���^�[�X�V
	Explosion *pExplosion;
	VERTEX_3D *pVtx;

	pExplosion = &g_aExplosion[0];

	if ((g_nCounterAnimE % 5) == 0)
	{
		//�A�j���[�V�����p�^�[��No���X�V
		g_nPatternAnimE = (g_nPatternAnimE + 1) % 8;
	}

	if (g_pVtxBuffExplosion != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W���X�V
		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
		{
			g_aExplosion[nCntExplosion].nCounterAnimE++;

			if (g_aExplosion[nCntExplosion].bUse == true)
			{
				if (g_aExplosion[nCntExplosion].nCounterAnimE > 4)
				{
					g_aExplosion[nCntExplosion].nCounterAnimE = 0;						//�����A�j���[�V����
					g_aExplosion[nCntExplosion].nPatternAnimE++;

					//���_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnimE), 1.0f);	//�e�N�X�`�����W
					pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnimE), 0.0f);	//�A�j���[�V����
					pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnimE), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnimE), 0.0f);

					if (g_aExplosion[nCntExplosion].nPatternAnimE == 8)
					{
						g_aExplosion[nCntExplosion].nPatternAnimE = 0;
						g_aExplosion[nCntExplosion].bUse = false;						//�g������̂Ă�
					}
				}
			}
			//�S�̒��_��i�߂�
			pVtx += 4;
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// �����̕`�揈��
//=============================================================================
void DrawExplosion(void)
{
	// �ϐ��錾
	Explosion *pExplosion = &g_aExplosion[0];
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxScaling, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@��K�p
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pExplosion->mtxWorld);

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion->mtxWorld, NULL, &mtxView);
			g_aExplosion->mtxWorld._41 = 0.0f;
			g_aExplosion->mtxWorld._42 = 0.0f;
			g_aExplosion->mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);
			D3DXMatrixMultiply(&pExplosion->mtxWorld, &pExplosion->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
	// �W���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//�\���̂̃|�C���^
	Explosion *pExprosion = &g_aExplosion[0];

	//�����̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			//�ʒu�̐ݒ�
			pExprosion->pos = pos;

			//�g�p���Ă��邩�ǂ���
			pExprosion->bUse = true;

			break;
		}
	}
}
