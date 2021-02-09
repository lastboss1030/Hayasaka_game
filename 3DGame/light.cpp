//=============================================================================
//
// ���C�g�̏��� [light.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "light.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
D3DLIGHT9 g_light[3];	//���C�g�̏��

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir;				//�ݒ�p�����x�N�g��

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//----------���C�g1��----------//
	//���C�g���N���A����
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	g_light[0].Type = D3DLIGHT_DIRECTIONAL;	//���s����

	//���C�g�̊g�U����ݒ�
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	//���K������
	g_light[0].Direction = vecDir;

	//���C�g��ݒ肷��
	pDevice->SetLight(0, &g_light[0]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);


	//----------���C�g2��----------//
	//���C�g���N���A����
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	g_light[1].Type = D3DLIGHT_DIRECTIONAL;	//���s����

											//���C�g�̊g�U����ݒ�
	g_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, 0.8f, -0.8f);
	D3DXVec3Normalize(&vecDir, &vecDir);	//���K������
	g_light[0].Direction = vecDir;

	//���C�g��ݒ肷��
	pDevice->SetLight(1, &g_light[1]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(1, TRUE);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLight(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{

}