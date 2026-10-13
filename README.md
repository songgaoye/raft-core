# raft-core

`raft-core` is a C++17 Raft protocol core built as a reconstruction exercise.
It deliberately separates protocol state from RPC, timers, disk I/O, and
application callbacks. It is not a drop-in replacement for braft.

## Design Direction

```text
ClusterSimulator -> RaftCore <- TimerDriver
                     |     |
                 LogStore  ApplyExecutor
                     |
                  MetaStore
```

`RaftCore` will be a single-owner state machine: only its event handler may
modify term, voted-for, role, membership, log progress, and commit state.
Adapters report external completion as events; they never mutate the protocol
state directly.

## Build and Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
```

## Current Scope

1. Membership and quorum ballots.
2. Deterministic virtual network with directed partitions and no wall clock.
3. Election, durable term/vote state, AppendEntries, commit/apply, snapshots,
   and membership transitions will follow in small verified stages.

## Provenance

Some test scenarios are dependency-free behavioral rewrites of the Apache-2.0
licensed braft test suite, especially its ballot and joint-consensus cases.
They are not copied code and this library has no brpc, bthread, protobuf, or
braft runtime dependency. Each future migrated scenario must retain source
attribution in its test file or accompanying documentation.
