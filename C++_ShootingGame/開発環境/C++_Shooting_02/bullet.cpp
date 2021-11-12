//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "score.h"
#include "life.h"
#include "Input_Keyboard.h"
#include "enemy.h"
#include "scene.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "boss_parts.h"

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
	m_bulletType = BULLETTYPE_NONE;
	m_life = 0;
	m_nCntHoming = 0;
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
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, ATTACKTYPE AttackType, int nDamage)
{
	// �|�C���^
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// ���I�m��
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos, move, size, type, nDamage);

			pBullet->BindTexture(m_pTexture);
		}
	}

	// �U����ޏ��̌Ăяo��
	pBullet->SetAttackType(AttackType);

	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, int nDamage)
{
	// �����o�ϐ��̏�����
	m_move = move;
	m_size = size;
	m_bulletType = type;
	m_nDamage = nDamage;

	// �e�̃��C�t�ݒ�
	if (m_bulletType != BULLETTYPE_ENEMYHOMING)
	{
		// �ǔ��ȊO�̒e
		m_life = 40;
	}
	else
	{
		// �ǔ��̒e
		m_life = 140;
	}


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

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();


	// �ǔ��J�E���g
	m_nCntHoming++;

	// �U���^�C�v���ǔ���������
	if (m_AttackType == ATTACKTYPE_HOMING)
	{
		// �ǔ�����
		HomingBullet();
	}

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
		// �����̐���
		CExplosion::Create(D3DXVECTOR3(Pos.x, Pos.y, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

		// �e������
		Uninit();

		return;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �G�t�F�N�g�𐶐�
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		// �v���C���[�̒e
		CEffect::Create(Pos, 50, m_size*1.8f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		CEffect::Create(Pos, 50, m_size*1.8f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_ENEMY)
	{
		// �G�̒e
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_BOSSPARTS)
	{
		// �{�X�̒e
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_ENEMYHOMING)
	{
		// �{�X�̒e(�ǔ�)
		CEffect::Create(Pos, 40, m_size * 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CEffect::Create(Pos, 40, m_size * 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �����蔻��
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// �V�[��
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// �I�u�W�F�^�C�v
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// �e���
				CBullet::BULLETTYPE bulletType = m_bulletType;

				// �v���C���[
				int nPlayerState = CPlayer::GetPlayerState();
				CPlayer *pPlayer;
				pPlayer = CGame::GetPlayer();

				// �G��������
				if (objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER)
				{
					// �ϐ��錾
					D3DXVECTOR3 PosEnemy;
					D3DXVECTOR3 EnemySize;

					// �G�̈ʒu�ƃT�C�Y���擾
					PosEnemy = pScene->GetPos();
					EnemySize = pScene->GetSize();

					// �e�ƓG�̓����蔻��
					if (Pos.x - (m_size.x / 2) < PosEnemy.x + (EnemySize.x / 2) &&
						Pos.x + (m_size.x / 2) > PosEnemy.x - (EnemySize.x / 2) &&
						Pos.y - (m_size.y / 2) < PosEnemy.y + (EnemySize.y / 2) &&
						Pos.y + (m_size.y / 2) > PosEnemy.y - (EnemySize.y / 2))
					{
						// �����̐���
						CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// ���C�t����
						CEnemy *pEnemy = (CEnemy*)pScene;
						pEnemy->HitEnemy(1);

						// �X�R�A�̉��Z
						CScore::AddScore(200);

						// �e�̔j��
						Uninit();

						return;
					}
				}

				// �{�X��������
  				if (objType == CScene::OBJTYPE_BOSS && m_bulletType == BULLETTYPE_PLAYER)
				{
					// �ϐ��錾
					D3DXVECTOR3 PosParts;
					D3DXVECTOR3 PartsSize;

					// �G�̈ʒu�ƃT�C�Y���擾
					PosParts = pScene->GetPos();
					PartsSize = pScene->GetSize();

					// �e�ƓG�̓����蔻��
					if (Pos.x - (m_size.x / 2) < PosParts.x + (PartsSize.x / 2) &&
						Pos.x + (m_size.x / 2) > PosParts.x - (PartsSize.x / 2) &&
						Pos.y - (m_size.y / 2) < PosParts.y + (PartsSize.y / 2) &&
						Pos.y + (m_size.y / 2) > PosParts.y - (PartsSize.y / 2))
					{
						// �����̐���
						CExplosion::Create(D3DXVECTOR3(Pos.x, Pos.y, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// ���C�t����
						CParts *pParts = (CParts*)pScene;
						pParts->HitBossParts(1);

						// �X�R�A�̉��Z
						CScore::AddScore(100);

						// �e�̔j��
						Uninit();

						return;
					}
				}

				// �v���C���[��������
				if (objType == CScene::OBJTYPE_PLAYER && ( m_bulletType == BULLETTYPE_ENEMY || m_bulletType== BULLETTYPE_BOSSPARTS || m_bulletType == BULLETTYPE_ENEMYHOMING) && nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
				{
					// �v���C���[�̏��
					D3DXVECTOR3 posPlayer = pScene->GetPos();
					D3DXVECTOR3 sizePlayer = pScene->GetSize();
					CLife *pLife = CGame::GetLife();

					// �v���C���[�̏��
					int nPlayerState = CPlayer::GetPlayerState();
					CPlayer *pPlayer;
					pPlayer = CGame::GetPlayer();

					if (Pos.y - m_size.y / 2.0f < posPlayer.y + sizePlayer.y / 2 &&
						Pos.y + m_size.y / 2.0f > posPlayer.y - sizePlayer.y / 2 &&
						Pos.x - m_size.x / 2.0f < posPlayer.x + sizePlayer.x / 2 &&
						Pos.x + m_size.x / 2.0f > posPlayer.x - sizePlayer.x / 2)
					{
						// �����̐���
						CExplosion::Create(posPlayer, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// SE�̒ǉ�
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

						// �v���C���[�̎c�@����
						pPlayer->HitPlayer(1);

						// �e������
						Uninit();

						return;
					}

				}
			}
		}
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

//=============================================================================
// �ǔ�����
//=============================================================================
void CBullet::HomingBullet(void)
{
	// ���W
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// �ϐ��錾
	float fAngle = 0.0f;
	float fAngleDest;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// �V�[��
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// �I�u�W�F�^�C�v�擾
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// �v���C���[�̏��擾
				int nPlayerState = CPlayer::GetPlayerState();
				CPlayer *pPlayer;
				pPlayer = CGame::GetPlayer();

				// �^�C�v���v���C���[�Ńm�[�}����Ԃ�������
				if (objType == CScene::OBJTYPE_PLAYER && nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
				{
					// �v���C���[�̏��
					D3DXVECTOR3 posPlayer = pScene->GetPos();

					// �p�x�v�Z
					float VecX = (posPlayer.x - pos.x);
					float VecY = (posPlayer.y - pos.y);

					//fAngleDest = atan2f(VecX, VecY);
					//fAngle = (fAngleDest - fAngle) * 0.6f;

					fAngle = atan2f(VecX, VecY);

				//  fAngleDest = (�ړI�̈ʒu - ���݂̊p�x) * �ړI�n�ւ̑��x

					// �ǔ��v�Z
					m_move.x = sinf(fAngle) * 6.0f;
					m_move.y = cosf(fAngle) * 6.0f;
				}
			}
		}
	}
}