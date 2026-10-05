# Lightweight User-Space Snapshot & Rollback Storage Engine

A Linux-only storage-engine capstone implemented in **C**. It provides named snapshots and rollback over a fixed logical block space using append-only, block-level copy-on-write (CoW). An optional Linux character driver records write, snapshot, and rollback events.

## Features

- Fixed 4 KiB logical blocks
- Append-only copy-on-write records
- Named snapshots without copying payload data
- Atomic metadata replacement using `fsync()` + `rename()`
- CRC-32 integrity verification
- Persistent state across process restarts
- Optional `/dev/snapmon` Linux character device using `ioctl`
- Automated end-to-end test and reproducible demo

## Architecture

```text
CLI commands
    │
    ▼
Snapshot engine ── atomic metadata.bin (current map + snapshot maps)
    │
    ├── append-only blocks.dat (record header + payload + CRC-32)
    │
    └── optional ioctl notifications ──> /dev/snapmon kernel module
```

A logical block points to the byte offset of its latest immutable physical record. A snapshot copies only the small offset map. New writes append a record and update the current map; rollback replaces the current map with a saved map. Existing records remain unchanged.

## Requirements

- Linux
- GCC or Clang with C11 support
- GNU Make
- Optional driver: matching Linux kernel headers and root access to load modules

## Build and test

```bash
make
make test
```

## Quick demo

```bash
make demo
```

## CLI

```text
snapstore init STORE BLOCKS
snapstore write STORE BLOCK INPUT
snapstore read STORE BLOCK OUTPUT
snapstore snapshot STORE NAME
snapstore list STORE
snapstore rollback STORE NAME
snapstore delete-snapshot STORE NAME
snapstore stats STORE
snapstore verify STORE
```

Example:

```bash
./bin/snapstore init my-store 1024
printf 'original\n' > original.txt
./bin/snapstore write my-store 0 original.txt
./bin/snapstore snapshot my-store before-change
printf 'changed\n' > changed.txt
./bin/snapstore write my-store 0 changed.txt
./bin/snapstore rollback my-store before-change
./bin/snapstore read my-store 0 restored.txt
cat restored.txt
```

## Optional Linux device driver

The `driver/snapmon.c` module demonstrates character-device registration, `file_operations`, user/kernel copying, mutex synchronization, and an `ioctl` interface.

```bash
sudo apt install build-essential linux-headers-$(uname -r)
make driver
sudo insmod driver/snapmon.ko
cat /dev/snapmon
make demo
cat /dev/snapmon
sudo rmmod snapmon
```

If the driver is not loaded, the user-space engine continues normally.

## Storage format

- `metadata.bin`: versioned header, current logical-to-physical offset map, snapshot descriptors and saved maps.
- `blocks.dat`: immutable variable-length records containing logical block number, payload length, CRC-32, and payload.
- `metadata.tmp`: temporary transaction file; committed through atomic rename.

## Crash consistency

1. Append the new immutable record.
2. `fsync()` the data file.
3. Write complete metadata to `metadata.tmp`.
4. `fsync()` temporary metadata.
5. Atomically rename it to `metadata.bin`.
6. `fsync()` the store directory.

A crash may leave an unreachable appended record, but it cannot expose partially committed metadata.

## Limitations and future work

- Maximum 32 snapshots and 4096-byte input per write
- No concurrent writer coordination yet
- Unreachable records are not reclaimed; add garbage collection/compaction
- Add range writes, compression, encryption, and fault-injection testing

## Safety

Use this educational engine only with disposable test data. The kernel module is optional and should be built and loaded only on a development Linux machine or VM.

## Documentation

- [`docs/architecture.md`](docs/architecture.md)
- [`docs/demonstration.md`](docs/demonstration.md)
- [`docs/evaluation-notes.md`](docs/evaluation-notes.md)
