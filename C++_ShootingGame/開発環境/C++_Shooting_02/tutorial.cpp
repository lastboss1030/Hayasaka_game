//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "sound.h"
#include "animation.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CTutorial::Unload(void)
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
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTutorial *pTutorial = NULL;

	if (pTutorial == NULL)
	{
		// ���I�m��
		pTutorial = new CTutorial;

		if (pTutorial != NULL)
		{
			// ����������
			pTutorial->Init(pos, size);
		}
	}
	return pTutorial;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// �^�C�g��BGM
	pSound->Play(CSound::SOUND_LABEL_BGM_TUTORIAL);

	// ���I�m��(�������̊m��)
	m_pScene2D = new CScene2D;

	// �摜��ǂݍ���
	Load();

	// ����������
	m_pScene2D->Init(pos, size);

	// �e�N�X�`���̊��蓖��
	m_pScene2D->BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �摜�̔j��
	Unload();

	// �j������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
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

	// �^�C�g��BGM�I�t
	pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);

	// �G���^�[���������Ƃ�
	if (pFade->GetFade() == CFade::FADE_NONE && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// �^�C�g��BGM
		pSound->Play(CSound::SOUND_LABEL_SE_ENTER);

		// ���[�h�̐ݒ�
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}