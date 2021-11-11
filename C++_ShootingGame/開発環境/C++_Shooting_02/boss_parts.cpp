//=============================================================================
//
// �{�X�̏��� [boss_parts.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "boss_parts.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "enemy.h"
#include "explosion.h"
#include "fade.h"
#include "sound.h"
#include "score.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CParts::m_apTexture[] = {};
CEnemy *CParts::m_pEnemy = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParts::CParts(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_Partstype = PARTSTYPE_NONE;
	m_nHitCnt = 0;
	m_bShoot = false;
	m_nCntShoot = 0;
	m_nBreakCnt = 0;
	m_nExplosionCnt = 0;
	m_nSwingWidth = 40.0f;

	// �ړ��J�E���g
	m_nMoveCnt = 0;
	m_nMoveCnt2 = MOVECNT2 / 2;
	m_nMoveCnt3 = 0;

	// ���[�U�[�̏�����
	m_nLaserCnt = 0;
	m_nLaserCnt2 = 0;
	m_nLaserCnt3 = 0;

	// �z�[�~���O�e�̏�����
	m_nCntShotHoming = 0;
	m_nCntShotHoming2 = 0;

	// �H����ꂽ�Ƃ��p�J�E���g
	m_nWingBreakCnt = 2;

	// �T�E���h�J�E���g�̏�����
	m_nSoundCnt = 0;
	m_nSoundCnt2 = 0;
	m_nSoundCnt3 = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParts::~CParts()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParts::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nLife)
{
	// ������
	m_size = size;
	m_move = move;
	m_Partstype = type;
	m_nLife = nLife;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_BOSS);

	// �{�X�̏�Ԃ�ʏ�ɂ���
	SetState(PARTSSTATE::PARTS_NORMAL);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParts::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParts::Update(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// �t�F�[�h�֌W
	CFade *pFade;
	pFade = CManager::GetFade();

	// �Q�[��
	CGame *pGame;
	pGame = CManager::GetGame();

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// �ϐ��錾
	int EnemyCnt = m_pEnemy->GetEnemyCnt();		// �G�J�E���g


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �{�X�p�[�c�̏��
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	switch (m_state)
	{
	// �m�[�}�����
	case PARTS_NORMAL:
		// �ʏ�F
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���Ă���
		m_bShoot = true;

		break;

	// �_���[�W���
	case PARTS_DAMAGE:
		// �Ԃ�����
		m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �������Ă���̃J�E���g���₷
		m_nHitCnt++;
		if (m_nHitCnt >= 10)
		{
			// �m�[�}���ɖ߂�
			m_nHitCnt = 0;
			m_state = PARTS_NORMAL;
		}

		break;
	
	// �j�����
	case PARTS_BREAK:

		// �����J�E���g
		m_nExplosionCnt++;
		if (m_nExplosionCnt >= 8)
		{
			// �����J�E���g�߂�
			m_nExplosionCnt = 0;

			// ��������
			CExplosion::Create(D3DXVECTOR3(pos.x, pos.y - 5.0f, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 5, 0));
		}
		else if ((rand() % 8) == 0)
		{
			// ��������
			CExplosion::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(30, 30, 0), D3DXVECTOR3(0, 6, 0));
		}

		// �I�u�W�F�N�g�̎�ސݒ�
		SetObjType(CScene::OBJTYPE_BOSSBREAK);

		// �Â�����
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// ���ĂȂ���
		m_bShoot = false;

		break;

	// ���ė�����
	case PARTS_BREAKFALL:

		// ��[���̂�
		if (m_Partstype == PARTSTYPE_FRONT)
		{
			// �����J�E���g
			m_nExplosionCnt++;
			if (m_nExplosionCnt >= 8)
			{
				// �����J�E���g�߂�
				m_nExplosionCnt = 0;

				// ��������
				CExplosion::Create(D3DXVECTOR3(pos.x, pos.y - 8.0f, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 5, 0));
			}
			else if ((rand() % 8) == 0)
			{
				// ��������
				CExplosion::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(30, 30, 0), D3DXVECTOR3(0, 6, 0));
			}

			// �Â�����
			m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);

			// �I�u�W�F�N�g�̎�ސݒ�
			SetObjType(CScene::OBJTYPE_BOSSBREAK);

			// ������\��
			pos.y += 4.0f;
			m_move.x *= 0;

			// ���ĂȂ���
			m_bShoot = false;

		}
		break;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �G���G���S���������瓮���n�߂�
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (EnemyCnt <= 0)
	{
		// �ړ�
		pos.y += 2.0;
		m_nMoveCnt++;

		// �x����
		m_nSoundCnt++;
		if (m_nSoundCnt == 1)
		{
			pSound->Play(CSound::SOUND_LABEL_SE_ALERT);
		}

		// ��莞�ԂŒ�~
		if (m_nMoveCnt >= 250)
		{
			// �ړ��ʌ��炷
			pos.y -= 2.0;

			// �ړ��ʉ��Z
			pos += m_move;
			m_nMoveCnt2++;
			m_nMoveCnt3++;

			// ���˃J�E���g�i�߂�
			m_nCntShoot++;			// �ʏ�e
			m_nCntShotHoming++;		// �z�[�~���O
			m_nLaserCnt++;			// ���[�U�[

			// ��~������{�X�̒e���o��
			if (m_bShoot == true)
			{
				// 4�̏e���̒e
				// ���˃J�E���g
				if (m_nCntShoot >= 60)
				{
					// �J�E���g���Z�b�g
					m_nCntShoot = 0;

					// �p�[�c��4�̏e����������
					if (m_Partstype == PARTSTYPE_SHOOT1 || m_Partstype == PARTSTYPE_SHOOT2 || m_Partstype == PARTSTYPE_SHOOT3 || m_Partstype == PARTSTYPE_SHOOT4)
					{
						// ���ˉ�
						pSound->Play(CSound::SOUND_LABEL_SE_SHOTBOSS);

						// �e�̐���
						CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
					}
				}
				else if (m_nCntShotHoming >= 120)
				{
					// �v���C���[�̏��擾
					int nPlayerState = CPlayer::GetPlayerState();
					CPlayer *pPlayer;
					pPlayer = CGame::GetPlayer();

					if (nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
					{

						// �J�E���g���Z�b�g
						m_nCntShotHoming = 0;

						if (m_Partstype == PARTSTYPE_LEFT)
						{
							// �z�[�~���O�e�̐���
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(-2.0f, -5.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMYHOMING, CBullet::ATTACKTYPE_HOMING, 1);

							m_nCntShotHoming2++;
						}
						else if (m_Partstype == PARTSTYPE_RIGHT)
						{
							// �z�[�~���O�e�̐���
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(2.0f, -5.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMYHOMING, CBullet::ATTACKTYPE_HOMING, 1);
						}

						if (m_nCntShotHoming2 >= 20)
						{
							// �ǔ��J�n


							// �J�E���g���Z�b�g
							m_nCntShotHoming2 = 0;
						}
					}
				}


				// ���[�U�[����
				// ���˃J�E���g
				if (m_nLaserCnt >= 400)
				{
					// �J�E���g�𑝂₷
					m_nLaserCnt3++;

					// ���[�U�[��
					m_nSoundCnt3++;
					if (m_nSoundCnt3 == 1 && m_Partstype == PARTSTYPE_FRONT)
					{
						pSound->Play(CSound::SOUND_LABEL_SE_LASER);
					}

					// ���[�U�[�̎���
					if (m_nLaserCnt3 >= 400)
					{
						// �J�E���g���Z�b�g
						m_nLaserCnt = 0;
						m_nLaserCnt3 = 0;
						m_nSoundCnt3 = 0;
					}

					// �p�[�c���擪��������
					if (m_Partstype == PARTSTYPE_FRONT)
					{
						m_nLaserCnt2++;

						// �e�̊Ԋu
						if (m_nLaserCnt2 >= 2)
						{
							// �Ǐ]�܂ł̎���
							m_nCntShotHoming2++;

							// �J�E���g���Z�b�g
							m_nLaserCnt2 = 0;

							// �e�̐���
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(40.0f, 40.0f, 0.0f), CBullet::BULLETTYPE_BOSSPARTS, CBullet::ATTACKTYPE_NORMAL, 1);
						}
					}
				}
			}

			//- - - - - - - - - - - - - - - - - - - - - - - -
			// ��莞�Ԃňړ� (���ړ�)
			//- - - - - - - - - - - - - - - - - - - - - - - -
			
			// �j���������ȊO
			if (m_state != PARTS_BREAKFALL)
			{
				if (m_nMoveCnt2 >= MOVECNT2)
				{
					m_nMoveCnt2 = 0;
					m_move *= -1.0;
				}

				// �㉺�ړ�
				if (m_nMoveCnt3 >= m_nSwingWidth)
				{
					m_nMoveCnt3 = 0;
					m_move.y *= -1.0f;
				}
			}
		}
	}

	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(pos, m_size);

	// �F�`�F���W
	CScene2D::SetCol(m_Color);


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//���C�t��0�̏ꍇ�j����Ԃ�
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_nLife <= 0)
	{
		// �p�[�c�j����Ԃ�
		m_state = PARTS_BREAK;

		// �j���T�E���h�J�E���g
		m_nSoundCnt2++;
		if (m_nSoundCnt2 == 1)
		{
			// �j��
			pSound->Play(CSound::SOUND_LABEL_SE_PARTSBREAK);

			// �X�R�A�̉��Z
			CScore::AddScore(500);
		}

		// �p�[�c�̃^�C�v��������������
		if (m_Partstype == PARTSTYPE_MIDLE)
		{
			// �j���J�E���g
			m_nBreakCnt++;

			// ���U���g�̃��[�h���N���A��
			pGame->SetResult(CGame::RESULTMODE_GAMECLEAR);

			// ���U���g�Ɉړ�����
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);

			// �j���J�E���g�����ɂȂ�����
			if (m_nBreakCnt >= 40)
			{
				// �I��
				Uninit();

				//�T�E���h�X�g�b�v
				pSound->Stop(CSound::SOUND_LABEL_SE_LASER);

				// �ʏ�BGM�I�t
				pSound->Stop(CSound::SOUND_LABEL_BGM_NORMAL);
			}
		}

		if (m_Partstype == PARTSTYPE_FRONT)
		{
			// �p�[�c�j����Ԃ�
			m_state = PARTS_BREAKFALL;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParts::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CParts::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts1.png", &m_apTexture[0]);	// ��[
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts2.png", &m_apTexture[1]);	// �e���P
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts3.png", &m_apTexture[2]);	// �e���Q
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts4.png", &m_apTexture[3]);	// �e���R
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts5.png", &m_apTexture[4]);	// �e���S
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts6.png", &m_apTexture[5]);	// ����
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts7.png", &m_apTexture[6]);	// �E��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts8.png", &m_apTexture[7]);	// ����

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CParts::Unload(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CParts *CParts::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nTexNum, int nLife)
{
	// �ϐ��錾
	CParts *pParts = NULL;

	if (pParts == NULL)
	{
		// ���I�m��
		pParts = new CParts;

		if (pParts != NULL)
		{
			// �������������Ăяo��
			pParts->Init(pos, size, move, type, nLife);

			// �G�̃e�N�X�`�������蓖�Ă�
			pParts->BindTexture(m_apTexture[nTexNum]);
		}
	}

	// �l��Ԃ�
	return pParts;
}

//=============================================================================
// �Փ˔��菈��
//=============================================================================
void CParts::Collision(void)
{

}

//=============================================================================
// �����蔻��
//=============================================================================
bool CParts::HitBossParts(int nDamage)
{
	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���C�t����
	m_nLife -= nDamage;

	// �_���[�W����
	m_state = PARTS_DAMAGE;

	// �q�b�g��
	pSound->Play(CSound::SOUND_LABEL_SE_HIT);

	return false;
}

//=============================================================================
// �{�X�p�[�c�̏�Ԃ�ݒ�
//=============================================================================
void CParts::SetPartsState(PARTSSTATE PartsState)
{
	// �p�[�c�̏��
	m_state = PartsState;
}