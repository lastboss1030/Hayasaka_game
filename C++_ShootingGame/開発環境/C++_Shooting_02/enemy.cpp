//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "sound.h"

#include <stdlib.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_COUNTER (50)		// �J�E���^�[

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture = {};
CEnemy::ENEMY CEnemy::m_State = ENEMY_NONE;
int CEnemy::m_enmeyCnt = 0;

//=============================================================================
// CEnemy�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColor = 0;

	m_Enemytype = ENEMYTYPE_NONE;
	m_enmeyCnt++;
}

//=============================================================================
// CEnemy�̃f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type)
{
	//������
	m_size = size;
	m_move = move;
	m_Enemytype = type;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	// �G�̏�Ԃ�ʏ�ɂ���
	SetState(ENEMY::ENEMY_NORMAL);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
	m_enmeyCnt--;
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// �v���C���[�̈ړ��ʎ擾
	D3DXVECTOR3 PlayerMove;
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	PlayerMove = pPlayer->GetMove();

	// �ړ��ʉ��Z
	pos += m_move;

	// ���̈ʒu�܂ŗ�����~�߂�
	if (pos.y >= 250 && m_Enemytype == ENEMYTYPE_STOP)
	{
		pos.y -= 2;
	//	m_State == ENEMY_MOVING;
	}

	// �G�̈ړ�
	MoveEnemy();

	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(pos, m_size);

	// ��ʊO�ɍs������
	if (pos.x - m_size.x / 2.0f < 0.0f || pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||  pos.y - m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// �G������
		Uninit();
	}

	// �G�̒e���o��
	m_nShotCnt++;

	if (m_nShotCnt == 60)
	{
		// �J�E���g�߂�
		m_nShotCnt = 0;

		// ���ˉ�
		pSound->Play(CSound::SOUND_LABEL_SE_SHOTENEMY);

		CBullet::Create(pos, D3DXVECTOR3(-1.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f,0.0f), CBullet::BULLETTYPE_ENEMY, 1);
		CBullet::Create(pos, D3DXVECTOR3(1.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f,0.0f), CBullet::BULLETTYPE_ENEMY, 1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &m_apTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CEnemy::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
// ��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type)
{
	// �ϐ��錾
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// ���I�m��
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			// �������������Ăяo��
			pEnemy->Init(pos, size, move, speed, type);

			// �G�̃e�N�X�`�������蓖�Ă�
			pEnemy->BindTexture(m_apTexture);
		}
	}
	// �l��Ԃ�
	return pEnemy;
}

//=============================================================================
// �G�̈ړ��Ǘ�
//=============================================================================
void CEnemy::MoveEnemy(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = GetPosition();


	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(pos, m_size);
}