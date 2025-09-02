# Record-tables
A lightweight C++ record/table engine with a minimal SQL-like query pipeline, B+ tree indexing, and binary file-backed storage. Built to showcase systems fundamentals: parsing, indexing, file I/O, and core data structures.

Demo video: https://youtu.be/2pizsEKHbJU

## At a glance (recruiter-friendly)
- Systems programming in modern C++ (C++11) with CMake
- End-to-end query flow: Tokenizer → Shunting-yard parser → RPN evaluation
- Indexing with a B+ tree for logarithmic lookups
- Purpose-built data structures: doubly linked list, stack, queue
- Binary on-disk storage for tables
- Unit tests with GoogleTest (fetched automatically)

## Key features
- Binary storage engine: Persists table data to disk using compact binary files.
- SQL-like mini-language: Tokenizes, parses (shunting-yard), and evaluates expressions in Reverse Polish Notation.
- B+ tree index: Accelerates lookups and range queries with O(log n) search.
- Modular data structures: Reusable doubly linked list, stack, and queue abstractions.
- Cross-platform build: CMake-based, with automatic GoogleTest integration.

## Architecture overview
- Tokenizer: Splits input query text into typed tokens.
- Parser (Shunting-yard): Converts infix expressions into RPN, handling operator precedence and associativity.
- RPN Evaluator: Executes query plans over table rows and available indexes.
- Storage layer: Reads/writes fixed-format records to binary files.
- Indexing: B+ tree maintains sorted keys and supports fast point and range queries.
- Support structures: Doubly linked list, stack, and queue aid parsing and execution.

## Data structures & algorithms
- B+ tree: Balanced, disk-friendly structure enabling efficient ordered traversals and range scans.
- Shunting-yard algorithm: Deterministic linear-time parsing for expressions with precedence.
- RPN evaluation: Stack-based execution for compact, efficient expression evaluation.

## Build
Prerequisites:
- CMake 3.5+
- A C++11-compatible compiler (GCC/Clang/MSVC)

Steps:
```bash
git clone https://github.com/PhyoTh/Record-tables.git
cd Record-tables
mkdir build && cd build
cmake ..
cmake --build .
```

Binaries are emitted to build/bin on most platforms.

Notes:
- On GNU compilers, code coverage flags (-fprofile-arcs -ftest-coverage) are enabled by default.
- GoogleTest is fetched automatically via CMake (FetchContent).

## Run
From the build directory:
```bash
# Main demo executable
./bin/main
```

## Tests
Two test executables are generated:
```bash
./bin/basic_test
./bin/testB
```
(Tests are linked with GoogleTest; run the executables directly.)

## Usage
The engine supports a small, SQL-like expression language. Typical flow:
1) Load or create a table backed by a binary file
2) Run a SELECT-style query with filter expressions
3) Results are evaluated via the tokenizer → parser → RPN → executor pipeline, using B+ tree indexes when applicable

See main.cpp for concrete examples of constructing and running queries.

## Project layout
- CMakeLists.txt — Build configuration (C++11, GoogleTest, outputs to build/bin)
- main.cpp — Example entry point and demo
- includes/ — Core engine sources (storage, tokenizer, parser, RPN, B+ tree, data structures)
- _tests/_test_files/ — Test sources (GoogleTest-based)

## Known issues
- Cannot delete all files after selecting a table.

## Roadmap
- Improve delete/cleanup semantics for tables and associated index files
- Expand the query language (projection, joins, aggregation)
- Add schema management and metadata catalogs
- Strengthen persistence, recovery, and durability guarantees
- Increase test coverage and add CI

## What this project demonstrates
- Practical systems design: file I/O, binary formats, and indexing
- Compiler techniques in miniature: tokenization, precedence parsing, and stack-based evaluation
- Algorithmic rigor: balanced tree structures and complexity-aware design
- Engineering hygiene: unit testing with GoogleTest and CMake-driven builds

## Demo
Watch the engine in action: https://youtu.be/2pizsEKHbJU

## Author
Maintainer: @PhyoTh
