# Auto Resolution

Sets SSE Display Tweaks' `[Render] Resolution` to the primary display's current
resolution in physical pixels, including when Windows display scaling is enabled.
Uses `SSEDisplayTweaks_custom.ini` when present, otherwise `SSEDisplayTweaks.ini`.

To scale the resolution, set `Data/SKSE/Plugins/PreSSEDisplayTweaks.ini` to:

```ini
[Settings]
fRatio=1.0
```

`1.0` uses the full display resolution, `0.5` halves both dimensions, and `2.0`
doubles both dimensions. Values are limited to `0.1` through `10.0`. The plugin
creates this file with `fRatio=1.0` if it is missing.

## Building

1. Clone with `git clone --recurse-submodules https://github.com/QTR-Modding/AutoResolution.git`.
2. Set `VCPKG_ROOT` to your [vcpkg](https://github.com/microsoft/vcpkg) directory.
3. From an x64 Visual Studio developer shell with C++23 support, the Windows SDK,
   CMake and Ninja, run `cmake --preset release`, then `cmake --build build/release --parallel`.
4. To stage the DLL, default INI and required license/source notices, run
   `cmake --install build/release --component AutoResolution --prefix build/package`.

For an existing clone, run `git submodule update --init --recursive` before building.
[CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG/tree/v10.1.0), formerly
CommonLibVR, is pinned to v10.1.0 (`39f9d07a6ffabea8fb559eee87ab7d27cd463e8a`) in
`extern/CommonLibSSE`. A separate local CommonLib clone is not required. vcpkg
dependencies are pinned by the manifest baseline. CommonLib fetches hde64 from
MinHook v1.3.4 during configuration.

Setting `SKYRIM_MODS_FOLDER` copies the built DLL into that mods directory.

## License

[GPL-3.0-or-later](LICENSE) with the [modding and linking exceptions](EXCEPTIONS.md),
as used by the pinned CommonLib. See [NOTICE](NOTICE) for attribution and the
preserved original MIT notice.

Distribute the staged `licenses/AutoResolution` directory with the DLL. It includes
the exact plugin source revision, dependency source links, and rebuilding
instructions in `SOURCE.txt`. Include that source link beside the binary download
when publishing on Nexus. GitHub's source ZIPs omit submodules; use the recursive
Git checkout described in `SOURCE.txt` to obtain them.
