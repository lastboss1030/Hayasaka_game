//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "manager.h"
#include "scene2D.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "renderer.h"		
#include "scene.h"			
#include "Input_Keyboard.h"				
#include "fade.h"					
#include "polygon.h"	
#include "fade.h"


//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CFade *CManager::m_pFade = NULL;
CGame *CManager::m_pGame = NULL;

CManager::MODE CManager::m_mode = MODE_GAME;		// ���݂̃��[�h
bool CManager::m_bPause = false;					// �|�[�Y�g�p�ϐ�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_�����O�N���X
	if (m_pRenderer == NULL)
	{
		// ���I�m��
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// ����������
			m_pRenderer->Init(hWnd, true);
		}
	}

	// �L�[�{�[�h
	if (m_pInputKeyboard == NULL)
	{
		// ���I�m��
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{
			// ����������
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// �t�F�[�h�̐���
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �G�̔j��
	CEnemy::Unload();

	// �w�i�̔j��
	CBg::Unload();

	// �v���C���[�̔j��
	CPlayer::Unload();

	// �����̔j��
	CExplosion::Unload();

	// �e�̔j��
	CBullet::Unload();

	//�����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �S�j��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �����_���[�̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//�Q�[����ʎ��݂̂Ƀ|�[�Y��`�恕�X�V�ł���
	if (CManager::m_mode == MODE_GAME)
	{
		//�|�[�Y�̐؂�ւ��I��
		if (m_bPause == false)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
			{
				m_bPause = true;
			}
		}

		//�|�[�Y�̐؂�ւ��I�t
		else if (m_bPause == true)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
			{
				m_bPause = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		// �����_���̕`�揈��
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// �|�[�Y
	m_bPause = false;

	// ���݂̃��[�h�̔j��
	switch (m_mode)
	{

	case MODE_GAME:		// �Q�[�����
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
	}

	// �V�[���S�j��
	CScene::ReleaseAll();

	// ���[�h�̐ݒ�
	m_mode = mode;

	switch (m_mode)
	{
	case MODE_GAME:
		if (m_pGame == NULL)
		{
			// ���I�m��
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				// ����������
				m_pGame->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
			}
		}
		break;
	}
}

//=============================================================================
// �|�[�Y�̏���
//=============================================================================
void CManager::SetPause(void)
{
	if (m_pFade->GetFade() == CFade::FADE_NONE)
	{
		m_bPause = m_bPause ? false : true;
	}
}