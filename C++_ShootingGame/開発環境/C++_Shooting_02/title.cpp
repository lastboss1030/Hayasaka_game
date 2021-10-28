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

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &m_pTexture);

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

	// ���U���gBGM�I�t
	pSound->Stop(CSound::SOUND_LABEL_BGM_RESULT);

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
void CTitle::Draw(void)
{

}