#pragma once

#include <cstdint>
#include <filesystem>

namespace Settings {
    inline constexpr float kDefaultRatio = 1.0f;
    inline constexpr float kMinimumRatio = 0.1f;
    inline constexpr float kMaximumRatio = 1.0f;

    struct Resolution {
        std::uint32_t width;
        std::uint32_t height;
    };

    float ClampRatio(float ratio);
    Resolution ScaleResolution(std::uint32_t width, std::uint32_t height, float ratio);
    std::filesystem::path SelectDisplayTweaksINI(
        const std::filesystem::path& customPath,
        const std::filesystem::path& defaultPath);
    bool UpdateDisplayTweaksINI(const std::filesystem::path& filepath, Resolution resolution);
}
