# 5–10 Minute Evaluation Notes

## 0:00–1:00 — Problem
Conventional full copies waste time and storage. The project implements lightweight block snapshots in user space.

## 1:00–3:00 — Architecture
Explain the logical map, immutable records, snapshot maps, CRC-32, and atomic metadata commit.

## 3:00–6:00 — Demo
Create a store, write stable data, snapshot, modify, read, roll back, read restored data, and verify.

## 6:00–7:30 — Linux driver
Explain `/dev/snapmon`, `file_operations`, `ioctl`, synchronization, and graceful operation when the module is absent.

## 7:30–9:00 — Reliability and trade-offs
Discuss ordering with `fsync`, atomic `rename`, unreachable records after crashes, snapshot-map cost, and the need for compaction.

## 9:00–10:00 — Future work
Concurrent writer locks, garbage collection, range I/O, compression, encryption, and fault injection.
