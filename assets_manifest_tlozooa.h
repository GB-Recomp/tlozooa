/* Minimal single-section manifest. Stewmath/oracles-disasm has a pret-style
 * asset layout (gfx/, audio/, rooms/, text/, ...), so a future pass can split
 * this into per-section assets via a derived PC-range map. For now the whole
 * ROM is staged as one opaque blob, same pattern as GB-Recomp/gbcamera. */
#ifndef TLOZOOA_ASSETS_MANIFEST_H
#define TLOZOOA_ASSETS_MANIFEST_H

#include <stddef.h>
#include <stdint.h>

#ifndef PG1_ASSET_ENTRY_DEFINED
#define PG1_ASSET_ENTRY_DEFINED
typedef struct {
    uint32_t rom_offset;
    uint32_t size;
    const char* path;
} Pg1AssetEntry;
#endif

#define TLOZOOA_ASSETS_MANIFEST_COUNT 1

static const Pg1AssetEntry TLOZOOA_ASSETS_MANIFEST[TLOZOOA_ASSETS_MANIFEST_COUNT] = {
    { 0x00000000, 0x00100000, "rom.bin" },
};

#endif
