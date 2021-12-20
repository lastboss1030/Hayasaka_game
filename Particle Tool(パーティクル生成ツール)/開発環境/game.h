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
class CPolygon;				// �|���S��

//=============================================================================
// �N���X
//=============================================================================
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadAll(void);										// �S�Ẵe�N�X�`����ǂݍ���
	static void UnloadAll(void);									// �S�Ẵe�N�X�`����j��

	//�f�o�C�X�̎擾
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// �v���C���[�̎擾
	static CPolygon *GetPolygon(void) { return m_pPolygon; }		// �|���S���̎擾

private:
	static CPlayer *m_pPlayer;										// �v���C���[�̃|�C���^
	static CPolygon *m_pPolygon;									// �|���S���̃|�C���^
};

#endif