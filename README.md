<div style="text-align: justify;">

- weird behaviour note: Mingw alone has a problem with getting data that is stored in a different hpp file (vars.tot_occ) where it assign the occurances in a random positive block. The differences between occs stay the same so no problem, however weird. Debug function and run through json functions as expected.

- use watched literals during backtracking with learned clauses differenlty

- Leave preprocessing to the end of implementation, start woth backtracking + Deletion and watched

- Do NOT use BCE

- NIVER: Do NOT delete the two producing resolvents once a resolved clause is added



# Group_K_Project_2

This project implements the DPLL algorithm enhanced with Unit Propagation (UP) and Pure Literal Elimination (PLE).   
  
The solver offers four heuristic options: Incremental (INC=0), Dynamic Largest Individual Sum (DLIS=1), Dynamic Largest Combined Sum (DLCS=2) and Jeroslow-Wang (JW=3). Both satisfiable and unsatisfiable files (previously found under `./test`, `./sat`, `./unsat`) have been consolidated into two designated `./test` and `./comp` folders respectively, identifiable by their affix (_u = unsat, _s = sat).

## Build + Run
To build the project, enter `make`. To run the solver on a test file, enter `./main [testfile] [heur]`. To build and run immediately, enter `make run arg=[testfile] [heur]`. 

**Batch execution:**

If compiled, you can run a range of test or comp files at once with a chosen heuristic `.\run.ps1 -type [char] -start [int] -end [int] -heur [int]`. 
By omitting the `-heur` parameter, you can run all heuristics over the specified range.

It is likely that you first have to allow the execution of local scripts by runnning i.e. `Set-ExecutionPolicy -Scope Process Bypass`.

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






