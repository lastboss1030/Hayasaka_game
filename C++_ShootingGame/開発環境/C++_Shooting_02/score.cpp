//=============================================================================
//
// �X�R�A�̏���[score.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "score.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CScore::m_nScore = NULL;
CNumber *CScore::m_apNumber[MAX_NUMBER] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(PRIORITY nPriority) :CScene(nPriority)
{
	// ������
	int nScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ��������
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		// ���I�m��
		pScore = new CScore;

		if (pScore != NULL)
		{
			// �������������Ăяo��
			pScore->Init(pos, size);
		}
	}
	
	return pScore;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCntNumber = 0; nCntNumber < 8; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x + 15)), pos.y, 0.0f),size);
	}

	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < 8; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Uninit();

			delete m_apNumber[nCntNumber];
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < 8; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// �Z�b�g�X�R�A
//=============================================================================
void CScore::SetScore(int nScore)
{
	int aNumber[MAX_NUMBER];

	m_nScore = nScore;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		aNumber[nCntScore] = m_nScore / (int)powf(10.0f, 8.0f - (nCntScore + 1.0f));

		m_apNumber[nCntScore]->SetNumber(aNumber[nCntScore]);
	}
}

//=============================================================================
// �X�R�A���Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	SetScore(m_nScore);
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}
