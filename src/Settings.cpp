#include "Settings.h"
#include "Utils.h"
#include "SimpleIni.h"


void Settings::GetINISettings() {
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
    Settings::ratio = std::min(std::max(new_ratio, ratio_min), ratio_max);
    logger::info("fRatio: {}", Settings::ratio);

    // write the value back to the INI file
    ini.SetValue("Settings", "fRatio", std::to_string(Settings::ratio).c_str());

    ini.SaveFile(std::format("Data/SKSE/Plugins/{}.ini", Utilities::mod_name).c_str());

    logger::info("INI file updated.");
}
void Settings::ReadWriteDisplayTweaksINI()
{   
	if (std::filesystem::exists(Utilities::display_tweaks_custom_ini)) {
		logger::info("SSEDisplayTweaks_custom.ini exists.");
		return ReadWriteDisplayTweaksINI(Utilities::display_tweaks_custom_ini.c_str());
	}

    if (std::filesystem::exists(Utilities::display_tweaks_ini)) {
		logger::info("SSEDisplayTweaks.ini exists.");
		return ReadWriteDisplayTweaksINI(Utilities::display_tweaks_ini.c_str());
	}
	logger::info("SSEDisplayTweaks.ini does not exist.");
	return;
};
void Settings::ReadWriteDisplayTweaksINI(const char* filepath) {
    // first make sure the INI file exists
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(filepath);
    
    // Read the current display resolution in physical pixels, independent of DPI scaling.
    DEVMODEW displayMode{};
    displayMode.dmSize = sizeof(displayMode);
    if (!EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &displayMode) ||
        displayMode.dmPelsWidth == 0 || displayMode.dmPelsHeight == 0) {
        logger::error("Failed to retrieve the current display resolution; INI unchanged.");
        return;
    }

    auto displayWidth = displayMode.dmPelsWidth;
    auto displayHeight = displayMode.dmPelsHeight;
    logger::info("Display resolution: {}x{}", displayWidth, displayHeight);
    logger::info("Ratio: {}", Settings::ratio);

    // apply the ratio to the display resolution
    displayWidth = static_cast<DWORD>(displayWidth * Settings::ratio);
    displayHeight = static_cast<DWORD>(displayHeight * Settings::ratio);

    const auto windows_resolution = fmt::format("{}x{}", displayWidth, displayHeight);
    
    auto resolutions = ini.GetValue("Render", "Resolution", windows_resolution.c_str());
    logger::info("Resolution: {}", resolutions);
    // set the Display Tweaks resolution to the user's display resolution
    ini.SetValue("Render", "Resolution", windows_resolution.c_str());
    ini.SaveFile(filepath);
};
