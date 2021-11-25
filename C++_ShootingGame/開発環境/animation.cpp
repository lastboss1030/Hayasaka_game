//=============================================================================
//
// �A�j���[�V�������� [animasion.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "player.h"
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
		m_fAnimeX = 1.0f;			// �e�N�X�`������
		m_fAnimeY = 0.05f;			// �e�N�X�`������
		m_nX = 1;
		m_nY = 20;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 1)
	{
		m_fAnimeX = 0.333f;			// �e�N�X�`������
		m_fAnimeY = 0.333f;			// �e�N�X�`������
		m_nX = 3;
		m_nY = 3;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 2)
	{
		m_fAnimeX = 1.0f;			// �e�N�X�`������
		m_fAnimeY = 0.1f;			// �e�N�X�`������
		m_nX = 1;
		m_nY = 10;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 3)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.166f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 6;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 4)
	{
		m_fAnimeX = 0.5f;			// �e�N�X�`������
		m_fAnimeY = 0.2f;			// �e�N�X�`������
		m_nX = 2;
		m_nY = 5;
		m_fCriss = 1;				// ���[�v����

		// �F�ύX
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (nType == 5)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.5f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;				// ���[�v����
	}

	if (nType == 6)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.1666f;		// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;				// ���[�v����
	}

	if (nType == 7)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.3333f;		// �e�N�X�`������
		m_nX = 5;
		m_nY = 3;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 8)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.3333f;		// �e�N�X�`������
		m_nX = 5;
		m_nY = 3;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 9)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.5f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;				// ���[�v����
	}

	if (nType == 10)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.25f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 4;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 11)
	{
		m_fAnimeX = 0.20f;			// �e�N�X�`������
		m_fAnimeY = 0.3333f;		// �e�N�X�`������
		m_nX = 5;
		m_nY = 3;
		m_fCriss = 1;				// ���[�v�Ȃ�
	}

	if (nType == 12)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.5f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 13)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.1666f;		// �e�N�X�`������
		m_nX = 5;
		m_nY = 6;
		m_fCriss = 1;				// ���[�v�Ȃ�
	}

	if (nType == 14)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.25f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 4;
		m_fCriss = 0;				// ���[�v�Ȃ�
	}

	if (nType == 15)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.5f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;				// ���[�v����
	}

	if (nType == 16)
	{
		m_fAnimeX = 0.2f;			// �e�N�X�`������
		m_fAnimeY = 0.5f;			// �e�N�X�`������
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;				// ���[�v����
	}

	if (nType == 17)
	{
		m_fAnimeX = 0.5f;			// �e�N�X�`������
		m_fAnimeY = 0.1f;			// �e�N�X�`������
		m_nX = 2;
		m_nY = 10;
		m_fCriss = 1;				// ���[�v����
	}

	if (m_nType == 6 || m_nType == 9 || m_nType == 11 || m_nType == 5 || m_nType == 13)
	{
		// �F�ύX
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
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
	// �v���C���[�p�A�j���[�V����
	if (m_nType == 6 || m_nType == 9 || m_nType == 11 || m_nType == 5 || m_nType == 13)
	{
		// ���݂̈ʒu�̎擾
		D3DXVECTOR3 pos = m_pPlayer->GetPos();
		D3DXVECTOR3 size = m_pPlayer->GetSize();

		D3DXVECTOR3 objective = D3DXVECTOR3(pos.x + m_pos.x, pos.y + m_pos.y, pos.z);

		// �ړ�������
		SetPosition(objective, D3DXVECTOR3(5.0f,5.0f,0.0f));
	}

	// �r���ύXUninit
	if (m_nType == g_nUninitType)
	{
		m_fCriss = 0;
		m_nAnime = 1;
	}

	// �A�C�e���擾���̃G�t�F�N�g�A�j���[�V����
	if (m_nType == 7 || m_nType == 8)
	{
		// ���݂̈ʒu�̎擾
		D3DXVECTOR3 pos = m_pPlayer->GetPos();
		D3DXVECTOR3 size = m_pPlayer->GetSize();

		// �ړ�������
		SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(size.x, size.y, size.z));
	}

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
		if (m_fCriss == 0)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/frameeffects006test.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/animation000.png", &m_pTexture[10]);

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