//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "pause.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "animation.h"
#include "logo.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[PAUSE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(PRIORITY nPriority) :CScene(nPriority)
{
	float m_fMove = 0;
	float m_fAnim = 0;
	int	  m_ColorCnt = 0;
	int   m_nSelectCnt = 0;

	int nTimeGamePause = 0;
	int nTimeCounterPause = 0;
	int g_nPointerPauseX = 0;
	bool g_bButtonDownPause = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A�j���[�V�����摜���[�h
	CAnimation::Load();

	// �w�i
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSE01);

	// ���S
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), D3DXVECTOR3(190, 40, 0.0f), PAUSE_CONTINUE);

	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR3(190, 40, 0.0f), PAUSE_RESTART);

	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f), D3DXVECTOR3(120, 40, 0.0f), PAUSE_QUIT);

	// �p�g�����v
	//	CAnimasion::Create(D3DXVECTOR3(550.0f, 125.0f, 0.0f), 100, 80, 5, 18);

	// �l��Ԃ�
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		if (m_apScene2D[nCnt] == NULL)
		{
			m_apScene2D[nCnt] = new CScene2D;
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = m_apScene2D[m_nSelectCnt]->GetPosition();

	// �ړ��̐ݒ�
	D3DXVECTOR3 move = m_apScene2D[m_nSelectCnt]->GetMove();

	// �I������
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		// ��ɍs���Ƃ��ԍ������炷
		m_nSelectCnt--;

		if (m_nSelectCnt < PAUSE_CONTINUE)
		{
			// �R���e�B�j���[��菬�����Ȃ�����
			m_nSelectCnt = PAUSE_TITLE;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		// ���ɍs���Ƃ��ԍ��𑝂₷
		m_nSelectCnt++;

		if (m_nSelectCnt > PAUSE_TITLE)
		{
			// �^�C�g�����傫���Ȃ�����
			m_nSelectCnt = PAUSE_CONTINUE;
		}
	}

	// �I�����̏���
	for (int nCntPause = PAUSE_CONTINUE; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (m_nSelectCnt == nCntPause)
		{
			move.x++;

			// ���ݑI������Ă�����̂̐F��ς���
			m_apScene2D[nCntPause]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// ����ȊO�̓��l��ς���
			m_apScene2D[nCntPause]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		switch (m_nSelectCnt)
		{
		case PAUSE_CONTINUE:
			CManager::SetPause(false);

			break;

		case PAUSE_RETRY:
			CManager::SetPause(false);
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);

			break;

		case PAUSE_TITLE:
			CManager::SetPause(false);
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
			break;
		}
	}

	pos += move;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	m_apScene2D[0]->Draw();
}

//=============================================================================
// ��������
//=============================================================================
CPause *CPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���I�m��
	CPause* pPause;
	pPause = new CPause;

	// �����̌Ăяo��
	pPause->Init(pos, size);

	// �l��Ԃ�
	return pPause;
}