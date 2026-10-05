# Architecture

## Components

1. **CLI and validation** — Parses commands and rejects invalid block indices and snapshot names.
2. **Metadata manager** — Loads and atomically commits the current map and snapshot maps.
3. **Append-only block store** — Persists immutable block records in `blocks.dat`.
4. **Integrity layer** — Calculates and verifies CRC-32 for every payload.
5. **Snapshot manager** — Copies logical offset maps, not payload blocks.
6. **Kernel monitor (optional)** — A character driver counts engine events sent using `ioctl`.

## Copy-on-write example

Before a write, logical block 7 points to physical record A. Snapshot `stable-v1` saves that mapping. A new write appends record B and changes only the current mapping to B. Rollback restores the mapping to A; neither record is overwritten.

## Complexity

| Operation | Time | Additional metadata |
|---|---:|---:|
| Read block | O(1) | O(1) |
| Write block | O(block size) | One appended record |
| Create snapshot | O(logical blocks) | One offset map |
| Rollback | O(logical blocks) | No payload copy |
| Verify | O(mapped references) | O(block size) buffer |

## Linux concepts used

POSIX file descriptors, `pread`, append-only writes, `fsync`, atomic `rename`, directory synchronization, permissions, kernel character devices, `file_operations`, mutexes, `copy_from_user`, and `ioctl`.
