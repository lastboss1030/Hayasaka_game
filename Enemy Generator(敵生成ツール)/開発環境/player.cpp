//=============================================================================
//
// �v���C���[����[player.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "explosion.h"
#include "scene2D.h"		
#include "Input_Keyboard.h"	
#include "game.h"			
#include "fade.h"			
#include "effect.h"
#include "enemy.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
CPlayer::PLAYERSTATE CPlayer::m_state = CPlayer::PLAYERSTATE_NORMAL;

D3DXVECTOR3 g_pos;	// �擾�p

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_RIGHT	(5.0f)			// �v���C���[�̈ړ���(�E)
#define PLAYER_LEFT		(-5.0f)			// �v���C���[�̈ړ���(��)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
bool g_bBumpedPlayer = false;		//���G���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;

	m_shootspeed = 0;
	g_bBumpedPlayer = false;
	m_state = PLAYERSTATE_NORMAL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		 // ���I�m��
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			 // ����������
			pPlayer->Init(pos, size);

			// �e�N�X�`���̊��蓖��
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �T�C�Y
	m_size = size;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	// �v���C���[�̏��
	m_state = PLAYERSTATE_NORMAL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// Scene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// CInputKeyboard�C���X�^���X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();


	//- - - - - - - - - - - - - - - - - 
	// �v���C���[�̏��
	//- - - - - - - - - - - - - - - - -
	switch (m_state)
	{
	//�����Ȃ�
	case PLAYERSTATE_NORMAL:

		// �����F
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ��ԃJ�E���g��������
		m_nStateCnt = 0;

		// �J�E���g������
		m_nDispCnt = 0;

		// �\������
		m_bDisp = true;

		break;

	// �o��
	case PLAYERSTATE_APPEAR:

		// �\���J�E���g���Z
		m_nDispCnt++;

		// ��ԃJ�E���g���Z
		m_nStateCnt++;

		// 10�̔{���ɂȂ�����
		if ((m_nDispCnt % 3) == 0)
		{
			// �_�� true false�؂�ւ�
			m_bDisp = m_bDisp ? false : true;
		}

		// 60�̔{���ɂȂ�����
		if (m_nStateCnt >= 120)
		{
			// ���ʏ�Ԃ�
			m_state = PLAYERSTATE_NORMAL;
		}

		break;

	// �ҋ@
	case PLAYERSTATE_WAIT:

		// �o���ʒu
		pos = D3DXVECTOR3(D3DXVECTOR3(SCREEN_CENTER_X, 700, 0.0f));

		// ��\���ɂ���
		m_bDisp = false;

		// ��ԃJ�E���g���Z
		m_nStateCnt++;

		// �o���܂ł̎���
		if (m_nStateCnt >= 30)
		{
			// �o���ɂ���
			m_state = PLAYERSTATE_APPEAR;

			// ��ԃJ�E���g��������
			m_nStateCnt = 0;
		}

		break;
		
	// ���S
	case PLAYERSTATE_DEATH:

		// ��ԃJ�E���g���Z
		m_nStateCnt++;

		break;

	}
	

	//- - - - - - - - - - - - - - - - - - - -
	// �ړ� (�o�� & �ʏ펞)
	//- - - - - - - - - - - - - - - - - - - -
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_APPEAR)
	{
		// �ړ�����
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			m_move.y -= 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			m_move.y += 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			m_move.x -= 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			m_move.x += 2.0f;
		}

		// ��������
		m_move.x += (0.0f - m_move.x) * 0.2f;
		m_move.y += (0.0f - m_move.y) * 0.2f;

		// ���W�̐ݒ�
		pos += m_move;
	}


	// ��ʊO����
	if (pos.x - m_size.x / 2 <= 0.0f)
	{
		pos.x = m_size.x / 2;
	}
	if (pos.x + m_size.x / 2 >= SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}
	if (pos.y - m_size.y / 2 <= 0.0f)
	{
		pos.y = m_size.y / 2;
	}
	if (pos.y + m_size.y / 2 >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - m_size.y / 2;
	}


	// ��ʍ��E���΂���o�Ă���
	if (pos.x + m_size.x / 2 > SCREEN_WIDTH)
	{
		pos.x = m_size.x / 2;
	}
	else if (pos.x - m_size.x / 2 < 0)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}


	//- - - - - - - - - - - - - - - - - - - -
	// �v���C���[�ƓG�̓����蔻��
	//- - - - - - - - - - - - - - - - - - - -
	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	//	{
	//		// �V�[���֌W
	//		CScene *pScene;
	//		pScene = CScene::GetScene(nCntPriority, nCntScene);

	//		if (pScene != NULL)
	//		{
	//			CScene::OBJTYPE objType;
	//			objType = pScene->GetObjType();

	//			// �G��������
	//			if (objType == CScene::OBJTYPE_ENEMY && m_state == PLAYERSTATE_NORMAL)
	//			{
	//				// �ϐ��錾
	//				D3DXVECTOR3 PosEnemy;
	//				D3DXVECTOR3 EnemySize;

	//				// �G�̈ʒu�ƃT�C�Y���擾
	//				PosEnemy = pScene->GetPos();
	//				EnemySize = pScene->GetSize();

	//				// �G�̏��
	//				CEnemy *pEnemy;
	//				pEnemy = (CEnemy*)pScene;

	//				// �v���C���[�ƓG�̓����蔻��
	//				if (pos.x - (m_size.x / 2) < PosEnemy.x + (EnemySize.x / 2) &&
	//					pos.x + (m_size.x / 2) > PosEnemy.x - (EnemySize.x / 2) &&
	//					pos.y - (m_size.y / 2) < PosEnemy.y + (EnemySize.y / 2) &&
	//					pos.y + (m_size.y / 2) > PosEnemy.y - (EnemySize.y / 2))
	//				{
	//					// �G�����̐���
	//					CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

	//					// ���C�t����
	//					CEnemy *pEnemy = (CEnemy*)pScene;
	//					pEnemy->HitEnemy(1);

	//					// �_���[�W
	//					HitPlayer(1);

	//					return;
	//				}
	//			}
	//		}
	//	}
	//}


	//- - - - - - - - - - - - - - - - - - - - - - -
	// �v���C���[�ƃ{�X�p�[�c�Ƃ̓����蔻��
	//- - - - - - - - - - - - - - - - - - - - - - -
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// �V�[���֌W
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// �I�u�W�F�^�C�v
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// �{�X��������
				if (objType == CScene::OBJTYPE_BOSS && m_state == PLAYERSTATE_NORMAL)
				{
					// �ϐ��錾
					D3DXVECTOR3 PosParts;
					D3DXVECTOR3 PartsSize;

					// �G�̈ʒu�ƃT�C�Y���擾
					PosParts = pScene->GetPos();
					PartsSize = pScene->GetSize();

					// �G�̏��
					CParts *pParts;
					pParts = (CParts*)pScene;

					// �v���C���[�ƓG�̓����蔻��
					if (pos.x - (m_size.x / 4) < PosParts.x + (PartsSize.x / 4) &&
						pos.x + (m_size.x / 4) > PosParts.x - (PartsSize.x / 4) &&
						pos.y - (m_size.y / 4) < PosParts.y + (PartsSize.y / 4) &&
						pos.y + (m_size.y / 4) > PosParts.y - (PartsSize.y / 4))
					{
						// �_���[�W
						HitPlayer(1);

						return;
					}
				}
			}
		}
	}


	//- - - - - - - - - - - - - - - - -
	// ���ʏ�� & �o�����
	//- - - - - - - - - - - - - - - - -
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_APPEAR)
	{
		// �e����
		m_shootspeed++;

		// �e���ˊԊu
		if (m_shootspeed >= 15)
		{
			m_shootspeed = 0;
		}

		// �������u�Ԍ��Ă���
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
		{
			// �J�E���g���Z�b�g
			m_shootspeed = 0;
		}

		// �e�̔���
		if (pInputKeyboard->GetPress(DIK_SPACE) == true)
		{
			if (m_shootspeed == 0)
			{
				CBullet::Create(D3DXVECTOR3(pos.x + 0.0f, pos.y - 10.0f, 1.0f), D3DXVECTOR3(0.0f, -8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, CBullet::ATTACKTYPE_NORMAL, 1);
			}
		}
	}


	//- - - - - - - - - - - - -
	// ���S���
	//- - - - - - - - - - - - -
	else if (m_state == PLAYERSTATE_DEATH)
	{
		if (m_nPlayerCnt >= 0)
		{
			m_state = PLAYERSTATE_APPEAR;

			m_nPlayerInvincibleCnt = 120;

			pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 4, 0.0f);
		}
	}

	// ���
	g_pos = pos;

	// ���W�̐ݒ�
	CScene2D::SetPosition(pos,m_size);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{
		// Scene2D�̕`�揈��
		CScene2D::Draw();
	}
}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �v���C���[�̈ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �v���C���[�̈ʒu�擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetPlayerPos(void)
{
	return g_pos;
}

//=============================================================================
// �ʒu��ݒ�
//=============================================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2D�ɏ���n��
	SetPosition(pos, m_size);
}

//=============================================================================
// �v���C���[�̈ړ��ʂ�ݒ�
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// �v���C���[�̓����蔻��
//=============================================================================
bool CPlayer::HitPlayer(int nDamage)
{
	// ���݂̈ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// ��������
	CExplosion::Create(pos, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

	// ��Ԃ�ҋ@��Ԃ�
	m_state = PLAYERSTATE_WAIT;

	return false;
}

//=============================================================================
// �v���C���[�̏��
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetPlayerState(void)
{
	return m_state;
}