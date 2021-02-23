//=============================================================================
//
// �^�C���̏��� [time.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "time.h"
#include "Sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define LEFT_X			(895.0f)										//��X���W
#define RIGHT_X			(945.0f)										//�EX���W
#define HIGH_Y			(10.0f)											//��Y���W
#define LOW_Y			(90.0f)											//��Y���W
#define WIDTH_X			(45.0f)											//�X�R�A�̕�

#define MAX_VERTEX		(4)
#define MAX_PATTERN     (10)											//�A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(10)											//�A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)												//�A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * pExprosion->nPatternAnim)	//�e�N�X�`���̓���
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)					//U�̒l
#define UV_V			(1.0f / TEXTURE_Y)								//V�̒l

#define MAX_TIMER		(3)												//�X�R�A�e�N�X�`���̍ő�l

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	//�^�C�}�[�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;		//�^�C�}�[�̒��_�o�b�t�@�̃|�C���^
int g_nTime;									//�^�C�}�[
int g_nTimeCnt;									//�^�C�}�[�J�E���g
int g_aNumber[3];								//�c�莞�Ԃ��i�[
D3DXCOLOR g_TimeCol;							//�^�C�}�[�̐F

//=============================================================================
// �^�C�}�[�̏���������
//=============================================================================
HRESULT InitTime(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureTime);

	// �ϐ��̏�����
	g_nTime = 60;									//�^�C�}�[
	g_nTimeCnt = 60;								//�^�C�}�[�J�E���g
	g_TimeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�^�C�}�[�̐F

													//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TIMER, //�m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL)))
	{
		return E_FAIL;
	}

	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(LEFT_X + WIDTH_X * nCntTime, LOW_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LEFT_X + WIDTH_X * nCntTime, HIGH_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTime, LOW_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTime, HIGH_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_TimeCol;
		pVtx[1].col = g_TimeCol;
		pVtx[2].col = g_TimeCol;
		pVtx[3].col = g_TimeCol;

		//�c�莞�Ԃ̌v�Z
		g_aNumber[nCntTime] = (g_nTime / powf(10.0f, MAX_TIMER - (nCntTime + 1.0f)));

		//�e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 0.0f);

		//�X�R�A�̏������ɂ���
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

	return S_OK;
}

//=============================================================================
// �^�C�}�[�̏I������
//=============================================================================
void UninitTime(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//=============================================================================
// �^�C�}�[�̍X�V����
//=============================================================================
void UpdateTime(void)
{
	//�^�C�}�[�J�E���g�̌���
	g_nTimeCnt--;

	if (g_nTimeCnt == 0 && g_nTime != 0)
	{
		//�c�莞�Ԃ����炷
		g_nTime -= 1;

		//������
		g_nTimeCnt = 60;

		if (g_nTime == 0)
		{
			g_nTimeCnt = 10;
		}
	}
	else if (g_nTimeCnt == 0 && g_nTime == 0)
	{
		//�^�C�}�[��_�ł�����
		g_TimeCol.a = 0.0f;
		g_nTimeCnt = 20;

		//���ʉ�
		PlaySound(SOUND_LABEL_SE_TIMEUP);
	}
	else if (g_nTimeCnt == 10 && g_nTime == 0)
	{
		g_TimeCol.a = 1.0f;
	}

	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//�c�莞�Ԃ��i�[
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//�c�莞�Ԃ̌v�Z
		g_aNumber[nCntTime] = (g_nTime / powf(10.0f, MAX_TIMER - (nCntTime + 1.0f)));

		//�e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 0.0f);

		//���_�J���[�̍X�V
		pVtx[0].col = g_TimeCol;
		pVtx[1].col = g_TimeCol;
		pVtx[2].col = g_TimeCol;
		pVtx[3].col = g_TimeCol;

		//�X�R�A�̏������ɂ���
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// �^�C�}�[�̕`�揈��
//=============================================================================
void DrawTime(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�ϐ��̎󂯓n��
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�ƕ`��
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//�\��
		pDevice->SetTexture(0, g_pTextureTime);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//=============================================================================
// �^�C�}�[���̎擾
//=============================================================================
int GetTime(void)
{
	return g_nTime;
}