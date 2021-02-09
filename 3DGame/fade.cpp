//=============================================================================
//
// �t�F�[�h�̏��� [fade.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "fade.h"								//�C���N���[�h�t�@�C��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//���_���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
FADE g_fade;									//�t�F�[�h�̏��
MODE g_modeNext;								//���̃��[�h(���)
D3DXCOLOR g_colorFade;							//�t�F�[�h�F

//=============================================================================
// �t�F�[�h�̏���������
//=============================================================================
HRESULT InitFade(MODE modeNext)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Z��0.0�Œ�
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//=============================================================================
// �t�F�[�h�̏I������
//=============================================================================
void UninitFade(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// �t�F�[�h�̍X�V����
//=============================================================================
void UpdateFade(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//�t�F�[�h�̏���
	if (g_fade == FADE_IN)			//�t�F�[�h�C��
	{
		g_colorFade.a -= 0.03f;

		if (g_colorFade.a <= 0.0f)	//0.0f�ɂȂ����牽�����Ȃ�
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
	}
	else if (g_fade == FADE_OUT)	//�t�F�[�h�A�E�g
	{
		g_colorFade.a += 0.03f;

		if (g_colorFade.a >= 1.0f)	//1.0f�ɂȂ�����t�F�[�h�C��
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;

			SetMode(g_modeNext);
		}
	}
	else if (g_fade == FADE_NONE)	//�������ĂȂ��Ƃ�
	{
		g_colorFade.a = 0.0f;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// �t�F�[�h�̕`�揈��
//=============================================================================
void DrawFade(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);	//�g��Ȃ��ꍇNULL

									//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// �t�F�[�h�̐ݒ�
//=============================================================================
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �t�F�[�h�̎擾
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}