<div style="text-align: justify;">

# Group_K_Project_3

This project implements the DPLL algorithm enhanced with Unit Propagation (UP) and Pure Literal Elimination (PLE).   
  
The solver offers four heuristic options: Incremental (INC=0), Dynamic Largest Individual Sum (DLIS=1), Dynamic Largest Combined Sum (DLCS=2) and Jeroslow-Wang (JW=3). Both satisfiable and unsatisfiable files (previously found under `./test`, `./sat`, `./unsat`) have been consolidated into two designated `./test` and `./comp` folders respectively, identifiable by their affix (_u = unsat, _s = sat).

## Build + Run
To build the project, enter `make`. To run the solver on a test file, enter `./main [testfile] [heur]`. To build and run immediately, enter `make run arg=[testfile] [heur]`. 

**Examples:**

`make run arg=c1 0` (build and run the .exe on the first competition file with INC)

`./main c1 2` (run the compiled .exe on the first competition file with DLCS)
  
`.\run.ps1 -type 't' -start 1 -end 30 -heur 1` (run the compiled .exe on the first 30 test files with DLIS)

`python run.py c 1 30 2` (run the compiled .exe on the first 30 comp files with DLCS)

## Results

We plotted our runtimes by sorting them in increasing order using `mkplot`. For exact runtimes and plots with higher timeout, see folder `./solution/runtime` and `./cactus` respectively.

The number of solved instances within 60 seconds per heuristic (INC: 109, DLIS: 124, DLCS: 126, JW: 124) and their performance curve can be studied below:

![Alt text](cactus/cactusPlot60secs.png/?raw=true "Optional Title")

**Test Environment Specifications:**
- **Hardware:**
  - Processor: AMD Ryzen 7 5700U @ 1.80 GHz
  - Memory: 16 GB
- **Software:**
  - Operating System: Windows 11 Pro
  - Compiler: GCC 11.2.0

</div>






