//=============================================================================
//
// �v���C���[����[player.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"		
#include "Input_Keyboard.h"	
#include "game.h"			
#include "fade.h"			
#include "effect.h"
#include "particle.h"
#include "bullet.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
D3DXCOLOR CPlayer::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

// �p�[�e�B�N���p
int CPlayer::m_nCreateNum = NULL;	// ������
int CPlayer::m_nSpeed = NULL;		// ���x
float CPlayer::m_fRadius = NULL;	// ���a
int CPlayer::m_nLife = NULL;		// ����
float CPlayer::m_fInertia = NULL;	// ����
float CPlayer::m_fAngle = NULL;		// �p�x
int CPlayer::m_nRange = NULL;		// �͈�

D3DXVECTOR3 g_pos;	// �擾�p
#define PARTICLE_FILE_NAME "particle.txt"	// �e�L�X�g

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_RIGHT	(5.0f)			// �v���C���[�̈ړ���(�E)
#define PLAYER_LEFT		(-5.0f)			// �v���C���[�̈ړ���(��)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
bool g_bBumpedPlayer = false;		//���G���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;

	// �p�[�e�B�N���p���l������
	m_nCreateNum = 20;
	m_nSpeed = 8;
	m_fRadius = 20.0f;
	m_nLife = 70;
	m_fInertia = 1.00f;
	m_nRange = 628;
	m_fAngle = 1.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		 // ���I�m��
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			 // ����������
			pPlayer->Init(pos, size);

			// �e�N�X�`���̊��蓖��
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �T�C�Y
	m_size = size;

	// �p�[�e�B�N���p���l������
	m_nCreateNum = 20;
	m_nSpeed = 8;
	m_fRadius = 20.0f;
	m_nLife = 70;
	m_fInertia = 1.00f;
	m_nRange = 628;
	m_fAngle = 1.0;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// Scene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �J���[�ݒ�
	SetCol(m_col);

	// �ړ�����
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		m_move.y -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		m_move.y += 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		m_move.x -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		m_move.x += 2.0f;
	}

	// ��������
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	// ���W�̐ݒ�
	pos += m_move;

	// ��ʊO����
	if (pos.x - m_size.x / 2 <= 0.0f)
	{
		pos.x = m_size.x / 2;
	}
	if (pos.x + m_size.x / 2 >= SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}
	if (pos.y - m_size.y / 2 <= 0.0f)
	{
		pos.y = m_size.y / 2;
	}
	if (pos.y + m_size.y / 2 >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - m_size.y / 2;
	}

	// ���l�ύX
	ChangeParticle();

	// �������
	PlayerLimit();

	// ���W���
	g_pos = pos;

	// �e�̔���
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		CBullet::Create(D3DXVECTOR3(pos.x + 0.0f, pos.y - 10.0f, 1.0f), 
						D3DXVECTOR3(0.0f, -8.0f, 0.0f), 
						D3DXVECTOR3(15.0f, 15.0f, 0.0f));
	}

	// �p�[�e�B�N������
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		for (int nCntEffect = 0; nCntEffect < m_nCreateNum; nCntEffect++)						// ��
		{
			//�p�x�̐ݒ�
			float fAngle = ((float)(rand() % m_nRange - (m_nRange / 2))) / 100 + D3DX_PI * m_fAngle;					// �p�x
			float fmove = (float)(rand() % m_nSpeed);											// ���x

			// �p�[�e�B�N������
			CParticle::Create(pos,																// ���W
				D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 5),						// �ړ���
				D3DXVECTOR3(m_fRadius, m_fRadius, 0),											// �T�C�Y
				D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f),										// �J���[
				m_nLife,																		// ����
				m_fInertia);																	// ����
		}
	}

	// �Z�[�u
	if (pInputKeyboard->GetPress(DIK_F1) == true)
	{
		SaveData();
	}

	// ���[�h
	if (pInputKeyboard->GetPress(DIK_F2) == true)
	{
		LoadData();
	}

	// ���W�̐ݒ�
	CScene2D::SetPosition(pos, m_size);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// Scene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �������
//=============================================================================
void CPlayer::PlayerLimit(void)
{
	// ��
	if (m_col.r > 1.0f)
	{
		m_col.r = 1.0f;
	}
	else if (m_col.r < 0.0f)
	{
		m_col.r = 0.0f;
	}

	// ��
	if (m_col.g > 1.0f)
	{
		m_col.g = 1.0f;
	}
	else if (m_col.g < 0.0f)
	{
		m_col.g = 0.0f;
	}

	// ��
	if (m_col.b > 1.0f)
	{
		m_col.b = 1.0f;
	}
	else if (m_col.b < 0.0f)
	{
		m_col.b = 0.0f;
	}

	// ������
	if (m_nCreateNum > 500)
	{
		m_nCreateNum = 500;
	}
	if (m_nCreateNum < 10)
	{
		m_nCreateNum = 10;
	}

	// ���x
	if (m_nSpeed > 30)
	{
		m_nSpeed = 30;
	}
	if (m_nSpeed < 2)
	{
		m_nSpeed = 2;
	}

	// ���a
	if (m_fRadius > 70)
	{
		m_fRadius = 70;
	}
	if (m_fRadius < 10)
	{
		m_fRadius = 10;
	}

	// ����
	if (m_nLife > 150)
	{
		m_nLife = 150;
	}
	if (m_nLife < 20)
	{
		m_nLife = 20;
	}

	// ����
	if (m_fInertia > 1.0f)
	{
		m_fInertia = 1.0f;
	}
	if (m_fInertia < 0.8f)
	{
		m_fInertia = 0.8f;
	}

	// �͈�
	if (m_nRange > 628)
	{
		m_nRange = 628;
	}

	if (m_nRange < 40)
	{
		m_nRange = 40;
	}

	// �p�x
	if (m_fAngle > 1.00f)
	{
		m_fAngle = 1.00f;
	}
	if (m_fAngle < -1.00)
	{
		m_fAngle = 1.00f;
	}

}

//=============================================================================
// �p�[�e�B�N�����l�ύX
//=============================================================================
void CPlayer::ChangeParticle(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���Z�b�g
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		// �p�[�e�B�N���p���l������
		m_nCreateNum = 20;
		m_nSpeed = 8;
		m_fRadius = 20.0f;
		m_nLife = 70;
		m_fInertia = 1.00f;
		m_nRange = 628;
		m_fAngle = 1.0;

		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ������
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_nCreateNum += 10;
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_nCreateNum -= 10;
	}

	// ���x
	if (pInputKeyboard->GetTrigger(DIK_T) == true)
	{
		m_nSpeed += 1;
	}
	if (pInputKeyboard->GetTrigger(DIK_G) == true)
	{
		m_nSpeed -= 1;
	}

	// ���a
	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_fRadius += 2;
	}
	if (pInputKeyboard->GetTrigger(DIK_H) == true)
	{
		m_fRadius -= 2;
	}

	// ����
	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_nLife += 10;
	}
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		m_nLife -= 10;
	}

	// ����
	if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		m_fInertia += 0.01f;
	}
	if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		m_fInertia -= 0.01f;
	}

	// �͈�
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_nRange -= 10;
	}

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_nRange += 10;
	}

	// �p�x
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_fAngle += 0.05f;
	}

	if (pInputKeyboard->GetTrigger(DIK_F) == true)
	{
		m_fAngle -= 0.05f;
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �J���[
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// �J���[R
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD7) == true)
	{
		m_col.r -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD9) == true)
	{
		m_col.r += 0.1f;
	}

	// �J���[G
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
	{
		m_col.g -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD6) == true)
	{
		m_col.g += 0.1f;
	}

	// �J���[B
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD1) == true)
	{
		m_col.b -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD3) == true)
	{
		m_col.b += 0.1f;
	}
}

//=============================================================================
// �v���C���[�̈ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �v���C���[�̈ʒu�擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetPlayerPos(void)
{
	return g_pos;
}

//=============================================================================
// �ʒu��ݒ�
//=============================================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2D�ɏ���n��
	SetPosition(pos, m_size);
}

//=============================================================================
// �v���C���[�̈ړ��ʂ�ݒ�
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}


//=============================================================================
// �p�[�e�B�N���p�擾
// �F���擾
//=============================================================================
D3DXCOLOR CPlayer::GetCol(void)
{
	return m_col;
}

//=============================================================================
// �������擾
//=============================================================================
int CPlayer::GetCreateNum(void)
{
	return m_nCreateNum;
}

//=============================================================================
// �p�[�e�B�N�����x�擾
//=============================================================================
int CPlayer::GetSpeed(void)
{
	return m_nSpeed;
}

//=============================================================================
// �p�[�e�B�N�����a�擾
//=============================================================================
float CPlayer::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
// �p�[�e�B�N�����C�t�擾
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
// �p�[�e�B�N���͈͎擾
//=============================================================================
int CPlayer::GetRange(void)
{
	return m_nRange;
}

//=============================================================================
// �p�[�e�B�N���p�x�擾
//=============================================================================
float CPlayer::GetAngle(void)
{
	return m_fAngle;
}


//=============================================================================
// �Z�[�u
//=============================================================================
void CPlayer::SaveData(void)
{
	FILE *pFile;
	pFile = fopen(PARTICLE_FILE_NAME, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "%d\n", m_nCreateNum);		// ������
		fprintf(pFile, "%d\n", m_nSpeed);			// ���x
		fprintf(pFile, "%.0f\n", m_fRadius);		// ���a
		fprintf(pFile, "%d\n", m_nLife);			// ����

		fprintf(pFile, "%.2f\n", m_fInertia);		// ����
		fprintf(pFile, "%d\n", m_nRange);			// �͈�
		fprintf(pFile, "%.2f\n", m_fAngle);			// �p�x

		fprintf(pFile, "%.1f\n", m_col.r);			// R
		fprintf(pFile, "%.1f\n", m_col.g);			// G
		fprintf(pFile, "%.1f\n", m_col.b);			// B
	}
	else
	{ // �t�@�C�����J���Ȃ�������
		printf("�t�@�C�����J���܂���ł���\n");
	}

	fclose(pFile);
}

//=============================================================================
// ���[�h
//=============================================================================
void CPlayer::LoadData(void)
{
	FILE *pFile;

	pFile = fopen(PARTICLE_FILE_NAME, "r");

	if (pFile != NULL)
	{
		// �t�@�C�����J������
		fscanf(pFile, "%d\n", &m_nCreateNum);		// ������
		fscanf(pFile, "%d\n", &m_nSpeed);			// ���x
		fscanf(pFile, "%f\n", &m_fRadius);			// ���a
		fscanf(pFile, "%d\n", &m_nLife);			// ����

		fscanf(pFile, "%f\n", &m_fInertia);			// ����
		fscanf(pFile, "%d\n", &m_nRange);			// �͈�
		fscanf(pFile, "%f\n", &m_fAngle);			// �p�x

		fscanf(pFile, "%f\n", &m_col.r);			// R
		fscanf(pFile, "%f\n", &m_col.g);			// G
		fscanf(pFile, "%f\n", &m_col.b);			// B
	}

	else
	{ // �t�@�C�����J���Ȃ�������
		printf("�t�@�C�����J���܂���ł���\n");
	}

	fclose(pFile);
}
