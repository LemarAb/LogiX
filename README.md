<div style="text-align: justify;">

## Build + Run
To build the project, enter `make`. To run the solver on a test file, enter `./main [testfile]`. To build and run immediately, enter `make run arg=[testfile] [heur]`. To run with preprocessing `./main [testfile] [-pre]` or `make run arg=[testfile] [prepr=-pre]`. A similar approach for proof logging:  `./main [testfile] [-proof]` and  `make run arg=[testfile] [proof=-proof]`

**Examples:**

`make run arg=c1` (build and run the .exe on the first competition file )

`./main c1 -pre` (run the compiled .exe on the first competition file with preprocessing)

`./main c2 -proof` (run the compiled .exe on the first competition file with proof logging)
  
`.\run.ps1 -type 't' -start 1 -end 30 ` (run the compiled .exe on the first 30 test files)

`python runCDCL.py c 1 30` (run the compiled .exe on the first 30 comp files)

## Experimenal evaluation
To analyze the solver's performance, we conducted experiments and visualized the results using the images in the cactus folder. The cactus plots illustrate the solver's runtime and efficiency across different test cases and used heuristics.

<p align="center">
  <img src="cactus/CDCLcactusplot.png" alt="Cactus Runtime" height="400">
  <br>
  <em>Figure 1: CDCL Runtime Plot</em>
</p>
<p align="center">
  <img src="cactus/cactusPlot350secs.png" alt="Cactus Runtime" height="400">
  <br>
  <em>Figure 2: DPDL Runtime Plot</em>
</p>

# Background

The Davis-Putnam-Logemann-Loveland (DPLL) algorithm and the Conflict-Driven Clause Learning (CDCL) algorithm are two fundamental approaches to solving the Boolean satisfiability problem (SAT).

## DPLL

DPLL is a backtracking algorithm that extends the basic backtracking search with:

Unit Propagation: If a clause has only one unassigned literal, it must be true.

Pure Literal Elimination: If a literal appears only with one polarity, it can be assigned accordingly.

Backtracking: If a contradiction occurs, the algorithm reverts the last decision.

## Heuristics in DPLL

Various heuristics improve DPLL's efficiency by guiding variable selection. The most common are:

1. DLCS (Dynamic Largest Combined Sum): Selects the variable that appears in the most clauses, considering both positive and negative occurrences.

2. DLIS (Dynamic Largest Individual Sum): Chooses the variable that occurs most frequently in satisfied clauses, prioritizing the most impactful assignment.

3. JW (Jeroslow-Wang Heuristic): Weights variables based on the sum of inverse clause lengths, prioritizing those in shorter clauses since they are more constraining.

## CDCL

CDCL enhances DPLL by adding conflict-driven learning, making it significantly more efficient for many practical SAT problems. It introduces:

Clause Learning: New clauses are added based on conflicts, preventing the solver from repeating the same mistakes.

Non-Chronological Backtracking (Backjumping): Instead of undoing only the last decision, the solver jumps back multiple levels in the decision tree.

Restart Strategies: The solver periodically restarts with learned clauses to escape difficult search spaces.

## Test Environment Specifications:
- **Hardware:**
  - Processor: AMD Ryzen 7 5700U @ 1.80 GHz
  - Memory: 16 GB
- **Software:**
  - Operating System: Windows 11 Pro
  - Compiler: GCC 11.2.0

</div>






