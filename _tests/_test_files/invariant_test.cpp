// invariant_test.cpp -- GoogleTest coverage for B+ tree structural invariants.
//
// Backs the resume claim: "Validated B+ tree structural invariants across 100k
// randomized insert/delete operations via an is_valid() checker."
//
// Two paths are exercised:
//   1. StructuralFuzz -- a plain-int tree (dups=false) under a randomized
//      insert/delete workload, stressing splits, merges, transfers, and root
//      shrink/grow. Note: dups=false is required for a plain-int tree because
//      BPlusTree merges equal keys via operator+, which for `int` is arithmetic
//      addition and would corrupt key order.
//   2. IndexTypeMPair -- the exact index element the Table engine uses,
//      MPair<int,long> with dups=true, whose operator+ concatenates the
//      record-number value_list. The engine is insert+query only (no row
//      deletion), so this validates the real production path.
//
// Portable C++11 (no windows.h) so it builds and runs everywhere.

#include "gtest/gtest.h"
#include <iomanip> // must precede the bplustree headers (set_class.h uses setw)
#include <iostream>
#include <vector>
#include <random>

#include "../../includes/bplustree/bplustree.h"
#include "../../includes/bplustree/multimap.h" // pulls in MPair

using namespace std;

// 100k randomized insert/delete ops on a plain-int tree; the structure must
// remain a valid B+ tree at every checkpoint.
TEST(BPLUSTREE_INVARIANT, StructuralFuzz100kInsertDelete)
{
    BPlusTree<int> tree(false);
    mt19937 rng(7);
    vector<int> present;
    const int OPS = 100000;

    for (int i = 0; i < OPS; ++i)
    {
        bool doInsert = present.empty() || (rng() % 3 != 0); // ~2/3 inserts
        if (doInsert)
        {
            int k = (int)(rng() % 1000);
            tree.insert(k);
            present.push_back(k);
        }
        else
        {
            int j = (int)(rng() % present.size());
            tree.remove(present[j]);
            present.erase(present.begin() + j);
        }
        if ((i % 1000) == 0)
            ASSERT_TRUE(tree.is_valid()) << "invariant broke at op " << i;
    }
    EXPECT_TRUE(tree.is_valid());
}

// The real engine index type: MPair with a merged value_list under dups=true.
// The Table engine inserts and queries but never deletes rows, so 100k inserts
// mirror the production workload.
TEST(BPLUSTREE_INVARIANT, IndexTypeMPair100kInsert)
{
    BPlusTree<MPair<int, long>> tree(true);
    mt19937 rng(7);
    const int OPS = 100000;

    for (int i = 0; i < OPS; ++i)
    {
        tree.insert(MPair<int, long>((int)(rng() % 1000), (long)i));
        if ((i % 1000) == 0)
            ASSERT_TRUE(tree.is_valid()) << "invariant broke at op " << i;
    }
    EXPECT_TRUE(tree.is_valid());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n---------- running invariant_test.cpp ----------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
