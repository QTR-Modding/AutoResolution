#include "Settings.h"

#include <SimpleIni.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <format>

namespace {
    constexpr float kDefaultRatio = 1.0f;
    constexpr float kMinimumRatio = 0.1f;
    constexpr float kMaximumRatio = 1.0f;

    struct Resolution {
        std::uint32_t width;
        std::uint32_t height;
    };

    float ratio = kDefaultRatio;

    float ClampRatio(const float value)
    {
        if (!std::isfinite(value)) {
            return kDefaultRatio;
        }

        return std::clamp(value, kMinimumRatio, kMaximumRatio);
    }

    Resolution ScaleResolution(
        const std::uint32_t width,
        const std::uint32_t height,
        const float value)
    {
        const auto clampedRatio = ClampRatio(value);
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
}

void GetINISettings() {
    // We have one section called [Settings] and just one key called "fRatio" with value 1.0

    // first make sure the INI file exists
    const bool iniExists = std::filesystem::exists(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name));
    if (!iniExists) {
        // make the INI file
        std::ofstream iniFile(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name));
        iniFile << "[Settings]\n";
        iniFile << "fRatio=1.0\n";
        iniFile.close();

        logger::info("INI file created.");

        return;
    } else {
        logger::info("INI file exists.");
    }

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name).c_str());

    const float new_ratio = static_cast<float>(ini.GetDoubleValue("Settings", "fRatio", ratio));
    ratio = ClampRatio(new_ratio);
    logger::info("fRatio: {}", ratio);

    // write the value back to the INI file
    ini.SetValue("Settings", "fRatio", std::to_string(ratio).c_str());

    ini.SaveFile(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name).c_str());

    logger::info("INI file updated.");
}

void ReadWriteDisplayTweaksINI()
{
	const auto filepath = SelectDisplayTweaksINI(
		Utilities::display_tweaks_custom_ini,
		Utilities::display_tweaks_ini);
	if (filepath.empty()) {
		logger::info("SSEDisplayTweaks.ini does not exist.");
		return;
	}

	logger::info("Using {}.", filepath.filename().string());
	return ReadWriteDisplayTweaksINI(filepath.string().c_str());
};

void ReadWriteDisplayTweaksINI(const char* filepath) {
    // first make sure the INI file exists
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile(filepath) != SI_OK) {
        logger::error("Failed to load {}; INI unchanged.", filepath);
        return;
    }

    // Get the user's actual Windows display resolution in physical pixels.
    DEVMODEW displayMode{};
    displayMode.dmSize = sizeof(displayMode);

    if (!EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &displayMode) ||
        displayMode.dmPelsWidth == 0 || displayMode.dmPelsHeight == 0) {
        logger::error("Failed to retrieve a valid current display resolution; INI unchanged.");
        return;
    }

    auto displayWidth = displayMode.dmPelsWidth;
    auto displayHeight = displayMode.dmPelsHeight;
    logger::info("Display resolution: {}x{}", displayWidth, displayHeight);
    logger::info("Ratio: {}", ratio);

    const auto resolution = ScaleResolution(displayWidth, displayHeight, ratio);

    auto resolutions = ini.GetValue("Render", "Resolution", "");
    logger::info("Resolution: {}", resolutions);

    const auto resolutionValue = std::format("{}x{}", resolution.width, resolution.height);
    if (ini.SetValue("Render", "Resolution", resolutionValue.c_str()) < 0) {
        logger::error("Failed to set resolution in {}; INI unchanged.", filepath);
        return;
    }

    if (ini.SaveFile(filepath) != SI_OK) {
        logger::error("Failed to save {}.", filepath);
    }
};
