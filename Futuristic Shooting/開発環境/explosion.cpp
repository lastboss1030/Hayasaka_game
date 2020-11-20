//=============================================================================
//
// �������� [explosion.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"									//�C���N���[�h�t�@�C��

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EXPLOSION (256)								//�����ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; 	//���_�o�b�t�@�ւ̃|�C���^
int g_nCounterAnim;										//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;										//�A�j���[�V�����p�^�[��No
EXPLOSION g_aExplosion[MAX_EXPLOSION];					//�����̏��
D3DXVECTOR3 g_posExplosion;								//�����̈ʒu
D3DXVECTOR3 g_rotExplosion;								//�����̉�]�p
D3DXVECTOR3 g_moveExplosion;							//�����̈ړ���
float g_fLengthExplosion;								//�����̑Ίp���̒���
float g_fAngleExplosion;								//�����̑Ίp���̊p�x

//=============================================================================
// �����̏���������
//=============================================================================
HRESULT InitExplosion(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�����̏�����
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPattenAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//�Ίp���̒���
	g_fLengthExplosion = sqrtf(50 * 50 + 60 * 60);

	//�Ίp���̊p�x
	g_fAngleExplosion = atan2f(50, 60);

	//�����ʒu
	g_posExplosion = D3DXVECTOR3(200, 200, 0.0f);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

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
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);

		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// �����̏I������
//=============================================================================
void UninitExplosion(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion -> Release();
		g_pTextureExplosion = NULL;
	}
}

//=============================================================================
// �����̍X�V����
//=============================================================================
void UpdateExplosion(void)
{
	g_nCounterAnim++;		//�A�j���[�V�����J�E���^�[�X�V
	EXPLOSION *pExplosion;
	VERTEX_2D *pVtx;

	pExplosion = &g_aExplosion[0];

	if ((g_nCounterAnim % 4) == 0)
	{
		//�A�j���[�V�����p�^�[��No���X�V
		g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	}

	if (g_pVtxBuffExplosion != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W���X�V
		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
		{
			g_aExplosion[nCntExplosion].nCounterAnim++;

			if (g_aExplosion[nCntExplosion].bUse == true)
			{
				if (g_aExplosion[nCntExplosion].nCounterAnim > 6)
				{
					g_aExplosion[nCntExplosion].nCounterAnim = 0;						//�����A�j���[�V����
					g_aExplosion[nCntExplosion].nPattenAnim++;

					//���_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnim), 1.0f);	//�e�N�X�`�����W
					pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnim), 0.0f);	//�A�j���[�V����
					pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnim), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnim), 0.0f);

					if (g_aExplosion[nCntExplosion].nPattenAnim == 8)
					{
						g_aExplosion[nCntExplosion].nPattenAnim = 0;
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
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		nCntVertex += 4;
	}
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - 40.0f, pos.y + 40.0f, 0.0f);	//Z��0.0�Œ�
			pVtx[1].pos = D3DXVECTOR3(pos.x - 40.0f, pos.y - 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + 40.0f, pos.y + 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + 40.0f, pos.y - 40.0f, 0.0f);

			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPattenAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;

			break;
		}
		//�S�̒��_��i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}