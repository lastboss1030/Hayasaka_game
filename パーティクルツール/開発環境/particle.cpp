//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "scene.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;
D3DXCOLOR CParticle::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_life = 0;				// ����
	m_fRadius = 0;			// ���a
	m_fInertia = 1.00f;		// ����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ��������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// �|�C���^
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// ���I�m��
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init(pos, move, size, col, nLife, fInertia);

			pParticle->BindTexture(m_pTexture);
		}
	}

	return pParticle;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// �����o�ϐ��̏�����
	m_move = move;
	m_size = size;
	m_life = nLife;
	m_fInertia = fInertia; 
	m_col = col;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	// �ϐ��錾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// pos��move�̒l�𖈕b+����
	Pos += m_move;

	// ����
	m_move *= m_fInertia;

	// ���W
	SetPosition(Pos, m_size);

	// �G�t�F�N�g�����񂾂񓧖��ɂ���
	SetEffect(m_col, (float)m_life);

	// ���C�t����
	m_life--;


	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���C�t��0�ȉ��ɂȂ����Ƃ�
	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		Uninit();

		return;
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - -
	// �p�[�e�B�N������ʊO�ɍs�������������
	//- - - - - - - - - - - - - - - - - - - - - - - - - -
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		Uninit();

		return;
	}
}

//=============================================================================
// �`��
//=============================================================================
void CParticle::Draw(void)
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

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CParticle::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CParticle::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}