#ifndef SNAPSTORE_H
#define SNAPSTORE_H
#include <stdint.h>
#include <stddef.h>
#define SNAPSTORE_MAGIC 0x53534E50u
#define SNAPSTORE_VERSION 1u
#define SNAPSTORE_BLOCK_SIZE 4096u
#define SNAPSTORE_NAME_MAX 64u
#define SNAPSTORE_MAX_SNAPSHOTS 32u
#define SNAPSTORE_UNMAPPED UINT64_MAX
#define SNAPSTORE_RECORD_MAGIC 0x424C4B31u
typedef struct { uint32_t magic, version, block_size, logical_blocks, snapshot_count, reserved; uint64_t generation; } meta_header_t;
typedef struct { char name[SNAPSTORE_NAME_MAX]; int64_t created_at; } snapshot_header_t;
typedef struct { uint32_t magic, logical_block, data_len, crc32; } record_header_t;
uint32_t snap_crc32(const void *data, size_t len);
#endif
