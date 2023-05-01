Source code of the modified versions of IC3ref and simple_CAR of paper *Searching for i-Good Lemmas to Accelerate Safety Model Checking*.

---
## The modified IC3ref 

To build:

```
IC3ref/minisat$ make
IC3ref/aiger$ ./configure.sh
IC3ref/aiger$ make
IC3ref$ make
```

To Run:

`$ IC3 [-br][-rs] <AIGER_file.aig> <OUTPUT_PATH>`

- -br: enable branching heuristic
- -rs: enable refer-skipping heuristic

---
## simple_CAR 

To build:

`simple_CAR/$ make`

To Run:

`$ simpleCAR -f [-br 1][-rs] <AIGER_file.aig> <OUTPUT_PATH>`

- -br 1: enable branching heuristic
- -rs: enable refer-skipping heuristic
