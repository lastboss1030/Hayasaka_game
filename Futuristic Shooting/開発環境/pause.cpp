//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[5] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//���_�o�b�t�@�ւ̃|�C���^
bool g_bPause2 = false;							//�|�[�Y�����ǂ���
int g_State;									//�X�e�[�g
D3DXCOLOR PauseCol[5];							//�|�[�Y�J���[

//=============================================================================
// �|�[�Y�̏���������
//=============================================================================
HRESULT InitPause(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &g_pTexturePause[4]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 5,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL)))
	{
		return E_FAIL;
	}

	//������
	g_State = PAUSE_MENU_CONTINUE;
	PauseCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < 5; nCntPause++)
	{
		//���_���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//�e�N�X�`�����W
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_���W�̐ݒ�
		SetTexturePause(nCntPause);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�S�̒��_��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void UninitPause(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < 4; nCntTexture++)
	{
		if (g_pTexturePause[nCntTexture] != NULL)
		{
			g_pTexturePause[nCntTexture]->Release();
			g_pTexturePause[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;

	//�Z���N�g
	if (GetKeyboardTrigger(DIK_DOWN) == true)
	{
		g_State++;
		if (g_State > 2)
		{
			g_State = 0;
		}
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	if (GetKeyboardTrigger(DIK_UP) == true)
	{
		g_State--;
		if (g_State < 0)
		{
			g_State = 2;
		}
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPause = 0; nCntPause < 5; nCntPause++, pVtx += 4)
	{
		switch (g_State)
		{
		case PAUSE_MENU_CONTINUE:	//�R���e�B�j���[�I����

			PauseCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			PauseCol[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[4] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case PAUSE_MENU_RETRY:		//���g���C�I����
			PauseCol[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			PauseCol[4] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case PAUSE_MENU_QUIT:		//�I���I����
			PauseCol[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			PauseCol[4] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = PauseCol[nCntPause];
		pVtx[1].col = PauseCol[nCntPause];
		pVtx[2].col = PauseCol[nCntPause];
		pVtx[3].col = PauseCol[nCntPause];
	}

	//�G���^�[��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (g_State == PAUSE_MENU_RETRY)	//���g���C�I����
		{
			//�Q�[����ʂ�
			SetFade(FADE_OUT, MODE_GAME);

			//���艹
			PlaySound(SOUND_LABEL_SE_DECISION);
		}
		else if (g_State == PAUSE_MENU_QUIT)	//�I���I����
		{
			//�^�C�g����ʂ�
			SetFade(FADE_OUT, MODE_TITLE);

			//���艹
			PlaySound(SOUND_LABEL_SE_DECISION);
		}

		SetPause(false);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}
//=============================================================================
// �|�[�Y�̕`�揈��
//=============================================================================
void DrawPause(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntTexture = 0; nCntTexture < 5; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTexturePause[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//=============================================================================
// �|�[�Y�̃e�N�X�`���̐ݒ�
//=============================================================================
void SetTexturePause(int nCntPause)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntPause * 4;

	if (nCntPause == 0)			//���w�i
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//�J���[
		PauseCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	
	}
	 else if (nCntPause == 1)	//�|�[�Y���j��
	{
		 //���_���W
		pVtx[0].pos = D3DXVECTOR3(720, 850, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(720, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 850, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 250, 0.0f);

		//�J���[
		PauseCol[nCntPause] = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f);
	}
	else if (nCntPause == 2)	//�R���e�B�j���[
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(720, 400, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(720, 300, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 300, 0.0f);

		//�J���[
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntPause == 3)	//���g���C
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(720, 600, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(720, 500, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 600, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 500, 0.0f);

		//�J���[
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntPause == 4)	//�I��
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(720, 800, 0.0f);	//Z��0.0�Œ�
		pVtx[1].pos = D3DXVECTOR3(720, 700, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200, 800, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200, 700, 0.0f);

		//�J���[
		PauseCol[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = PauseCol[nCntPause];
	pVtx[1].col = PauseCol[nCntPause];
	pVtx[2].col = PauseCol[nCntPause];
	pVtx[3].col = PauseCol[nCntPause];


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}