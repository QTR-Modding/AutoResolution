#include "Settings.h"
#include "Logger.h"

SKSEPluginLoad(const SKSE::LoadInterface *skse) {

    SetupLog();
    logger::info("Plugin loaded");
    SKSE::Init(skse);
    Settings::GetINISettings();
    Settings::ReadWriteDisplayTweaksINI();
    return true;
}