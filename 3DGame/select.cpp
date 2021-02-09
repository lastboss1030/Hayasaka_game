//=============================================================================
//
// �I����ʂ̏��� [select.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "select.h"								//�C���N���[�h�t�@�C��
#include "input.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX (3)									//�e�N�X�`���ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	//���_���̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureSelect[MAX_TEX];	//�e�N�X�`���̃|�C���^
D3DXCOLOR g_colorSelect;							//�F
int g_nCounterAnimS;								//�A�j���[�V�����J�E���^�[
int g_nPatternAnimS;								//�A�j���[�V�����p�^�[��No
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
	g_colorSelect = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnterS = 0;
	//�A�j���[�V�����̏�����
	g_nCounterAnimS = 0;
	g_nPatternAnimS = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Select_BG.png", &g_apTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_apTextureSelect[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,	//�m�ۂ���o�b�t�@�T�C�Y
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
	//���̎擾
	int nFade = GetFade();
	g_nCounterAnimS++;		//�A�j���[�V�����J�E���^�[�X�V
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	if ((g_nCounterAnimS % 20) == 0)
	{
		//�A�j���[�V�����p�^�[�����X�V
		g_nPatternAnimS = (g_nPatternAnimS + 1) % 2;
	}

	//�e�N�X�`�����W���X�V
	pVtx[8].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimS), 1.0f);		//�e�N�X�`�����W
	pVtx[9].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimS), 0.0f);		//�A�j���[�V����
	pVtx[10].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimS), 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimS), 0.0f);

	//PRESS_ENTER�̐F�X�V
	pVtx[4].col = g_colorSelect;
	pVtx[5].col = g_colorSelect;
	pVtx[6].col = g_colorSelect;
	pVtx[7].col = g_colorSelect;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelect->Unlock();

	//PRESS_ENTER�̓_��
	if (nFade == FADE_NONE)
	{
		g_nCntEnterS++;

		if (g_nCntEnterS == 20)
		{
			g_colorSelect.a = 1.0f;
		}
		else if (g_nCntEnterS == 40)
		{
			g_colorSelect.a = 0.0f;
			g_nCntEnterS = 0;
		}
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//���ʉ�

			SetFade(FADE_OUT, MODE_GAME);	//�Q�[����ʂɐ؂�ւ�
		}
	}
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

	//�e�N�X�`���̐ݒ�
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
	}

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
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
	}
	else if (nCntSelect == 1)	//PRESS_ENTER
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(850, 200, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(850, 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1570, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1570, 50, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSelect->Unlock();
}