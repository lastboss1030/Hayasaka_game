//=============================================================================
//
// DirectX01 [main.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define DIRECTINPUT_VERSION (0x0800)

#include "dinput.h"					//���͏����ɕK�v
#include <windows.h>
#include "d3dx9.h"					//�`�揈���ɕK�v
#include "xaudio2.h"				//�T�E���h�Đ��ɕK�v

//=============================================================================
// ���C�u�����̃����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")	//�`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")	//�g�����C�u����
#pragma comment(lib, "dxguid.lib")	//�R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib")	//���͏����ɕK�v
#pragma comment(lib, "winmm.lib")

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_WIDTH (1920)			//��ʂ̕�
#define SCREEN_HEIGHT (1080)		//��ʂ̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) //���_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g

//=============================================================================
// ���_�t�H�[�}�b�g�ɍ��킹���\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

//=============================================================================
// ���[�h�̎��
//=============================================================================
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A��
	MODE_SELECT,		//�I�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_MAX
}MODE;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);
MODE *GetMode(void);
void SetMode(MODE mode);

#endif
