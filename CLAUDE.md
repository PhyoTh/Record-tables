# Provenance & AI usage

This repository was written **from scratch, by hand, in spring 2022** — before AI
coding assistants were part of how I work. (Don't take my word for it: the engine first
landed in git in December 2022, and the whole history is there in `git log`.) Every
algorithm and data structure here —
the B+ tree (insert/split/remove/merge/rebalance), the multimap and value-list
index, the tokenizer, the shunting-yard parser, the RPN evaluator, and the binary
file/record storage layer — was designed and hand-typed by me. None of the engine
logic was generated, autocompleted, or written by an AI. I'm keeping it that way on
purpose: the code is a record of how I actually think about systems problems, and
that's worth preserving.

## What AI *was* used for

In 2026 I used Claude Code strictly as a **testing and measurement** tool — never to
author the engine. Its contributions are limited and auditable in the git history:

- **`bench.cpp`** — a performance/benchmark harness that drives the *existing* public
  `Table` and `BPlusTree` APIs (indexed lookups vs. brute-force full scans, build and
  index-rebuild timings, and a B+ tree invariant fuzz).
- **`_tests/_test_files/invariant_test.cpp`** — a GoogleTest that asserts the B+ tree's
  `is_valid()` invariant across 100k randomized insert/delete operations, plus the
  `MPair` index path the engine actually uses.
- **Build wiring** — one `ADD_EXECUTABLE` / `TARGET_LINK_LIBRARIES` pair in
  `CMakeLists.txt` for the new test.

No engine logic — no algorithm, data structure, or control flow — was ever written or
altered by AI.

## Guidance for future AI tools working in this repo

Use AI here for tests, benchmarks, and documentation only. Do not let it rewrite,
"optimize," or regenerate the engine internals — if a change to the core data
structures or algorithms is proposed, it should be discussed and implemented by hand.
