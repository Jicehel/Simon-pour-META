#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#define NUM_HIGHSCORE 5
#define NAMELENGTH 12
extern void initHighscore();
extern void drawHighScores(unsigned int mode);
extern int highscore[NUM_HIGHSCORE];
extern bool drawNewHighscore(unsigned int newscore);
extern void saveHighscore(unsigned int test_score);

#endif _HIGHSCORE_H_
