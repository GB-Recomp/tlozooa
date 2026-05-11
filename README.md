# tlozooa

Static recompilation of **The Legend of Zelda: Oracle of Ages** (USA,
Australia) into portable C, built with
[GB-Recomp/gb-recompiled](https://github.com/GB-Recomp/gb-recompiled).

Symbol names come from the [Stewmath/oracles-disasm](https://github.com/Stewmath/oracles-disasm)
WLA-DX disassembly — labels like `tlozooa__sym_gfxRegisterStates` map back
to the same names you'd see in the decomp.

## Build

```sh
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
```

Produces a `tlozooa` executable (~560 KB with the default MinSizeRel +
dead-strip profile).

## Run

Drop your Oracle of Ages ROM next to the executable as `roms/tlozooa.gbc`:

```sh
mkdir -p roms
cp '/path/to/Legend of Zelda, The - Oracle of Ages (USA, Australia).gbc' roms/tlozooa.gbc
./tlozooa
```

First boot extracts the ROM into `assets/tlozooa/rom.bin`; the source ROM
isn't needed after that. Press Esc for the runtime menu (savestates,
border, audio, etc.).

## Notes

- The asset manifest is currently a single opaque section — the whole 1
  MiB ROM is staged as `rom.bin`. A future pass can split this into the
  decomp's `gfx/`, `audio/`, `rooms/`, `text/` regions for moddable
  assets.
- The bundled companion game (Oracle of Seasons) lives at
  [GB-Recomp/tlozoos](https://github.com/GB-Recomp/tlozoos).
