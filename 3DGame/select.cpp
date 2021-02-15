//=============================================================================
//
// �I����ʂ̏��� [select.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "select.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX (4)									//�e�N�X�`���ő吔
#define MAX_VERTEX (4)								//���_��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureSelect[MAX_TEX];		//�e�N�X�`���̃|�C���^
D3DXCOLOR g_colorSelect[MAX_TEX];					//�F
int g_nCounterAnimS;								//�A�j���[�V�����J�E���^�[
int g_nPatternAnimS;								//�A�j���[�V�����p�^�[��No
int g_nState;										//�X�e�[�g
float g_nCntEnterS;

//=============================================================================
// �I����ʂ̏���������
//=============================================================================
HRESULT InitSelect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	g_colorSelect[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnterS = 0;

	//�A�j���[�V�����̏�����
	g_nCounterAnimS = 0;
	g_nPatternAnimS = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Select_BG.png", &g_apTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/golem_select.png", &g_apTextureSelect[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/leo_select.png", &g_apTextureSelect[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stalker_select.png", &g_apTextureSelect[3]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntSelect = 0; nCntSelect < MAX_TEX; nCntSelect++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTextureSelect(nCntSelect);

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
	g_pVtxBuffSelect->Unlock();

	return S_OK;
}

//=============================================================================
// �I����ʂ̏I������
//=============================================================================
void UninitSelect(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureSelect[nCntTexture] != NULL)
		{
			g_apTextureSelect[nCntTexture]->Release();
			g_apTextureSelect[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �I����ʂ̍X�V����
//=============================================================================
void UpdateSelect(void)
{
	VERTEX_2D *pVtx;
	int nFade = GetFade();
	Player *pPlayer;
	pPlayer = GetPlayer();

	g_nCounterAnimS++;

	if ((g_nCounterAnimS % 10) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimS = (g_nPatternAnimS + 1) % 8;
	}

	//�Z���N�g
	if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		//���ʉ�


		g_nState++;
		if (g_nState > 2)
		{
			g_nState = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) == true)
	{
		//���ʉ�


		g_nState--;
		if (g_nState < 0)
		{
			g_nState = 2;
		}
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < MAX_TEX; nCntSelect++, pVtx += 4)
	{
		switch (g_nState)
		{
		case SELECT_MENU_GOLEM:		 //�S�[�����I����

			//pVtx[4].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnimS), 1.0f);
			//pVtx[5].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnimS), 0.0f);
			//pVtx[6].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnimS), 1.0f);
			//pVtx[7].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnimS), 0.0f);

			g_colorSelect[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case SELECT_MENU_LEO:		//���I�I����
			g_colorSelect[1] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case SELECT_MENU_STALKER:	//�X�g�[�J�[�I����
			g_colorSelect[1] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = g_colorSelect[nCntSelect];
		pVtx[1].col = g_colorSelect[nCntSelect];
		pVtx[2].col = g_colorSelect[nCntSelect];
		pVtx[3].col = g_colorSelect[nCntSelect];
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			if (g_nState == SELECT_MENU_GOLEM)	//�S�[�����I����
			{
				//���艹


				//�L��������
				pPlayer->playertype = PLAYERTYPE_GOLEM;

				//�S�[�����g�p
				SetFade(FADE_OUT, MODE_GAME);

			}
			else if (g_nState == SELECT_MENU_LEO)	//���I�I����
			{
				//���艹


				//�L��������
				pPlayer->playertype = PLAYERTYPE_LEO;

				//���I�g�p
				SetFade(FADE_OUT, MODE_GAME);

			}
			else if (g_nState == SELECT_MENU_STALKER)	//�X�g�[�J�[�I����
			{
				//���艹


				//�L��������
				pPlayer->playertype = PLAYERTYPE_STALKER;

				//�X�g�[�J�[�g�p
				SetFade(FADE_OUT, MODE_GAME);

			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelect->Unlock();
}

//=============================================================================
// �I����ʂ̕`�揈��
//=============================================================================
void DrawSelect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}


//=============================================================================
// �I����ʂ̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTextureSelect(int nCntSelect)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntSelect * 4;

	if (nCntSelect == 0)	//�w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//�J���[
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	else if (nCntSelect == 1)	//�S�[����
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(120, 950, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(120, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(520, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(520, 350, 0.0f);

		//�J���[
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntSelect == 2)	//���I
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(750, 950, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(750, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1150, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1150, 350, 0.0f);

		//�J���[
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntSelect == 3)	//�X�g�[�J�[
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(1400, 950, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(1400, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1800, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1800, 350, 0.0f);

		//�J���[
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = g_colorSelect[nCntSelect];
	pVtx[1].col = g_colorSelect[nCntSelect];
	pVtx[2].col = g_colorSelect[nCntSelect];
	pVtx[3].col = g_colorSelect[nCntSelect];


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelect->Unlock();
}