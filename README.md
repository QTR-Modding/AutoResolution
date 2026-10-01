# AutoResolution

Fork of [QTR-Modding's AutoResolution](https://github.com/QTR-Modding/AutoResolution) that replaces [GetSystemMetrics](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics) with [EnumDisplaySettingsW](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-enumdisplaysettingsw). Credit to [erdtreefaithful](https://www.nexusmods.com/profile/erdtreefaithful) for identifying the issue.

[CommonlibSSE-NG](https://github.com/alandtse/CommonlibSSE-NG) is cloned into `external/alandtse/CommonlibSSE-NG`. [vcpkg](https://github.com/microsoft/vcpkg) is cloned into `external/microsoft/vcpkg`.

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
