//=============================================================================
//
// �`���[�g���A������ [Tutorial.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "tutorial.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX (3)									//�e�N�X�`���ő吔
#define MAX_VERTEX (4)								//���_��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureTutorial[MAX_TEX];	//�e�N�X�`���̃|�C���^
D3DXCOLOR g_colorTutorial;							//�F
int g_nCounterAnimT;								//�A�j���[�V�����J�E���^�[
int g_nPatternAnimT;								//�A�j���[�V�����p�^�[��No
float g_nCntEnter;

//=============================================================================
// �`���[�g���A���̏���������
//=============================================================================
HRESULT InitTutorial(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	g_colorTutorial = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnter = 0;
	//�A�j���[�V�����̏�����
	g_nCounterAnimT = 0;
	g_nPatternAnimT = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &g_apTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_apTextureTutorial[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntTutorial = 0; nCntTutorial < MAX_TEX; nCntTutorial++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureTutorial(nCntTutorial);

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
	g_pVtxBuffTutorial->Unlock();

	//BGM
	PlaySound(SOUND_LABEL_BGM_TUTORIAL);

	return S_OK;
}

//=============================================================================
// �`���[�g���A���̏I������
//=============================================================================
void UninitTutorial(void)
{
	//�T�E���h��~
	StopSound();

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureTutorial[nCntTexture] != NULL)
		{
			g_apTextureTutorial[nCntTexture]->Release();
			g_apTextureTutorial[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̍X�V����
//=============================================================================
void UpdateTutorial(void)
{
	//���̎擾
	int nFade = GetFade();
	g_nCounterAnimT++;		//�A�j���[�V�����J�E���^�[�X�V
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	if ((g_nCounterAnimT % 20) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimT = (g_nPatternAnimT + 1) % 2;
	}

	//�e�N�X�`�����W���X�V
	pVtx[8].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimT), 1.0f);		//�e�N�X�`�����W
	pVtx[9].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimT), 0.0f);		//�A�j���[�V����
	pVtx[10].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimT), 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimT), 0.0f);

	//PRESS_ENTER�̐F�X�V
	pVtx[4].col = g_colorTutorial;
	pVtx[5].col = g_colorTutorial;
	pVtx[6].col = g_colorTutorial;
	pVtx[7].col = g_colorTutorial;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

	//PRESS_ENTER�̓_��
	if (nFade == FADE_NONE)
	{
		g_nCntEnter++;

		if (g_nCntEnter == 20)
		{
			g_colorTutorial.a = 1.0f;
		}
		else if (g_nCntEnter == 40)
		{
			g_colorTutorial.a = 0.0f;
			g_nCntEnter = 0;
		}
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//���ʉ�
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetFade(FADE_OUT, MODE_SELECT);	//�Q�[����ʂɐ؂�ւ�
		}
	}
}

//=============================================================================
// �`���[�g���A���̕`�揈��
//=============================================================================
void DrawTutorial(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}


//=============================================================================
// �`���[�g���A���̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureTutorial(int nCntTutorial)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTutorial * 4;

	if (nCntTutorial == 0)	//�w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTutorial == 1)	//PRESS_ENTER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(300, 1000, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(300, 900, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(920, 1000, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(920, 900, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}