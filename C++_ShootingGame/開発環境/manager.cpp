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
#include "score.h"
#include "life.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "renderer.h"		
#include "scene.h"			
#include "Input_Keyboard.h"	
#include "sound.h"			
#include "fade.h"			
#include "number.h"			
#include "polygon.h"	
#include "fade.h"
#include "tutorial.h"
#include "logo.h"
#include "animation.h"
#include "pause.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CSound *CManager::m_pSound = NULL;
CFade *CManager::m_pFade = NULL;
CPause *CManager::m_pPause = NULL;

CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;

CManager::MODE CManager::m_mode = MODE_TITLE;		// ���݂̃��[�h
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

	// �T�E���h
	if (m_pSound == NULL)
	{
		// ���I�m��
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{
			// ����������
			m_pSound->Init(hWnd);
		}
	}
	//���S�̃e�N�X�`���̓ǂݍ���
	CLogo::Load();

	// �A�j���[�V�����摜���[�h
	CAnimation::Load();

	// �t�F�[�h�̐���
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �i���o�[�̔j��
	CNumber::Unload();

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

	// �A�j���[�V�����̔j��
	CAnimation::Unload();

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

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
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
	m_bPause = false;

	// ���݂̃��[�h�̔j��
	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:	// �`���[�g���A�����
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		// �Q�[�����
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
			
	case MODE_RESULT:	// ���U���g���
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	}

	// �V�[���S�j��
	CScene::ReleaseAll();

	// ���[�h�̐ݒ�
	m_mode = mode;

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{
			// ���I�m��
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{
				// ����������
				m_pTitle->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			}
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			// ���I�m��
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{
				// ����������
				m_pTutorial->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			}
		}
		break;

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

	case MODE_RESULT:
		if (m_pResult == NULL)
		{
			// ���I�m��
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				// ����������
				m_pResult->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
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