/*********************************************************************
Copyright (c) 2013, Aaron Bradley

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

extern "C" {
#include "aiger.h"
}
#include "IC3.h"
#include "Model.h"
#include "global.h"

int main(int argc, char **argv) {
  unsigned int propertyIndex = 0;
  bool basic = false, branching = false, skipping_refer = false, stat = false;
  int random = 0;
  int verbose = 0;
  string inputfile, outputpath;
  bool has_inputfile = false, has_outputpath = false;
  for (int i = 1; i < argc; ++i) {
    if (string(argv[i]) == "-v")
      // option: verbosity
      verbose = 2;
    else if (string(argv[i]) == "-s")
      // option: print statistics
      verbose = max(1, verbose);
    else if (string(argv[i]) == "-r") {
      // random seed
      random = atoi(argv[i + 1]);
      i++;
    } else if (string(argv[i]) == "-br")
      branching = true;
    else if (string(argv[i]) == "-rs")
      skipping_refer = true;
    else if (string(argv[i]) == "-stat")
      stat = true;
    else if (string(argv[i]) == "-b")
      // option: use basic generalization
      basic = true;
    // xyc set input and output
    else if (!has_inputfile) {
      // optional argument: set property index
      // propertyIndex = (unsigned)atoi(argv[i]);
      inputfile = string(argv[i]);
      has_inputfile = true;
    } else if (!has_outputpath) {
      outputpath = string(argv[i]);
      if (outputpath[outputpath.length() - 1] != '/') {
        outputpath += "/";
      }
      has_outputpath = true;
    }
  }

  FILE *fp;
  fp = fopen(inputfile.c_str(), "r");
  // read AIGER model
  aiger *aig = aiger_init();
  const char *msg = aiger_read_from_file(aig, fp);
  if (msg) {
    cout << msg << endl;
    return 0;
  }
  // create the Model from the obtained aig
  Model *model = modelFromAiger(aig, propertyIndex);
  aiger_reset(aig);
  if (!model) return 0;

  // xyc time
  clock_t t_start = clock();

  // xyc get outputfile name
  auto startIndex = inputfile.find_last_of("/");
  if (startIndex == string::npos) {
    startIndex = 0;
  } else {
    startIndex++;
  }
  auto endIndex = inputfile.find_last_of(".");
  string outputfile = outputpath + inputfile.substr(startIndex, endIndex - startIndex);
  ofstream output;
  output.open(outputfile + ".res");

  GLOBAL_STAT_FILE.open(outputfile + ".log");

  // model check it
  bool rv = IC3::check(*model, verbose, basic, random, branching, skipping_refer, stat);

  // print 0/1 according to AIGER standard
  output << !rv << endl;
  output << static_cast<double>(clock() - t_start) / CLOCKS_PER_SEC << endl;

  GLOBAL_STAT_FILE << "avg sat time: " << AVG_SAT_TIME << endl;
  GLOBAL_STAT_FILE << "all rate: " << GLOBAL_RATE << endl;
  GLOBAL_STAT_FILE << "gen good rate: " << (float)GEN_GOOD_TIMES / (float)GEN_TIMES << endl;
  GLOBAL_STAT_FILE << "prop succ rate: " << (float)PROP_SUCC_TIMES / (float)PROP_TIMES << endl;
  double temp_all_lemma_num = 0;
  for (int i = 0; i < GLOBAL_LEMMA_NUM_FRAME.size(); i++) {
    temp_all_lemma_num += GLOBAL_LEMMA_NUM_FRAME[i];
    if (GLOBAL_LEMMA_NUM_FRAME[i] == 0) CLS_IN_FIXPOINT_RATIO = GLOBAL_LEMMA_NUM_FRAME[i + 1];
  }
  CLS_IN_FIXPOINT_RATIO /= temp_all_lemma_num;
  GLOBAL_STAT_FILE << "cls in fixpoint ratio: " << CLS_IN_FIXPOINT_RATIO << endl;
  GLOBAL_STAT_FILE << "lemma num frame: " << endl;
  for (int i = 0; i < GLOBAL_LEMMA_NUM_FRAME.size(); i++) {
    GLOBAL_STAT_FILE << GLOBAL_LEMMA_NUM_FRAME[i] << " ";
  }


  delete model;

  return 1;
}
