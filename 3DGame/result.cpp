//=============================================================================
//
// ���U���g�̏��� [result.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "result.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_RESULT_TEX	(5)		//�e�N�X�`����
#define MAX_VERTEX		(4)		//���_��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXCOLOR g_colorR;											//�F
int g_nCounterAnimR;										//�A�j���[�V�����J�E���^�[
int g_nPatternAnimR;										//�A�j���[�V�����p�^�[��No
float g_fPatternAnimR[MAX_RESULT_TEX];						//�A�j���[�V�����p�^�[��
float g_nCountR;											//�J�E���g
int g_nResultState;											//���U���g���[�h
D3DXCOLOR ResultCol[MAX_RESULT_TEX];						//���U���g�J���[
float g_fMoveResultY[MAX_RESULT_TEX];						//�ړ���Y

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

	//�J���[�̏���
//	g_colorR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCountR = 0;

	//������
	g_nResultState = RESULT_GAMEOVER;
	ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo001.png", &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo001.png", &g_pTextureResult[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/haguruma.png", &g_pTextureResult[4]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_RESULT_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
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

		////���_�J���[�̐ݒ�
		//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�S�̒��_��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	//BGM
	PlaySound(SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void UninitResult(void)
{
	//�T�E���h��~
	StopSound();

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
	//�ϐ��錾
	VERTEX_2D *pVtx;
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = GetEnemy();
	int nFade = GetFade();

	//g_nCounterAnimR++;		//�A�j���[�V�����J�E���^�[�X�V

	if (pEnemy->nLife <= 0)
	{
		g_nResultState = RESULT_GAMECLEAR;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̍X�V
	if ((g_nCounterAnimR % 4) == 0)
	{
		// �e�N�X�`���̃p�^�[���̍X�V
		g_fPatternAnimR[0] += 0.0005f;

		// �e�N�X�`���̒��_���W�̍X�V
		pVtx[16].tex = D3DXVECTOR2(0.0f + g_fPatternAnimR[0], 1.0f + g_fMoveResultY[0]);
		pVtx[17].tex = D3DXVECTOR2(0.0f + g_fPatternAnimR[0], 0.0f + g_fMoveResultY[0]);
		pVtx[18].tex = D3DXVECTOR2(1.0f + g_fPatternAnimR[0], 1.0f + g_fMoveResultY[0]);
		pVtx[19].tex = D3DXVECTOR2(1.0f + g_fPatternAnimR[0], 0.0f + g_fMoveResultY[0]);
	}

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TEX; nCntResult++, pVtx += 4)
	{
		switch (g_nResultState)
		{
		case RESULT_GAMECLEAR:	//�Q�[���N���A

			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			ResultCol[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;

		case RESULT_GAMEOVER:	//�Q�[���I�[�o�[
			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[3] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			break;
		}
		pVtx[0].col = ResultCol[nCntResult];
		pVtx[1].col = ResultCol[nCntResult];
		pVtx[2].col = ResultCol[nCntResult];
		pVtx[3].col = ResultCol[nCntResult];
	}

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
			PlaySound(SOUND_LABEL_SE_ENTER);

			//�N���A��������
			if (g_nResultState == RESULT_GAMECLEAR)
			{
				SetFade(FADE_OUT, MODE_RANKING);	//�����L���O��ʂɐ؂�ւ�
			}

			//�Q�[���I�[�o�[��������
			if (g_nResultState == RESULT_GAMEOVER)
			{
				SetFade(FADE_OUT, MODE_TITLE);	//�����L���O��ʂɐ؂�ւ�
			}
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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
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

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else if (nCntResult == 1)	//PRESS_ENTER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(650, 950, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(650, 800, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1290, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1290, 800, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 2)	//�Q�[���I�[�o�[
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(480, 200, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 200, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 3)	//�Q�[���N���A
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(480, 200, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 200, 0.0f);

		//�J���[
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 4)	//����
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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