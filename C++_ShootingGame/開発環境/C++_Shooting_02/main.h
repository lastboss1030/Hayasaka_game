//=============================================================================
//
// ���C������ [main.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dinput.h"					// ���͏����ɕK�v
#include "d3dx9.h"					// �`�揈���ɕK�v
#include "xaudio2.h"				// �T�E���h�Đ��ɕK�v

//=============================================================================
// ���C�u�����t�@�C���̃����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")	// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")	// �g�����C�u����
#pragma comment(lib, "dxguid.lib")	// �R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib, "winmm.lib")

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_WIDTH (1280)					// ��ʂ̕�
#define SCREEN_HEIGHT (720)					// ��ʂ̍���
#define WIDTH_HALF	(SCREEN_WIDTH/2)
#define HEIGHT_HALF	(SCREEN_HEIGHT/2)	

#define CENTER_POS	(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF,0.0f))

#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // ���_�t�H�[�}�b�g

#define NUM_VERTEX (4)				// ���_��
#define NUM_POLYGON (2)				// �|���S����
#define MAX_DATA (256)				// �G�̐�

//=============================================================================
// �O���錾
//=============================================================================
class CScene;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
int GetFPS(void);

#endif
