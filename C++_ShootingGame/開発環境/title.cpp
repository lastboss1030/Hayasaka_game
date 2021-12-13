//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "sound.h"
#include "animation.h"
#include "logo.h"
#include "bg.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

// �O���[�o���ϐ��錾
int nTimeTitle = 0;					// ����
int nTimeCounterTitle = 0;			// ���ԃJ�E���^�[
int nNexEnterCount = 0;				// ���SEnter�̐F�ύX
int nSaveTime = -1;					// �A�j���[�V�����p
int nSaveTime2 = -1;				// �A�j���[�V�����p
int nFlashing = 30;					// PressEnter�̓_��
bool g_bPressEnter = false;			// ���S�̕\������
bool g_bNextEnter = false;			// ���SEnter�̐F�ω��Ɏg�p
bool g_bButtonDownTitle = false;	// �A�Ŗh�~�p


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	//������
	nTimeTitle = 0;
	nTimeCounterTitle = 0;
	nNexEnterCount = 0;
	nSaveTime = -1;
	nSaveTime2 = -1;
	nFlashing = 30;
	g_bPressEnter = false;
	g_bNextEnter = false;
	g_bButtonDownTitle = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CTitle::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_002.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CTitle::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �w�i�̐���
//=============================================================================
CTitle *CTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTitle *pTitle = NULL;

	if (pTitle == NULL)
	{
		 // ���I�m��
		pTitle = new CTitle;

		if (pTitle != NULL)
		{
			// ����������
			pTitle->Init(pos, size);
		}
	}
	return pTitle;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// �^�C�g��BGM
	pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	// ���I�m��(�������̊m��)
	m_pScene2D = new CScene2D;

	// �摜��ǂݍ���
	Load();

	// ���@�w�e�X�g
//	CAnimation::Create(D3DXVECTOR3(400.0f, 60.0f, 0.0f), D3DXVECTOR3(100.0f, 80.0f,0.0f), 5, 1);

	// �^�C�g�����S����
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f), D3DXVECTOR3(700.0f, 300.0f, 0.0f), TITLE_UI_LOGO);

	// ����������
	m_pScene2D->Init(pos, size);

	// �e�N�X�`���̊��蓖��
	m_pScene2D->BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �摜�̔j��
	Unload();

	// �j������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �L�[�{�[�h�֌W
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ���[�h�֌W
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// �t�F�[�h�֌W
	CFade *pFade;
	pFade = CManager::GetFade();

	// �T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// Enter���S�̕\��
	if (g_bPressEnter == false)
	{
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f), D3DXVECTOR3(450.0f, 100.0f, 0.0f), TITLE_UI_PRESS_ENTER);		//Enter���S
		g_bPressEnter = true;
	}

	// �_�ŏ���
	if (g_bPressEnter == true)
	{
		// �J�E���g�A�b�v
		nNexEnterCount++;
		if (nNexEnterCount == nFlashing)
		{
			// �����ɂ���
			if (g_bNextEnter == true)
			{
				CLogo::TypeChangeCollar(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), TITLE_UI_PRESS_ENTER);		//�F�ω�
				g_bNextEnter = false;
			}
			// �s�����ɂ���
			else if (g_bNextEnter == false)
			{
				CLogo::TypeChangeCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_UI_PRESS_ENTER);		//�F�ω�
				g_bNextEnter = true;
			}

			// ���Z�b�g
			nNexEnterCount = 0;
		}
	}

	if (g_bButtonDownTitle == false)
	{
		// ��ʑJ��
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			// EnterSE
			pSound->Play(CSound::SOUND_LABEL_SE_ENTER);

			// ��ʑJ�ڂɊւ���ϐ�
			nTimeCounterTitle = 0;

			// �_�łɊւ���ϐ�
			nFlashing = 5;
			nNexEnterCount = 0;

			// �{�^�����������^�C����ۑ�
			nSaveTime = nTimeTitle;
			nSaveTime2 = 15;

			// �A�Ŗh�~
			g_bButtonDownTitle = true;
		}
	}

	// ���J�E���g��
	if ((nSaveTime + 1) == nTimeTitle && nSaveTime2 == nTimeCounterTitle)
	{
		//���[�h�ݒ�
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);	// �`���[�g���A����ʂ�
	}

	// �^�C����i�߂�
	nTimeCounterTitle++;

	// �J�E���^�[����萔��
	if (nTimeCounterTitle >= 30)
	{
		// �b����i�߂�
		nTimeTitle++;

		//�J�E���^�[��߂�
		nTimeCounterTitle = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}