//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "result.h"									//�C���N���[�h�t�@�C��
#include "input.h"
#include "score.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[3] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//���_�o�b�t�@�ւ̃|�C���^
int g_nResultState;									//���U���g���[�h
D3DXCOLOR ResultCol[3];								//���U���g�J���[

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

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BG_Result.jpg", &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo001.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo001.png", &g_pTextureResult[2]);
	

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	//������
	g_nResultState = RESULT_GAMECLEAR;
	ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < 3; nCntResult++)
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

		//�S�̒��_��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	PlaySound(SOUND_LABEL_BGM_END);

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void UninitResult(void)
{
	StopSound();

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
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
	PLAYER *pPlayer;

	//���̎擾
	pPlayer = GetPlayer();
	int nFade = GetFade();
	
	if (pPlayer->nRemaining <= 0)
	{
		g_nResultState = RESULT_GAMEOVER;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < 3; nCntResult++, pVtx += 4)
	{
		switch (g_nResultState)
		{
		case RESULT_GAMECLEAR:	//�Q�[���N���A

			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			break;

		case RESULT_GAMEOVER:	//�Q�[���I�[�o�[
			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[1] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			ResultCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = ResultCol[nCntResult];
		pVtx[1].col = ResultCol[nCntResult];
		pVtx[2].col = ResultCol[nCntResult];
		pVtx[3].col = ResultCol[nCntResult];
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			SetFade(FADE_OUT, MODE_TITLE);	//�^�C�g����ʂɐ؂�ւ�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
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
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
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

	if (nCntResult == 0)		//�w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else if (nCntResult == 1)	//GAME_CLEAR
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(480, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 250, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 2)	//GAME_OVER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(480, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 250, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = ResultCol[nCntResult];
	pVtx[1].col = ResultCol[nCntResult];
	pVtx[2].col = ResultCol[nCntResult];
	pVtx[3].col = ResultCol[nCntResult];

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();
}