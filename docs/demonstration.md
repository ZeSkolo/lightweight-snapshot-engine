# Demonstration Script

1. Run `make clean && make && make test`.
2. Run `make demo`.
3. Explain that `stable-v1` retains the first offset map.
4. Point out that the experimental write appends data instead of overwriting it.
5. Show rollback restoring `CONFIG: stable-release`.
6. Run `./bin/snapstore verify build/demo-store` to demonstrate CRC validation.
7. Optionally load `snapmon.ko` and show counters with `cat /dev/snapmon`.

## Expected conclusion

Rollback changes metadata pointers rather than copying the old data back. This makes snapshots lightweight while immutable records and checksums improve safety.
