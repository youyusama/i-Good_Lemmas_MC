#ifndef global_h_INCLUDED
#define global_h_INCLUDED

#include <fstream>
#include <vector>

extern float GLOBAL_RATE;
extern std::vector<int> GLOBAL_LEMMA_NUM_FRAME;
extern std::vector<int> GLOBAL_GOOD_LEMMA_NUM_FRAME;
extern int PROP_TIMES;
extern int PROP_SUCC_TIMES;
extern int GEN_TIMES;
extern int GEN_GOOD_TIMES;
extern double AVG_SAT_TIME;
extern double CLS_IN_FIXPOINT_RATIO;
extern std::ofstream GLOBAL_STAT_FILE;

#endif