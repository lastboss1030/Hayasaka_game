//=============================================================================
// 
// ���U���g���� [result.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "result.h"
#include "bg.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CResult::m_apTexture[] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{


}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CResult::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result.png", &m_apTexture[TEXTURETYPE_GAMECLEAR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GameOver.png", &m_apTexture[TEXTURETYPE_GAMEOVER]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CResult::Unload(void)
{
	for (int nCnt = 0; nCnt < TEXTURETYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̊J��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CResult *CResult::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CResult *pResult = NULL;

	if (pResult == NULL)
	{
		// ���I�m��
		pResult = new CResult;

		if (pResult != NULL)
		{
			// ����������
			pResult->m_pScene2D = NULL;
			pResult->Init(pos, size);
		}
	}
	return pResult;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ������
	m_pScene2D = NULL;

	// �摜�ǂݍ���
	Load();

	// �T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���U���gBGM
	pSound->Play(CSound::SOUND_LABEL_BGM_RESULT);

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create(CENTER_POS, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	}

	if (m_pScene2D != NULL)
	{
		// �e�N�X�`���̐ݒ�
		if (CGame::GetResult() == CGame::RESULTMODE_GAMECLEAR)
		{
			// �N���A���
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMECLEAR]);
		}
		else if (CGame::GetResult() == CGame::RESULTMODE_GAMEOVER)
		{
			// �Q�[���I�[�o���
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMEOVER]);
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// �摜�̔j��
	Unload();

	// �j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���[�h�֌W
	CManager::MODE pManager = CManager::GetMode();

	// �t�F�[�h�֌W
	CFade *pFade = CManager::GetFade(); 

	// �G���^�[���������Ƃ�
	if (pFade->GetFade() == CFade::FADE_NONE&& pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// �^�C�g���Ɉړ�
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}