//=============================================================================
//
// サウンド処理
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// クラス
//=============================================================================
class CSound
{
public:
	CSound();
	virtual ~CSound();

	//サウンドファイル
	typedef enum
	{
		// 効果音
		SOUND_LABEL_SE_SHOTPLAYER,		// 弾発射音
		SOUND_LABEL_SE_SHOTENEMY,		// 敵発射音
		SOUND_LABEL_SE_SHOTBOSS,		// ボス弾発射音
		SOUND_LABEL_SE_EXPLOSION,		// 爆発音
		SOUND_LABEL_SE_PARTSBREAK,		// パーツ破損
		SOUND_LABEL_SE_ALERT,			// アラート
		SOUND_LABEL_SE_HIT,				// ヒット
		SOUND_LABEL_SE_LASER,			// レーザー音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//パラメータ構造体
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif
