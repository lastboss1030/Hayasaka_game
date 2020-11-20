//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "effect.h"									//�C���N���[�h�t�@�C��
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EFFECT (4096)							//�G�t�F�N�g�ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL; 	//���_�o�b�t�@�ւ̃|�C���^
EFFECT g_aEffect[MAX_EFFECT];
D3DXVECTOR3 g_posEffect;							//�G�t�F�N�g�̈ʒu
D3DXVECTOR3 g_rotEffect;							//�G�t�F�N�g�̉�]�p
D3DXVECTOR3 g_moveEffect;							//�G�t�F�N�g�̈ړ���
float g_fLengthEffect;								//�G�t�F�N�g�̑Ίp���̒���
float g_fAngleEffect;								//�G�t�F�N�g�̑Ίp���̊p�x

//=============================================================================
// �G�t�F�N�g�̏���������
//=============================================================================
HRESULT InitEffect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�G�t�F�N�g�̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�Ίp���̒���
	g_fLengthEffect = sqrtf(40 * 40 + 50 * 50);

	//�Ίp���̊p�x
	g_fAngleEffect = atan2f(40, 50);

	//�����ʒu
	g_posEffect = D3DXVECTOR3(0, 0, 0.0f);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
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
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//=============================================================================
// �G�t�F�N�g�̏I������
//=============================================================================
void UninitEffect(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//=============================================================================
// �G�t�F�N�g�̍X�V����
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].col.a -= 0.04f;

		if (g_aEffect[nCntEffect].col.a <= 0.0f)	//0.0f�ɂȂ����牽�����Ȃ�
		{
			g_aEffect[nCntEffect].bUse = false;
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//�S�̒��_��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �G�t�F�N�g�̕`�揈��
//=============================================================================
void DrawEffect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//�\�[�X(�`�挳)�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//�f�X�e�B�l�[�V����(�`���)�̍������@�̐ݒ�

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		nCntVertex += 4;
	}

	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X(�`�挳)�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//�f�X�e�B�l�[�V����(�`���)�̍������@�̐ݒ�
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - 20.0f, pos.y + 20.0f, 0.0f);	//Z��0.0�Œ�
			pVtx[1].pos = D3DXVECTOR3(pos.x - 20.0f, pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + 20.0f, pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + 20.0f, pos.y - 20.0f, 0.0f);

			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}