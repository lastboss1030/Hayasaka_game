//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "bg.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "player.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBg::CBg(PRIORITY nPriority) : CScene(nPriority)
{
	// ������
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
		m_fSpeed[nCnt] = 0;
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CBg::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg103.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBg::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (m_apTexture[nCntBg] != NULL)
		{
			m_apTexture[nCntBg]->Release();
			m_apTexture[nCntBg] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CBg *pBg = NULL;

	if (pBg == NULL)
	{
		pBg = new CBg;

		if (pBg != NULL)
		{
			// ����������
			pBg->Init(pos, size);
		}
	}
	return pBg;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �`�揇�̐ݒ�
	m_apScene2D[0] = new CScene2D(PRIORITY_BGBEHING);
	m_apScene2D[1] = new CScene2D(PRIORITY_BGFRONT);
	m_apScene2D[2] = new CScene2D(PRIORITY_BGBEHING);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt]->Init(pos, size);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}

	// �w�i�X�s�[�h����
	m_fSpeed[0] = SCROL_SPPED01;
	m_fSpeed[1] = SCROL_SPPED02;
	m_fSpeed[2] = SCROL_SPPED03;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (m_apScene2D[nCntBg] != NULL)
		{
			m_apScene2D[nCntBg]->Uninit();
			m_apScene2D[nCntBg] = NULL;
		}
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBg::Update(void)
{
	// �L�[�{�[�h�֌W
	CInputKeyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (plnputKeyboard->GetPress(DIK_D) == true)
		{
			// �E�ɓ�����
			m_fPosTexU[nCnt] += m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_A) == true)
		{
			// ���ɓ�����
			m_fPosTexU[nCnt] -= m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_W) == true)
		{
			// ���ɓ�����
			m_fPosTexV[nCnt] -= m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_S) == true)
		{
			// ��ɓ�����
			m_fPosTexV[nCnt] += m_fSpeed[nCnt] * 0.5f;
		}

		m_fPosTexV[nCnt] -= m_fSpeed[nCnt];

		m_apScene2D[nCnt]->SetTexBg(m_fPosTexU[nCnt], m_fPosTexV[nCnt]);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBg::Draw(void)
{
	 
}
