## Fillomino Solver & Polyomino Tools
This project contains original solving methods for the Fillomino puzzle, implemented in C++ with a strong focus on backtracking-based search and constraint reasoning.
In addition to the solver itself, the repository provides custom fixed polyomino generation utilities and solution validation tools. <br>
The algorithms and overall approach were developed independently as part of my bachelor’s thesis, with a particular emphasis on formalizing logical solving strategies and integrating them into a computational solver.

## Usage

The program can be run as follows:

```bash
make release
./poly [-g polyomino_file | fillomino_file polyomino_file]
```
 ### Options

- `-g polyomino_file`  
Generates codes for all fixed k-ominoes for k ∈ {1, 2, ..., 9} and saves them to the specified `polyomino_file`.

- `fillomino_file polyomino_file`  
Solves a Fillomino puzzle specified in `fillomino_file` using polyomino codes loaded from `polyomino_file`.  
Prints the solution to standard output along with the time taken.

### Fillomino File Format

The Fillomino file should follow this structure:
- The first line contains the puzzle dimensions:  
     ```
     n m
     ```  
     where `n` = number of rows, `m` = number of columns.
- The next lines contain `n*m` integers representing the cells row by row:  
     - pre-filled cell values,
     - `0`s represent empty cell to be filled during solving,
     - supported values: `k ∈ [1, 9]`.
