#include "Settings.h"

#include <SimpleIni.h>

#include <format>

namespace Settings {
    inline float ratio = kDefaultRatio;
};

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

    const float new_ratio = static_cast<float>(ini.GetDoubleValue("Settings", "fRatio", Settings::ratio));
    Settings::ratio = Settings::ClampRatio(new_ratio);
    logger::info("fRatio: {}", Settings::ratio);

    // write the value back to the INI file
    ini.SetValue("Settings", "fRatio", std::to_string(Settings::ratio).c_str());

    ini.SaveFile(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name).c_str());

    logger::info("INI file updated.");
}
void ReadWriteDisplayTweaksINI()
{
	const auto filepath = Settings::SelectDisplayTweaksINI(
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
    logger::info("Ratio: {}", Settings::ratio);

    const auto resolution = Settings::ScaleResolution(displayWidth, displayHeight, Settings::ratio);

    auto resolutions = ini.GetValue("Render", "Resolution", "");
    logger::info("Resolution: {}", resolutions);

    if (!Settings::UpdateDisplayTweaksINI(filepath, resolution)) {
        logger::error("Failed to update {}.", filepath);
    }
};
