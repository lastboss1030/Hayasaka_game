//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "sound.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[] = {};
int CEnemy::m_enmeyCnt = MAX_ENEMY;

// �G�\����
CEnemy::ENEMY CEnemy::m_aEnemy[MAX_ENEMY] =
{
	40,
	D3DXVECTOR3(320.0f, 100.0f ,0.0f),	// ���W
	D3DXVECTOR3(60.0f, 60.0f ,0.0f),	// �T�C�Y
	D3DXVECTOR3(0.0f, 2.0f ,0.0f),		// �ړ���
	2,									// ���C�t
	0,									// �^�C�v
};

int CEnemy::m_nIndexEnemy = 0;			// �ԍ�
int CEnemy::m_nCounterGame = 0;			// �Q�[���J�E���^�[

//=============================================================================
// CEnemy�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nType = 0;

	// �G�\���� ���l������
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		m_aEnemy[nCnt].pos = D3DXVECTOR3(320.0f, 100.0f, 0.0f);		// ���W
		m_aEnemy[nCnt].size = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// �T�C�Y
		m_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);		// �ړ���
		m_aEnemy[nCnt].nLife = 2;									// ���C�t
		m_aEnemy[nCnt].nType = 0;									// �^�C�v
	}

	// �^�C�v������
	m_Enemytype = ENEMYTYPE_NONE;

	// �O���t�@�C���ǂݍ���
	LoadData();
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, int nType)
{
	// ������
	m_size = size;
	m_move = move;
	m_nLife = nLife;
	m_nType = nType;

	// �\���̏�����
	m_aEnemy[m_nIndexEnemy].pos = D3DXVECTOR3(320.0f, 100.0f, 0.0f);	// ���W
	m_aEnemy[m_nIndexEnemy].size = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// �T�C�Y
	m_aEnemy[m_nIndexEnemy].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);		// �ړ���
	m_aEnemy[m_nIndexEnemy].nLife = 2;									// ���C�t
	m_aEnemy[m_nIndexEnemy].nType = 0;									// �^�C�v

	// �J�E���g������
	m_nHomingCnt = 0;
	m_nMoveCnt = 0;
	m_nShotCnt = 0;
	m_enmeyCnt = MAX_ENEMY;

	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	// �G�̏�Ԃ�ʏ�ɂ���
	SetState(ENEMYSTATE::ENEMY_NORMAL);

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

	// �J�E���g++
	m_nShotCnt++;
	m_nMoveCnt++;

	// ���̈ʒu�܂ŗ�����~�߂� (��~�^)
	if (pos.y >= 300 && m_nType == 2 && m_nMoveCnt < 300)
	{
		m_move.y = 0;

		// �e����
		if (m_nShotCnt >= 70 && m_nType == 2)
		{
			// �J�E���g�߂�
			m_nShotCnt = 0;

			// �e�̐���
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f),
				D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f),
				CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
		}

	}

	// ��莞�Ԃŉ�����
	else if (m_nMoveCnt == 900 && m_nType == 2)
	{
		m_move.y += 3;
	}

	// �G�̈ړ�
	MoveEnemy();

	// �ǔ��^
	if (m_nType == 1)
	{
		m_nHomingCnt++;

		if (m_nHomingCnt <= 300)
		{
			HomigEnemy();
		}
	}

	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(pos, m_size);

	//- - - - - - - - - - - - - - - - - - - - -
	// �G�̒e���o��
	//- - - - - - - - - - - - - - - - - - - - -
	if (m_nShotCnt >= 70 && m_nType == 2)
	{
		// �J�E���g�߂�
		m_nShotCnt = 0;

		// �e�̐���
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f),
			D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f),
			CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);

		// ���ˉ�
		pSound->Play(CSound::SOUND_LABEL_SE_SHOTENEMY);
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// �G�̏��
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	switch (m_State)
	{
	// �m�[�}�����
	case ENEMY_NORMAL:
		// �ʏ�F
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	// �_���[�W���
	case ENEMY_DAMAGE:
		// �Ԃ�����
		m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �������Ă���̃J�E���g���₷
		m_nHitCnt++;
		if (m_nHitCnt >= 10)
		{
			// �m�[�}���ɖ߂�
			m_nHitCnt = 0;
			m_State = ENEMY_NORMAL;
		}

		break;
	}

	// �F�`�F���W
	CScene2D::SetCol(m_Color);

	//- - - - - - - - - - - - - - - - - - - - - -
	// ��ʊO�ɍs������
	//- - - - - - - - - - - - - - - - - - - - - -
	if (pos.x + m_size.x / 2.0f < 0.0f ||
		pos.x - m_size.x / 2.0f > SCREEN_WIDTH ||
		pos.y - m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		Uninit();

		return;
	}

	//- - - - - - - - - - - - - - - - - - - - - -
	// �G�̃��C�t��0�ɂȂ��������
	//- - - - - - - - - - - - - - - - - - - - - -
	if (m_nLife <= 0)
	{
		Uninit();

		return;
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CEnemy::Unload(void)
{
	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TEX_ENEMY; nCnt++)
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
CEnemy *CEnemy::Create(int nIndex)
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
			pEnemy->Init(m_aEnemy[nIndex].pos, m_aEnemy[nIndex].size,
							m_aEnemy[nIndex].move,m_aEnemy[nIndex].nLife, m_aEnemy[nIndex].nType);

			// �G�̃e�N�X�`�������蓖�Ă�
			pEnemy->BindTexture(m_apTexture[m_aEnemy[nIndex].nType]);
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

//=============================================================================
// �����蔻��
//=============================================================================
bool CEnemy::HitEnemy(int nDamage)
{
	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// ���C�t����
	m_nLife -= nDamage;

	// �_���[�W����
	m_State = ENEMY_DAMAGE;

	// �q�b�g��
	pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

	return false;
}

//=============================================================================
// �ǔ��^�̓G
//=============================================================================
void CEnemy::HomigEnemy(void)
{
	// ���W
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// �ϐ��錾
	float fAngle = 0.0f;

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

					fAngle = atan2f(VecX, VecY);

					// �ǔ��v�Z
					m_move.x = sinf(fAngle) * 3.5f;
					m_move.y = cosf(fAngle) * 3.5f;
				}
			}
		}
	}
}

//=============================================================================
// �J�E���^�[�擾
//=============================================================================
int CEnemy::GetCounterGame(void)
{
	return m_nCounterGame;
}

//=============================================================================
// �O���t�@�C���ǂݍ���
//=============================================================================
void CEnemy::LoadData(void)
{
	FILE *pFile;

	// �t�@�C���J��
	pFile = fopen(ENEMY_TEXT_NAME, "r");

	if (pFile != NULL)
	{
		// �t�@�C�����J������
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nTime);
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].pos.x,  &m_aEnemy[nCnt].pos.y,  &m_aEnemy[nCnt].pos.z);		// ������
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].size.x, &m_aEnemy[nCnt].size.y, &m_aEnemy[nCnt].size.z);	// ���x
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].move.x, &m_aEnemy[nCnt].move.y, &m_aEnemy[nCnt].move.z);	// ���a
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nLife);															// ���C�t
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nType);															// �^�C�v
		}
	}

	// �t�@�C�������
	fclose(pFile);
}

//=============================================================================
// �G����
//=============================================================================
void CEnemy::EnemyGenerator(void)
{
	// �Q�[���J�E���g
	m_nCounterGame++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �Q�[���J�E���g��.nTime�Ɠ����ɂȂ�����
		if (m_nCounterGame == m_aEnemy[nCntEnemy].nTime)
		{
			// ����
			Create(nCntEnemy);
		}
	}
}

//=============================================================================
// �G�������Z�b�g�p (game.cpp�ŌĂԂ���)
//=============================================================================
void CEnemy::EnemyGeneratorReset(void)
{
	m_nCounterGame = 0;
}