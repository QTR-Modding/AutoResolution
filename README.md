#### BUILDING

1. Clone this repository with `git clone --recurse-submodules https://github.com/QTR-Modding/AutoResolution.git`. For an existing clone, run `git submodule update --init --recursive`.
2. Set **`VCPKG_ROOT`** to your [vcpkg](https://github.com/microsoft/vcpkg) directory.
3. From an x64 Visual Studio developer shell, run `cmake --preset release`, then `cmake --build build/release --parallel`.
4. (Optional) Set **`SKYRIM_MODS_FOLDER`** to your mods directory to copy the built plugin there.

[CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG/tree/v10.1.0), formerly CommonLibVR, is pinned to v10.1.0 (`39f9d07a6ffabea8fb559eee87ab7d27cd463e8a`) in `extern/CommonLibSSE`. A separate local CommonLib clone is not required.

#### THINGS TO EDIT

1. In LICENSE:
- **`YEAR`**
- **`YOURNAME`**
2. CMakeLists.txt
- **`AUTHORNAME`**
- **`MDDNAME`**
- (optional) Your plugin version. Default: `0.1.0.0`
3. vcpkg.json
- **`name`**: Your plugin's name.
- **`version-string`**: Your plugin version. Default: `0.1`
