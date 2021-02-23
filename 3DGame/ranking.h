//=============================================================================
//
// �����L���O�̏��� [ranking.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);	

void Ranking(void);					//�����L���O�̃X�R�A�̏��ԓ���ւ�
void SetVertexRanking(int nCnt);	//�����L���O�̒��_���W�̐ݒ�

void RankingBg(int nCnt);			//�����L���O�̔w�i
void RankingLogo(int nCnt);			//�����L���O�̃��S
void RankingRank(int nCnt);			//�����L���O�̏���
void RankingScore(int nCnt);		//�����L���O�̃X�R�A

#endif
