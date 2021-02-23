//=============================================================================
//
// ランキングの処理 [ranking.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);	

void Ranking(void);					//ランキングのスコアの順番入れ替え
void SetVertexRanking(int nCnt);	//ランキングの頂点座標の設定

void RankingBg(int nCnt);			//ランキングの背景
void RankingLogo(int nCnt);			//ランキングのロゴ
void RankingRank(int nCnt);			//ランキングの順位
void RankingScore(int nCnt);		//ランキングのスコア

#endif
