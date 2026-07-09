// bench.cpp  -- performance harness for Record-tables
// Place this at the repo root (next to main.cpp). Portable C++11, no windows.h.
//
// It uses the real Table API so numbers reflect the actual engine, and a
// brute-force full-scan baseline for the indexed-vs-unindexed comparison.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <fstream>

#include "includes/table/table.h"
#include "includes/bplustree/bplustree.h"

using namespace std;
using clk = chrono::high_resolution_clock;

static string pad(long v, int width) {
    string s = to_string(v);
    if ((int)s.size() < width) s = string(width - (int)s.size(), '0') + s;
    return s;
}

// Brute-force full table scan over the .tbl file.
// Returns record numbers where field[idx] `rel` value (lexicographic string compare,
// matching the engine's own string-keyed comparisons).
static vector<long> brute_scan(const string& tableName, int idx,
                               const string& rel, const string& value) {
    vector<long> out;
    fstream f;
    open_fileRW(f, (tableName + ".tbl").c_str());
    long recno = 0;
    while (true) {
        f.clear();                 // clear any eof from the previous read
        FileRecord r;
        long pos = r.read(f, recno);
        if (pos <= 0) break;
        vector<string> v = r.get_vector();
        if (idx < (int)v.size()) {
            const string& c = v[idx];
            bool m = (rel == "=")  ? (c == value)
                   : (rel == ">")  ? (c >  value)
                   : (rel == "<")  ? (c <  value)
                   : (rel == ">=") ? (c >= value)
                   : (rel == "<=") ? (c <= value) : false;
            if (m) out.push_back(recno);
        }
        ++recno;
    }
    f.close();
    return out;
}

int main() {
    // ---- B+ tree invariant fuzz (fills bullet 9) --------------------------
    {
        // Distinct-key mode (dups=false): a plain-int tree must NOT enable the
        // duplicate-merge path, because BPlusTree merges equal keys via operator+
        // -- which for `int` is arithmetic addition (5 + 5 -> 10) and corrupts key
        // order. That merge is only meaningful for the engine's real index type,
        // MPair<key,value>, whose operator+ concatenates the record-number list.
        // This fuzz validates the tree's structural machinery (splits / merges /
        // rebalancing) across insert+delete; the MPair value-list path is exercised
        // by the Table benchmarks below and by the GoogleTest suite.
        BPlusTree<int> tree(false);
        mt19937 r2(7);
        vector<int> present;
        const int OPS = 100000;
        bool ok = true;
        for (int i = 0; i < OPS; ++i) {
            bool doInsert = present.empty() || (r2() % 3 != 0);  // ~2/3 inserts
            if (doInsert) {
                int k = (int)(r2() % 1000);
                tree.insert(k);
                present.push_back(k);
            } else {
                int j = (int)(r2() % present.size());
                tree.remove(present[j]);
                present.erase(present.begin() + j);
            }
            if ((i % 1000) == 0 && !tree.is_valid()) { ok = false; break; }
        }
        cout << "B+ tree invariant after " << OPS
             << " randomized insert/delete ops: "
             << (ok ? "VALID" : "BROKEN") << "\n\n";
    }

    // ---- Table benchmarks -------------------------------------------------
    // Add 100000 if you're patient (~1 GB file, several minutes to build).
    vector<long> sizes = {1000, 10000, 50000};
    const int idx_id = 0, idx_age = 2;
    const int K_IDX = 1000;   // index queries per measurement
    const int K_SCAN = 20;    // full scans per measurement (kept small; each is O(N) disk reads)

    mt19937 rng(42);

    cout << left
         << setw(9)  << "N"
         << setw(12) << "build_ms"
         << setw(12) << "reload_ms"
         << setw(13) << "pt_idx_us"
         << setw(13) << "pt_scan_us"
         << setw(12) << "pt_speedup"
         << setw(13) << "rg_idx_us"
         << setw(13) << "rg_scan_us"
         << setw(12) << "rg_speedup" << "\n";

    for (long N : sizes) {
        string name = "bench_" + to_string(N);
        vector<string> fields = {"id", "name", "age"};
        const char* pool[] = {"Joe","Karen","Flo","Jack","Amy","Ravi","Mei","Sam"};

        // build
        auto t0 = clk::now();
        {
            Table t(name, fields);
            for (long i = 0; i < N; ++i) {
                vector<string> row = { pad(i, 7), pool[rng() % 8], pad((long)(rng() % 100), 3) };
                t.insert_into(row);
            }
        }
        auto t1 = clk::now();
        double build_ms = chrono::duration<double, milli>(t1 - t0).count();

        // reload (rebuild in-memory indices from disk)
        auto r0 = clk::now();
        Table t(name);
        auto r1 = clk::now();
        double reload_ms = chrono::duration<double, milli>(r1 - r0).count();

        long sink = 0;

        // point queries on unique id (returns exactly 1 record)
        vector<string> ids;
        for (int k = 0; k < K_IDX; ++k) ids.push_back(pad((long)(rng() % N), 7));

        auto p0 = clk::now();
        for (int k = 0; k < K_IDX; ++k) sink += (long)t.get_recnos("id", "=", ids[k]).size();
        auto p1 = clk::now();
        double pt_idx_us = chrono::duration<double, micro>(p1 - p0).count() / K_IDX;

        auto ps0 = clk::now();
        for (int k = 0; k < K_SCAN; ++k) sink += (long)brute_scan(name, idx_id, "=", ids[k]).size();
        auto ps1 = clk::now();
        double pt_scan_us = chrono::duration<double, micro>(ps1 - ps0).count() / K_SCAN;

        // range queries: age > random threshold
        vector<string> thr;
        for (int k = 0; k < K_IDX; ++k) thr.push_back(pad((long)(rng() % 100), 3));

        auto g0 = clk::now();
        for (int k = 0; k < K_IDX; ++k) sink += (long)t.get_recnos("age", ">", thr[k]).size();
        auto g1 = clk::now();
        double rg_idx_us = chrono::duration<double, micro>(g1 - g0).count() / K_IDX;

        auto gs0 = clk::now();
        for (int k = 0; k < K_SCAN; ++k) sink += (long)brute_scan(name, idx_age, ">", thr[k]).size();
        auto gs1 = clk::now();
        double rg_scan_us = chrono::duration<double, micro>(gs1 - gs0).count() / K_SCAN;

        cout << fixed << setprecision(2) << left
             << setw(9)  << N
             << setw(12) << setprecision(1) << build_ms
             << setw(12) << reload_ms
             << setw(13) << setprecision(2) << pt_idx_us
             << setw(13) << pt_scan_us
             << setw(12) << (pt_idx_us > 0 ? pt_scan_us / pt_idx_us : 0)
             << setw(13) << rg_idx_us
             << setw(13) << rg_scan_us
             << setw(12) << (rg_idx_us > 0 ? rg_scan_us / rg_idx_us : 0)
             << "\n";
        cout.flush();
        (void)sink;
    }

    cout << "\nDone. Delete bench_*.tbl / bench_*.fields when finished.\n";
    return 0;
}
