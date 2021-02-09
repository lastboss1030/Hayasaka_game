//=============================================================================
//
// �J�����̏��� [camera.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MOVE_CAMERA (4.0f)			//�ړ���
#define CAMERA_DISTANCE (600.0f)	//����

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Camera g_camera;	//�J�����̏��

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	//������
	g_camera.posV = D3DXVECTOR3(0.0f, 300.0f, -CAMERA_DISTANCE);
	g_camera.posVDest = g_camera.posV;
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posRDest = g_camera.posR;
	g_camera.rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_camera.viewport.X = 0;
	g_camera.viewport.Y = 0;
	g_camera.viewport.Width = 1080;
	g_camera.viewport.Height = 1920;
	g_camera.viewport.MinZ = 0.0f;
	g_camera.viewport.MaxZ = 1.0f;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCamera(void)
{
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(DIK_Z) == true)	//Z��������
	{
		g_camera.rot.y -= 0.03f;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_C) == true)	//C��������
	{
		g_camera.rot.y += 0.03f;

		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
	}

	//�Ǐ]
	FollowCamera();
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�r���[�|�[�g�̐ݒ�
//	pDevice->SetViewport(&g_camera.viewport);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),								//��p
		(float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
		10.0f,												//��O�̕`��
		2000.0f);											//���̕`��

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// ���f���̒Ǐ]
//=============================================================================
void FollowCamera(void)
{
	Player *pPlayer = GetPlayer();

	g_camera.posR = pPlayer->pos;

	// �����_�̍X�V
	g_camera.posR.x = pPlayer->pos.x;	// �����_��X�����W�̌���
	g_camera.posR.z = pPlayer->pos.z;	// �����_��Z�����W�̌���
	g_camera.posR.y = pPlayer->pos.y;	// �����_��Y�����W�̌���

	// ���_�̍X�V
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * 400;		//���_��X�����W�̌���
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * 400;		//���_��Z�����W�̌���
	g_camera.posV.y = g_camera.posR.y + 200.0f;							//���_��Y�����W�̌���
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
Camera *GetCamera(void)
{
	return &g_camera;
}