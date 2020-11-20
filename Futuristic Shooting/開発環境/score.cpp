//=============================================================================
//
// �X�R�A�̏��� [score.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "score.h"									//�C���N���[�h�t�@�C��

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_SCORE (8)								//�X�R�A�|���S���̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore[MAX_SCORE];					//�X�R�A�̈ʒu
float g_fLengthScore;								//�X�R�A�̑Ίp���̒���
float g_fAngleScore;								//�X�R�A�̑Ίp���̊p�x
int g_nScore;										//�X�R�A

//=============================================================================
// �X�R�A�̏���������
//=============================================================================
HRESULT InitScore(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�X�R�A�̏�����
	g_nScore = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureScore);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}
	
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pVtx += 4)
	{
		//�X�R�A�̈ʒu
		g_posScore[nCntScore] = D3DXVECTOR3(1460.0f + nCntScore * 60, 60, 0.0f);

		//�Ίp���̒���
		g_fLengthScore = sqrtf(30 * 30 + 50 * 50);

		//�Ίp���̊p�x
		g_fAngleScore = atan2f(30, 50);

		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos.x = (g_posScore[nCntScore].x - sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[0].pos.y = (g_posScore[nCntScore].y + cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = (g_posScore[nCntScore].x - sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[1].pos.y = (g_posScore[nCntScore].y - cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = (g_posScore[nCntScore].x + sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[2].pos.y = (g_posScore[nCntScore].y + cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = (g_posScore[nCntScore].x + sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[3].pos.y = (g_posScore[nCntScore].y - cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 200, 255, 255);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//=============================================================================
// �X�R�A�̏I������
//=============================================================================
void UninitScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//=============================================================================
// �X�R�A�̍X�V����
//=============================================================================
void UpdateScore(void)
{
	
}

//=============================================================================
// �X�R�A�̕`�揈��
//=============================================================================
void DrawScore(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	//�X�R�A�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 32);
}

//=============================================================================
// �X�R�A�v�Z
//=============================================================================
void AddScore(int nValue)
{
	int aNumber[MAX_SCORE];
	VERTEX_2D *pVtx;

	g_nScore += nValue;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nScore % 100000000 / 10000000;
	aNumber[1] = g_nScore % 10000000 / 1000000;
	aNumber[2] = g_nScore % 1000000 / 100000;
	aNumber[3] = g_nScore % 100000 / 10000;
	aNumber[4] = g_nScore % 10000 / 1000;
	aNumber[5] = g_nScore % 1000 / 100;
	aNumber[6] = g_nScore % 100 / 10;
	aNumber[7] = g_nScore % 10 / 1;

	//�e�N�X�`�����W�̍X�V
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		SetTextureScore(nCntScore,aNumber[nCntScore]);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̍X�V
//=============================================================================
void SetTextureScore(int nIdx, int nNumber)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 1.0f);		//�e�N�X�`�����W
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}