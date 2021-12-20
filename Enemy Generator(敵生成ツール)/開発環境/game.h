//=============================================================================
//
// �Q�[������ [game.h]
// Author : taiki hayasaka
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CPlayer;				// �v���C���[
class CBg;					// �w�i
class CEnemy;				// �G
class CNumber;				// �i���o�[
class CScore;				// �X�R�A
class CPolygon;				// �|���S��
class CLife;				// ���C�t
class CParts;				// �{�X�p�[�c

//=============================================================================
// �N���X
//=============================================================================
class CGame : public CScene
{
public:
	// ���U���g�̎��
	typedef enum
	{
		RESULTMODE_NONE = 0,
		RESULTMODE_GAMECLEAR,		// �N���A
		RESULTMODE_GAMEOVER,		// �Q�[���I�[�o�[
		RESULTMODE_MAX,
	}RESULTMODE;

	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadAll(void);										// �S�Ẵe�N�X�`����ǂݍ���
	static void UnloadAll(void);									// �S�Ẵe�N�X�`����j��

	void BossAll(void);												// �{�X�Ǘ�

	//�f�o�C�X�̎擾
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// �v���C���[�̎擾
	static CBg *GetBg(void) { return m_pBg; }						// �w�i�̎擾
	static CEnemy *GetEnemy(void) { return m_pEnemy; }				// �G�̎擾
	static CNumber *GetNumber(void) { return m_pNumber; }			// �i���o�[�̎擾
	static CScore *GetScore(void) { return m_pScore; }				// �X�R�A�̎擾
	static CPolygon *GetPolygon(void) { return m_pPolygon; }		// �|���S���̎擾
	static CParts *GetParts(void) { return m_pParts; }				// �p�[�c

	static RESULTMODE GetResult(void) { return m_resultmode; }		// ���U���g�̎擾
	static void SetResult(RESULTMODE resultmode);					// ���U���g�󂯎��

private:
	static CPlayer *m_pPlayer;										// �v���C���[�̃|�C���^
	static CBg *m_pBg;												// �w�i�̃|�C���^
	static CEnemy *m_pEnemy;										// �G�̃|�C���^
	static CNumber *m_pNumber;										// �i���o�[�̃|�C���^
	static CScore *m_pScore;										// �X�R�A�̃|�C���^
	static CPolygon *m_pPolygon;									// �|���S���̃|�C���^
	static CParts *m_pParts;										// �p�[�c

	static RESULTMODE m_resultmode;									// ���U���g���[�h
};

#endif