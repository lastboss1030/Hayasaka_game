//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : taiki hayasaka 
//
//=============================================================================
#include "renderer.h"		
#include "manager.h"		
#include "scene2D.h"		
#include "scene.h"			
#include "input.h"			
#include "player.h"			
#include "bullet.h"			
#include "explosion.h"		
#include "sound.h"			
#include "bg.h"				
#include "enemy.h"			
#include "number.h"			
#include "score.h"			
#include "life.h"			
#include "game.h"			
#include "Input_Keyboard.h"	
#include "fade.h"
#include "effect.h"
#include "boss_parts.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CNumber *CGame::m_pNumber = NULL;
CScore *CGame::m_pScore = NULL;
CPolygon *CGame::m_pPolygon = NULL;
CLife *CGame::m_pLife = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CParts *CGame::m_pParts = NULL;

CGame::RESULTMODE CGame::m_resultmode = RESULTMODE_NONE;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	// ������
	nBossDeathCnt = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ��������
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = new CGame;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;

	// ����������
	pGame->Init(pos, size);
	
	return pGame;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �S�Ẵe�N�X�`���ǂݍ���
	CGame::LoadAll();

	// �w�i�̐���
	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_CENTER_X, 700, 0.0f), D3DXVECTOR3(MAX_PLAYER_X, MAX_PLAYER_Y, 0));

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(1000.0f, 50.0f, 0), D3DXVECTOR3(20.0f, 30.0f, 0.0f));

	// ���C�t�̐���
	m_pLife = CLife::Create(D3DXVECTOR3(130.0f, 40.0f, 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));

	// �G����
	CGame::EnemyAll();

	// �{�X����
	CGame::BossAll();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �S�Ẵe�N�X�`����j������
	UnloadAll();

	// �S�ẴI�u�W�F�N�g������
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	// �L�[�{�[�h�֌W
	CInputKeyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// ���[�h�֌W
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// �t�F�[�h�֌W
	CFade *pFade;
	pFade = CManager::GetFade();

	// �G & �{�X�̃J�E���g
	int EnemyCnt = m_pEnemy->GetEnemyCnt();

	// ���C�t��0�ɂȂ�����
	if (m_pLife->GetLife() == 0 && pFade->GetFade() == CFade::FADE_NONE)
	{
		// �Q�[���I�[�o�[
		m_resultmode = RESULTMODE_GAMEOVER;

		// ���U���g�Ɉړ�����
		pFade->SetFade(CManager::MODE_RESULT);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// �Z�b�g
//=============================================================================
void CGame::SetResult(RESULTMODE resultmode)
{
	m_resultmode = resultmode;
}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
void CGame::LoadAll(void)
{
	// �e�N�X�`���̓ǂݍ���
	CBullet::Load();
	CExplosion::Load();
	CPlayer::Load();
	CBg::Load();
	CEnemy::Load();
	CNumber::Load();
	CLife::Load();
	CEffect::Load();
	CParts::Load();
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CGame::UnloadAll(void)
{
	// �e�N�X�`����j��
	CBullet::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CNumber::Unload();
	CLife::Unload();
	CEffect::Unload();
	CParts::Unload();
}

//=============================================================================
// �G�Ǘ�
//=============================================================================
void CGame::EnemyAll(void)
{
	// �G�̐���

	// �ŏ���2��
	CEnemy::Create(D3DXVECTOR3(320.0f, 100.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_STOP);
//	CEnemy::Create(D3DXVECTOR3(960.0f, 100.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_STOP);

	// ���ړ�
//	CEnemy::Create(D3DXVECTOR3(320.0f, -400.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_MOVE);
//	CEnemy::Create(D3DXVECTOR3(960.0f, -400.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(-1.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_MOVE);

	// �ړ��Q
//	CEnemy::Create(D3DXVECTOR3(120.0f, -800.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_MOVE2);
//	CEnemy::Create(D3DXVECTOR3(1160.0f, -800.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::ENEMYTYPE_MOVE2);
}

//=============================================================================
// �{�X�Ǘ�
//=============================================================================
void CGame::BossAll(void)
{
	// �{�X�p�[�c���Ƃɐ���
	CParts::Create(D3DXVECTOR3(640.0f, 400.0f - WAIT_Y, 0.0f), D3DXVECTOR3(120.0f, 240.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_FRONT, 0, 10);	// ��O
	CParts::Create(D3DXVECTOR3(425.0f, 295.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_SHOOT1, 1, 5);		// �e��1
	CParts::Create(D3DXVECTOR3(530.0f, 298.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_SHOOT2, 2, 5);		// �e��2
	CParts::Create(D3DXVECTOR3(750.0f, 298.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_SHOOT3, 3, 5);		// �e��3
	CParts::Create(D3DXVECTOR3(845.0f, 295.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_SHOOT4, 4, 5);		// �e��4
	CParts::Create(D3DXVECTOR3(440.0f, 215.0f - WAIT_Y, 0.0f), D3DXVECTOR3(280.0f, 160.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_LEFT, 5, 4);		// ����
	CParts::Create(D3DXVECTOR3(840.0f, 220.0f - WAIT_Y, 0.0f), D3DXVECTOR3(280.0f, 160.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_RIGHT, 6, 4);	// �E��
	CParts::Create(D3DXVECTOR3(640.0f, 120.0f - WAIT_Y, 0.0f), D3DXVECTOR3(140.0f, 400.0f, 0.0f), D3DXVECTOR3(1.5f, 0.0f, 0.0f), CParts::PARTSTYPE_MIDLE, 7, 20);	// ����
}