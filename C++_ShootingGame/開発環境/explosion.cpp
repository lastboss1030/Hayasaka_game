//=============================================================================
//
// �������� [explosion.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "Input_Keyboard.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CExplosion::CExplosion(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_nCountAnim = 0;
	m_nPatternAnim = 0;
	m_colorExplosion = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	// ������
	m_size = size;
	m_move = move;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`�����W�X�V
	SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CExplosion::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CExplosion::Update(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// �A�j���[�V�����J�E���^�[�X�V
	m_nCountAnim++;

	// �ړ��ʉ��Z
	pos += m_move;

	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(pos, m_size);

	if (m_nCountAnim % MAX_ANIMATION_COUNTER == 0)
	{
		// �p�^�[��No�X�V
		m_nPatternAnim += 1;

		// �e�N�X�`�����W�X�V
		SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

		if (m_nPatternAnim > 7)
		{
			// �I��
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	CExplosion *pExplosion = NULL;

	if (pExplosion == NULL)
	{
		// ���I�m��
		pExplosion = new CExplosion;

		if (pExplosion != NULL)
		{
			pExplosion->Init(pos, size, move);

			pExplosion->BindTexture(m_pTexture);
		}
	}
	return pExplosion;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CExplosion::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CExplosion::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}