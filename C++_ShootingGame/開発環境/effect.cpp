//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "effect.h" 
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "enemy.h"
#include "bullet.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;			// �e�N�X�`��

//=============================================================================
// �e�N�X�`���Ǎ�����
//=============================================================================
HRESULT CEffect::Load(void)
{
	// �f�o�C�X�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���j������
//=============================================================================
void CEffect::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(PRIORITY nPriority) : CScene2D(nPriority)
{
	// ������
	m_nLife = 70;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{


}

//=============================================================================
// ��������
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	if (pEffect != NULL)
	{
		pEffect->Init(pos, nLife ,size, col);
	}
	pEffect->BindTexture(m_pTexture);

	return pEffect;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// �J���[��ݒ�
	m_col = col;

	// �傫����ݒ�
	m_size = size;

	// ���C�t�̐ݒ�
	m_nLife = nLife;

	// CScene2D���Ăяo��
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(OBJTYPE_EFFECT);

	SetEffect(col, 1);

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CEffect::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CEffect::Update()
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �X�V
	CScene2D::Update();

	// ���C�t�����炷
	m_nLife--;

	// �G�t�F�N�g�����񂾂񓧖��ɂ���
	SetEffect(m_col, (float)m_nLife);

	// ���C�t��0�ȉ��ɂȂ����Ƃ�
	if (m_nLife <= 0)
	{
		Uninit();
	}

	// �͈͊O�̏ꍇ
	else if (pos.y <= 0)
	{
		Uninit();
	}
	else if (pos.y >= SCREEN_HEIGHT)
	{
		Uninit();
	}
	else if (pos.x <= 0)
	{
		Uninit();
	}
	else if (pos.x >= SCREEN_WIDTH)
	{
		Uninit();
	}
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CEffect::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���Z����
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// ���Z�������O��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

