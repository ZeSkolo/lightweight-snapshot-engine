#include "snapstore.h"
uint32_t snap_crc32(const void *data, size_t len) {
    const unsigned char *p = data; uint32_t crc = 0xFFFFFFFFu;
    for (size_t i=0;i<len;i++) { crc ^= p[i]; for (int j=0;j<8;j++) crc=(crc>>1)^((0u-(crc&1u))&0xEDB88320u); }
    return ~crc;
}
