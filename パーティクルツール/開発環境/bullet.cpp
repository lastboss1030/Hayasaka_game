//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "scene.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "particle.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
float g_fSpeed = 0.0f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_life = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ��������
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	// �|�C���^
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// ���I�m��
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos, move, size);

			pBullet->BindTexture(m_pTexture);
		}
	}

	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	// �����o�ϐ��̏�����
	m_move = move;
	m_size = size;

	// �e�̃��C�t
	m_life = 50;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_BULET);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// �ϐ��錾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// pos��move�̒l�𖈕b+����
	Pos += m_move;
	SetPosition(Pos, m_size);

	// ���C�t����
	m_life--;

	// ��ʂƒe�̓����蔻��
	// �e����ʊO�ɍs�������������
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// �e������
		Uninit();

		return;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �e�̃��C�t����������
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		// �e������
		Uninit();

		return;
	}
}

//=============================================================================
// �`��
//=============================================================================
void CBullet::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CBullet::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}