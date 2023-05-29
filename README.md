Artifacts of the paper 
  *Searching for i-Good Lemmas to Accelerate Safety Model Checking*
by
  Yechuan Xia, Anna Becchi, Alessandro Cimatti, Alberto Griggio,
  Jianwen Li, Geguang Pu,
published at CAV 2023.


## Content
---

```
IC3ref/         # Modified versions of IC3ref
nuXmv/          # The bit-level engine of the nuXmv model checker 
                # (called nuXmv-ic3 here). Please note this is subject 
                # to the license in nuXmv/LICENSE
simple_CAR/     # Modified versions of IC3ref
benchmarks.txt  # Benchmark list from HWMCC 15-17
```

## Usage
---
### 1. The modified IC3ref 

To build:

```
IC3ref/minisat$ make
IC3ref/aiger$ ./configure.sh
IC3ref/aiger$ make
IC3ref$ make
```

To Run:

```
$ ./IC3 [-br][-rs] <AIGER_file.aig> <OUTPUT_PATH>
```

- -br: enable branching heuristic
- -rs: enable refer-skipping heuristic

---
### 2. nuXmv (binary)
```
$ ./nuXmv-ic3 -a ic3 -s cadical -m 1 -u 4 -I 1 -D 0 -g 1 -X 0 -c 0 -p 1 -d 2 -G 1 -P 1 -A 100 -O 1 <AIGER_file.aig>
```
- -O 1: original varordering heuristic
- -O 3: enable both branching and refer-skipping heuristics
- -O 4: enable refer-skipping heuristic
- -O 5: enable branching heuristic

---
### 3. simple_CAR 

To build:

```
simple_CAR/$ make
```

To Run:

```
$ ./simpleCAR -f|-b [-br 1][-rs] <AIGER_file.aig> <OUTPUT_PATH>
```
- -f: forward searching
- -b: backward searching
- -br 1: enable branching heuristic
- -rs: enable refer-skipping heuristic
