//=============================================================================
//
// �A�j���[�V�������� [animasion.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CAnimation::m_pTexture[MAX_ANIMATION] = {};
int CAnimation::m_nUninitType = NULL;
int g_nUninitType;
CPlayer *CAnimation::m_pPlayer = NULL;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CAnimation::CAnimation(PRIORITY nPriority) : CScene2D(nPriority)
{
	m_nAnime = 0;
	m_nPatternAnime = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CAnimation::~CAnimation()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType)
{
	CScene2D::Init(pos, size);

	// ���
	m_pos = pos;
	m_nType = nType;

	// �A�j���[�V�����̏�����
	CScene2D::SetTex(1, 0);

	m_nAnime = nAnime;

	if (nType == 0)
	{
		m_fAnimeX = 0.2f;
		m_fAnimeY = 0.5f;
		m_nX = 5;
		m_nY = 2;
		m_floop = 1;
	}

	if (nType == 1)
	{
		m_fAnimeX = 1.0f;
		m_fAnimeY = 0.090909f;
		m_nX = 1;
		m_nY = 11;
		m_floop = 1;
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CAnimation::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CAnimation::Update(void)
{
	// �A�j���[�V�����̍X�V����
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// �A�j���[�V�����p�^�[���̍X�V
		m_nPatternAnime = (m_nPatternAnime + 1);

		// �A�j���[�V�����̍X�V
		CScene2D::SetAnimation(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// �J�E���^�[��i�߂�
	m_nCounterAnime++;

	// �X�V
	CScene2D::Update();

	// �A�j���[�V�������I�����Z�b�g
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// �J�E���^�[�̃��Z�b�g
		m_nPatternAnime = 0;

		// ���[�v�����Ȃ��Ƃ�
		if (m_floop == 0)
		{
			// �I�������
			Uninit();
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CAnimation::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);

	// �`��
	CScene2D::Draw();

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//==============================================================================
// ��������
//==============================================================================
CAnimation * CAnimation::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType)
{
	// �A�j���[�V����
	CAnimation *pAnimation = NULL;
	pAnimation = new CAnimation;

	// NULL�`�F�b�N
	if (pAnimation != NULL)
	{
		pAnimation->Init(pos, size, nAnime, nType);
	}

	// �e�N�X�`��������
	pAnimation->BindTexture(m_pTexture[nType]);

	return pAnimation;
}

//==============================================================================
// �e�N�X�`���̓ǂݍ���
//==============================================================================
HRESULT CAnimation::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ptranp.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magic.png", &m_pTexture[1]);

	return S_OK;
}

//==============================================================================
// �e�N�X�`���̔j��
//==============================================================================
void CAnimation::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIMATION; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �j������^�C�v
//==============================================================================
void CAnimation::UninitType(int nUninitType)
{
	g_nUninitType = nUninitType;
}

//==============================================================================
// �p�������O���[�o����������
//==============================================================================
void CAnimation::UninitTypeClear(void)
{
	g_nUninitType = 0;
}