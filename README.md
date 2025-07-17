# Dungeon Princess Solver

This project solves the classic "Dungeon Princess" problem: given a matrix (dungeon) with positive and negative values, find the path from the top-left to the bottom-right corner (where the princess is), ensuring the hero never dies (HP never <= 0) and the initial HP is minimized.

## The Problem

- Each cell in the matrix can increase or decrease the hero's HP.
- The goal is to find the minimum initial HP required to save the princess, as well as the path taken.

## Approaches Used

### 1. Recursive (Backtracking)
- Explores all possible paths recursively.
- Calculates the required HP for each path.
- Can be optimized with memoization (top-down dynamic programming).

**Pros:**
- Easy to implement and understand.
- Good for small matrices.

**Cons:**
- Very slow for large matrices (combinatorial explosion).
- Can cause stack overflow.

### 2. Dijkstra (Minimum Path Search)
- Models the problem as a graph, where each cell is a node.
- Uses a priority queue to always expand the path with the highest remaining HP.
- Efficiently finds the optimal path.

**Pros:**
- Very efficient for large matrices.
- Guarantees the minimum initial HP.

**Cons:**
- More complex implementation.
- Requires additional data structures (priority queue/min-heap).

## How to Run the Project

1. Build the project (requires CMake and a C++ compiler):

```sh
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```

2. Run the generated binary:

```sh
./cmake-build-debug/untitled.exe
```

3. The result will be saved in `cmake-build-debug/dungeon_result.json` with the path, minimum HP, and the input matrix.

## Interesting C++ Features
- Use of `std::priority_queue` for Dijkstra.
- Efficient handling of matrices and coordinate pairs.
- Serialization of results to JSON (can use libraries like nlohmann/json).
- Manual memory management and performance optimizations.

---

Feel free to modify the input dungeon and test different scenarios!
