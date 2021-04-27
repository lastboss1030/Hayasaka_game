//=============================================================================
//
// �^�C�g���̏��� [title.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "title.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TITLE_TEX	(4)					//�e�N�X�`����
#define MAX_VERTEX		(4)					//���_��

#define MOVE_TITLELOGO	(4.0f)				//�^�C�g�����S�̈ړ���
#define TITLELOGO_X		(1000.0f)			//�^�C�g�����S��
#define TITLELOGO_Y		(400.0f)			//�^�C�g�����S�̍���
#define HEIGHT_Y		(100.0f * -1.0f)	//�^�C�g�����S�̏������SY���W
#define TITLE_MARGIN	(3.5f)				//�^�C�g���]��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTitleLogo;								//�^�C�g�����S���W
D3DXCOLOR g_color;										//�F
float g_nCount;											//�J�E���g
float g_nCntWait;										//�҂�����
float g_nCountFade;										//�����t�F�[�h�J�E���g
int g_nCntAnimTitle;									//�A�j���[�V�����J�E���^�[
float g_fPatternAnimT[MAX_TITLE_TEX];					//�A�j���[�V�����p�^�[��
float g_fMoveTitleX[MAX_TITLE_TEX];						//�ړ���X
float g_fMoveTitleY[MAX_TITLE_TEX];						//�ړ���Y

//=============================================================================
// �^�C�g���̏���������
//=============================================================================
HRESULT InitTitle(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�J���[�̏���
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_posTitleLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_nCount = 0;
	g_nCntWait = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameTitle.png", &g_pTextureTitle[2]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TITLE_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_TEX; nCntTitle++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureTitle(nCntTitle);

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
	g_pVtxBuffTitle->Unlock();

	//BGM
	PlaySound(SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=============================================================================
// �^�C�g���̏I������
//=============================================================================
void UninitTitle(void)
{
	//�T�E���h��~
	StopSound();

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		if (g_pTextureTitle[nCntTexture] != NULL)
		{
			g_pTextureTitle[nCntTexture]->Release();
			g_pTextureTitle[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �^�C�g���̍X�V����
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;

	//���̎擾
	int nFade = GetFade();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//�^�C�g�����S�̈ʒu�X�V
	pVtx[8].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//�G���^�[�L�[���������Ƃ�
		if (nFade == FADE_IN)
		{
			//�t�F�[�h�C��
			SetFade(FADE_NONE, MODE_TITLE);
			g_posTitleLogo.y = SCREEN_HEIGHT / TITLE_MARGIN;
		}
		else if (g_posTitleLogo.y <= (SCREEN_HEIGHT / TITLE_MARGIN))
		{
			g_posTitleLogo.y = SCREEN_HEIGHT / TITLE_MARGIN;
		}
		else if (nFade == FADE_NONE && (g_posTitleLogo.y >= (SCREEN_HEIGHT / TITLE_MARGIN)))
		{
			//�������Ă��Ȃ��Ƃ�
			SetFade(FADE_OUT, MODE_TUTORIAL);
			g_nCount = 0;
		}
	}

	//�X�V����
	if (nFade == FADE_NONE && g_posTitleLogo.y <= (SCREEN_HEIGHT / TITLE_MARGIN))
	{
		//�^�C�g�����S�̈ړ��ʂ̍X�V
		g_posTitleLogo.y += MOVE_TITLELOGO;
	}

	//PRESS_ENTER�̐F�X�V
	pVtx[4].col = g_color;
	pVtx[5].col = g_color;
	pVtx[6].col = g_color;
	pVtx[7].col = g_color;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//PRESS_ENTER�̓_��
	if (nFade == FADE_NONE)
	{
		g_nCount++;

		if (g_nCount == 20)
		{
			g_color.a = 1.0f;
		}
		else if (g_nCount == 40)
		{
			g_color.a = 0.0f;
			g_nCount = 0;
		}
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_nCntWait++;

		if (g_nCount == 5)
		{
			g_color.a = 1.0f;
		}
		else if (g_nCount == 10)
		{
			g_color.a = 0.0f;
			g_nCount = 0;
		}

		if (nFade == FADE_NONE)
		{
			//���ʉ�
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetFade(FADE_OUT, MODE_TUTORIAL);	//�Q�[����ʂɐ؂�ւ�
		}
	}
}

//=============================================================================
// �^�C�g���̕`�揈��
//=============================================================================
void DrawTitle(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}

//=============================================================================
// �^�C�g���̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureTitle(int nCntTitle)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	if (nCntTitle == 0)	//�w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTitle == 1)	//PRESS_ENTER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(650, 900, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(650, 750, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1290, 900, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1290, 750, 0.0f);
	}
	else if (nCntTitle == 2)	//�^�C�g�����S
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTitleLogo.x - (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y + (TITLELOGO_Y / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTitleLogo.x + (TITLELOGO_X / 2), g_posTitleLogo.y - (TITLELOGO_Y / 2), 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}