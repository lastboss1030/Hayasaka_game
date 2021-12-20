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
#include "particle.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

// �p�[�e�B�N��
CPlayer::PARTICLE CPlayer::m_aParticle[MAX_PARTICLE] = 
{
	20,										// ������
	8,										// ���x
	20.0f,									// ���a
	70,										// ����
	1.00f,									// ����
	628,									// �͈�
	1.0f,									// �p�x
	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),			// �F(R,G,B,A)
};

int CPlayer::m_nIndexParticle = 0;			// �ۑ��ԍ�
D3DXVECTOR3 g_pos;	// �擾�p
#define PARTICLE_FILE_NAME "particle.txt"	// �e�L�X�g

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_RIGHT	(5.0f)			// �v���C���[�̈ړ���(�E)
#define PLAYER_LEFT		(-5.0f)			// �v���C���[�̈ړ���(��)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;

	// �p�[�e�B�N���p���l������
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		m_aParticle[nCnt].nCreateNum = 20;							// ������
		m_aParticle[nCnt].nSpeed = 8;								// ���x
		m_aParticle[nCnt].fRadius = 20.0f;							// ���a
		m_aParticle[nCnt].nLife = 70;								// ����
		m_aParticle[nCnt].fInertia = 1.00f;							// ����
		m_aParticle[nCnt].nRange = 628;								// �͈�
		m_aParticle[nCnt].fAngle = 1.0f;							// �p�x
		m_aParticle[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F(R,G,B,A)
	}
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
	m_aParticle[m_nIndexParticle].nCreateNum = 20;	// ������
	m_aParticle[m_nIndexParticle].nSpeed = 8;		// ���x
	m_aParticle[m_nIndexParticle].fRadius = 20.0f;	// ���a
	m_aParticle[m_nIndexParticle].nLife = 70;		// ����
	m_aParticle[m_nIndexParticle].fInertia = 1.00f;	// ����
	m_aParticle[m_nIndexParticle].nRange = 628;		// �͈�
	m_aParticle[m_nIndexParticle].fAngle = 1.0;		// �p�x

	m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F(R,G,B,A)

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
	SetCol(m_aParticle[m_nIndexParticle].col);

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

	// ��ʒ[����
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

	// �p�[�e�B�N������
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		for (int nCntEffect = 0; nCntEffect < m_aParticle[m_nIndexParticle].nCreateNum; nCntEffect++)	// ��
		{
			//�p�x�̐ݒ�
			float fAngle = ((float)(rand() % m_aParticle[m_nIndexParticle].nRange - (m_aParticle[m_nIndexParticle].nRange / 2))) / 100 + D3DX_PI * m_aParticle[m_nIndexParticle].fAngle;	// �p�x
			float fmove = (float)(rand() % m_aParticle[m_nIndexParticle].nSpeed);	// ���x

			// �p�[�e�B�N������
			CParticle::Create(pos,																// ���W
				D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 5),						// �ړ���
				D3DXVECTOR3(m_aParticle[m_nIndexParticle].fRadius, m_aParticle[m_nIndexParticle].fRadius, 0),	// �T�C�Y
				m_aParticle[m_nIndexParticle].col,												// �J���[
				m_aParticle[m_nIndexParticle].nLife,											// ����
				m_aParticle[m_nIndexParticle].fInertia);										// ����
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ParticlePlayer.png", &m_pTexture);

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
	// �F(R)
	if (m_aParticle[m_nIndexParticle].col.r > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.r = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.r < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.r = 0.0f;
	}

	// �F(G)
	if (m_aParticle[m_nIndexParticle].col.g > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.g = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.g < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.g = 0.0f;
	}

	// �F(B)
	if (m_aParticle[m_nIndexParticle].col.b > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.b = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.b < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.b = 0.0f;
	}

	// ������
	if (m_aParticle[m_nIndexParticle].nCreateNum > 500)
	{
		m_aParticle[m_nIndexParticle].nCreateNum = 500;
	}
	if (m_aParticle[m_nIndexParticle].nCreateNum < 10)
	{
		m_aParticle[m_nIndexParticle].nCreateNum = 10;
	}

	// ���x
	if (m_aParticle[m_nIndexParticle].nSpeed > 30)
	{
		m_aParticle[m_nIndexParticle].nSpeed = 30;
	}
	if (m_aParticle[m_nIndexParticle].nSpeed < 2)
	{
		m_aParticle[m_nIndexParticle].nSpeed = 2;
	}

	// ���a
	if (m_aParticle[m_nIndexParticle].fRadius > 70)
	{
		m_aParticle[m_nIndexParticle].fRadius = 70;
	}
	if (m_aParticle[m_nIndexParticle].fRadius < 10)
	{
		m_aParticle[m_nIndexParticle].fRadius = 10;
	}

	// ����
	if (m_aParticle[m_nIndexParticle].nLife > 150)
	{
		m_aParticle[m_nIndexParticle].nLife = 150;
	}
	if (m_aParticle[m_nIndexParticle].nLife < 20)
	{
		m_aParticle[m_nIndexParticle].nLife = 20;
	}

	// ����
	if (m_aParticle[m_nIndexParticle].fInertia > 1.0f)
	{
		m_aParticle[m_nIndexParticle].fInertia = 1.0f;
	}
	if (m_aParticle[m_nIndexParticle].fInertia < 0.8f)
	{
		m_aParticle[m_nIndexParticle].fInertia = 0.8f;
	}

	// �͈�
	if (m_aParticle[m_nIndexParticle].nRange > 628)
	{
		m_aParticle[m_nIndexParticle].nRange = 628;
	}

	if (m_aParticle[m_nIndexParticle].nRange < 40)
	{
		m_aParticle[m_nIndexParticle].nRange = 40;
	}

	// �p�x
	if (m_aParticle[m_nIndexParticle].fAngle > 1.00f)
	{
		m_aParticle[m_nIndexParticle].fAngle = 1.00f;
	}
	if (m_aParticle[m_nIndexParticle].fAngle < -1.00)
	{
		m_aParticle[m_nIndexParticle].fAngle = 1.00f;
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
		m_aParticle[m_nIndexParticle].nCreateNum = 20;
		m_aParticle[m_nIndexParticle].nSpeed = 8;
		m_aParticle[m_nIndexParticle].fRadius = 20.0f;
		m_aParticle[m_nIndexParticle].nLife = 70;
		m_aParticle[m_nIndexParticle].fInertia = 1.00f;
		m_aParticle[m_nIndexParticle].nRange = 628;
		m_aParticle[m_nIndexParticle].fAngle = 1.0;

		m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ������
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_aParticle[m_nIndexParticle].nCreateNum += 10;
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_aParticle[m_nIndexParticle].nCreateNum -= 10;
	}

	// ���x
	if (pInputKeyboard->GetTrigger(DIK_T) == true)
	{
		m_aParticle[m_nIndexParticle].nSpeed += 1;
	}
	if (pInputKeyboard->GetTrigger(DIK_G) == true)
	{
		m_aParticle[m_nIndexParticle].nSpeed -= 1;
	}

	// ���a
	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_aParticle[m_nIndexParticle].fRadius += 2;
	}
	if (pInputKeyboard->GetTrigger(DIK_H) == true)
	{
		m_aParticle[m_nIndexParticle].fRadius -= 2;
	}

	// ����
	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_aParticle[m_nIndexParticle].nLife += 10;
	}
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		m_aParticle[m_nIndexParticle].nLife -= 10;
	}

	// ����
	if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		m_aParticle[m_nIndexParticle].fInertia += 0.01f;
	}
	if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		m_aParticle[m_nIndexParticle].fInertia -= 0.01f;
	}

	// �͈�
	if (pInputKeyboard->GetTrigger(DIK_Z) == true)
	{
		m_aParticle[m_nIndexParticle].nRange -= m_aParticle[m_nIndexParticle].nRange / 4;
	}

	if (pInputKeyboard->GetTrigger(DIK_C) == true)
	{
		m_aParticle[m_nIndexParticle].nRange = m_aParticle[m_nIndexParticle].nRange * 2;
	}

	// �p�x
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_aParticle[m_nIndexParticle].fAngle += 0.1f;
	}

	if (pInputKeyboard->GetTrigger(DIK_F) == true)
	{
		m_aParticle[m_nIndexParticle].fAngle -= 0.1f;
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �J���[
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// �J���[R
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD7) == true)
	{
		m_aParticle[m_nIndexParticle].col.r -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD9) == true)
	{
		m_aParticle[m_nIndexParticle].col.r += 0.1f;
	}

	// �J���[G
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
	{
		m_aParticle[m_nIndexParticle].col.g -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD6) == true)
	{
		m_aParticle[m_nIndexParticle].col.g += 0.1f;
	}

	// �J���[B
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD1) == true)
	{
		m_aParticle[m_nIndexParticle].col.b -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD3) == true)
	{
		m_aParticle[m_nIndexParticle].col.b += 0.1f;
	}

	// �ۑ��ԍ�
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_nIndexParticle = (m_nIndexParticle + 1) % MAX_PARTICLE;
	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_nIndexParticle = (m_nIndexParticle + MAX_PARTICLE - 1) % MAX_PARTICLE;
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
// �p�[�e�B�N���\���̎擾
//=============================================================================
CPlayer::PARTICLE CPlayer::GetParticle(void)
{
	return m_aParticle[m_nIndexParticle];
}

//=============================================================================
// �p�[�e�B�N���p�擾
// �F���擾
//=============================================================================
D3DXCOLOR CPlayer::GetCol(void)
{
	return m_aParticle[m_nIndexParticle].col;
}

//=============================================================================
// �p�[�e�B�N���������擾
//=============================================================================
int CPlayer::GetCreateNum(void)
{
	return m_aParticle[m_nIndexParticle].nCreateNum;
}

//=============================================================================
// �p�[�e�B�N�����x�擾
//=============================================================================
int CPlayer::GetSpeed(void)
{
	return m_aParticle[m_nIndexParticle].nSpeed;
}

//=============================================================================
// �p�[�e�B�N�����a�擾
//=============================================================================
float CPlayer::GetRadius(void)
{
	return m_aParticle[m_nIndexParticle].fRadius;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_aParticle[m_nIndexParticle].nLife;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_aParticle[m_nIndexParticle].fInertia;
}

//=============================================================================
// �p�[�e�B�N���͈͎擾
//=============================================================================
int CPlayer::GetRange(void)
{
	return m_aParticle[m_nIndexParticle].nRange;
}

//=============================================================================
// �p�[�e�B�N���p�x�擾
//=============================================================================
float CPlayer::GetAngle(void)
{
	return m_aParticle[m_nIndexParticle].fAngle;
}

//=============================================================================
// �p�[�e�B�N���ۑ��ԍ��擾
//=============================================================================
int CPlayer::GetIndexParticle(void)
{
	return m_nIndexParticle;
}


//=============================================================================
// �Z�[�u
//=============================================================================
void CPlayer::SaveData(void)
{
	FILE *pFile;

	// �t�@�C���J��
	pFile = fopen(PARTICLE_FILE_NAME, "w");

	if (pFile != NULL)
	{
		// �t�@�C�����J������
		for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
		{
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nCreateNum);		// ������
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nSpeed);			// ���x
			fprintf(pFile, "%.0f\n", m_aParticle[nCnt].fRadius);		// ���a
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nLife);			// ����

			fprintf(pFile, "%.2f\n", m_aParticle[nCnt].fInertia);		// ����
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nRange);			// �͈�
			fprintf(pFile, "%.1f\n", m_aParticle[nCnt].fAngle);			// �p�x

			fprintf(pFile, "%.1f %.1f %.1f\n", m_aParticle[nCnt].col.r, m_aParticle[nCnt].col.g, m_aParticle[nCnt].col.b);
			fprintf(pFile, "\n");
		}
	}

	fclose(pFile);
}

//=============================================================================
// ���[�h
//=============================================================================
void CPlayer::LoadData(void)
{
	FILE *pFile;

	// �t�@�C���J��
	pFile = fopen(PARTICLE_FILE_NAME, "r");

	if (pFile != NULL)
	{
		// �t�@�C�����J������
		for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nCreateNum);		// ������
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nSpeed);			// ���x
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fRadius);			// ���a
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nLife);			// ����

			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fInertia);			// ����
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nRange);			// �͈�
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fAngle);			// �p�x

			fscanf(pFile, "%f %f %f\n", &m_aParticle[nCnt].col.r, &m_aParticle[nCnt].col.g, &m_aParticle[nCnt].col.b);	// �F(R,G,B,A)
		}
	}

	fclose(pFile);
}
