#include "global.h"

float GLOBAL_RATE = 0;
extern std::vector<int> GLOBAL_LEMMA_NUM_FRAME = std::vector<int>();
extern std::vector<int> GLOBAL_GOOD_LEMMA_NUM_FRAME = std::vector<int>();
extern int PROP_TIMES = 0;
extern int PROP_SUCC_TIMES = 0;
extern int GEN_TIMES = 0;
extern int GEN_GOOD_TIMES = 0;
extern double AVG_SAT_TIME = 0;
extern double CLS_IN_FIXPOINT_RATIO = 0;
std::ofstream GLOBAL_STAT_FILE;