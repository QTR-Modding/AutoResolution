#include "SettingsLogic.h"

#include <SimpleIni.h>

#include <algorithm>
#include <cmath>
#include <format>

namespace Settings {
    float ClampRatio(const float ratio)
    {
        if (!std::isfinite(ratio)) {
            return kDefaultRatio;
        }

        return std::clamp(ratio, kMinimumRatio, kMaximumRatio);
    }

    Resolution ScaleResolution(
        const std::uint32_t width,
        const std::uint32_t height,
        const float ratio)
    {
        const auto clampedRatio = ClampRatio(ratio);
        return {
            static_cast<std::uint32_t>(width * clampedRatio),
            static_cast<std::uint32_t>(height * clampedRatio)
        };
    }

    std::filesystem::path SelectDisplayTweaksINI(
        const std::filesystem::path& customPath,
        const std::filesystem::path& defaultPath)
    {
        if (std::filesystem::exists(customPath)) {
            return customPath;
        }

        if (std::filesystem::exists(defaultPath)) {
            return defaultPath;
        }

        return {};
    }

    bool UpdateDisplayTweaksINI(const std::filesystem::path& filepath, const Resolution resolution)
    {
        CSimpleIniA ini;
        ini.SetUnicode();

        const auto filepathString = filepath.string();
        if (ini.LoadFile(filepathString.c_str()) != SI_OK) {
            return false;
        }

        const auto resolutionValue = std::format("{}x{}", resolution.width, resolution.height);
        ini.SetValue("Render", "Resolution", resolutionValue.c_str());
        return ini.SaveFile(filepathString.c_str()) == SI_OK;
    }
}
