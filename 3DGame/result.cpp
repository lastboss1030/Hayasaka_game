//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "result.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_RESULT_TEX (4)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posGameclear;									//�N���A���S���W
D3DXCOLOR g_colorR;											//�F
int g_nCounterAnimR;										//�A�j���[�V�����J�E���^�[
int g_nPatternAnimR;										//�A�j���[�V�����p�^�[��No
float g_nCountR;											//�J�E���g

//=============================================================================
// ���U���g�̏���������
//=============================================================================
HRESULT InitResult(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�A�j���[�V�����̏�����
	g_nCounterAnimR = 0;
	g_nPatternAnimR = 0;

	//�J���[�̏���
	g_colorR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCountR = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo001.png", &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player100.png", &g_pTextureResult[3]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TEX; nCntResult++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureResult(nCntResult);

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
	g_pVtxBuffResult->Unlock();

	//BGM

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void UninitResult(void)
{
	//�T�E���h��~

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		if (g_pTextureResult[nCntTexture] != NULL)
		{
			g_pTextureResult[nCntTexture]->Release();
			g_pTextureResult[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// ���U���g�̍X�V����
//=============================================================================
void UpdateResult(void)
{
	VERTEX_2D *pVtx;
	g_nCounterAnimR++;		//�A�j���[�V�����J�E���^�[�X�V

	//���̎擾
	int nFade = GetFade();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	if ((g_nCounterAnimR % 30) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimR = (g_nPatternAnimR + 1) % 2;
	}

	//�e�N�X�`�����W���X�V
	pVtx[12].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimR), 1.0f);		//�e�N�X�`�����W
	pVtx[13].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimR), 0.0f);		//�A�j���[�V����
	pVtx[14].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimR), 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimR), 0.0f);

	//PRESS_ENTER�̐F�X�V
	pVtx[4].col = g_colorR;
	pVtx[5].col = g_colorR;
	pVtx[6].col = g_colorR;
	pVtx[7].col = g_colorR;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	//PRESS_ENTER�̓_��
	if (nFade == FADE_NONE)
	{
		g_nCountR++;

		if (g_nCountR == 20)
		{
			g_colorR.a = 1.0f;
		}
		else if (g_nCountR == 40)
		{
			g_colorR.a = 0.0f;
			g_nCountR = 0;
		}
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//���ʉ�

			SetFade(FADE_OUT, MODE_TITLE);	//�^�C�g����ʂɐ؂�ւ�
		}
	}
}

//=============================================================================
// ���U���g�̕`�揈��
//=============================================================================
void DrawResult(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//=============================================================================
// ���U���g�̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureResult(int nCntResult)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntResult * 4;

	if (nCntResult == 0)	//�w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntResult == 1)	//PRESS_ENTER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(650, 950, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(650, 800, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1290, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1290, 800, 0.0f);
	}
	else if (nCntResult == 2)	//�N���A���S
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(480, 300, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(480, 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 300, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 100, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}