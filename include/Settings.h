#pragma once

namespace Settings {
    void GetINISettings();
    void ReadWriteDisplayTweaksINI();
    void ReadWriteDisplayTweaksINI(const char* filepath);

	inline float ratio = 1.0f;
	inline float ratio_max = 10.0f;
	inline float ratio_min = 0.1f;
}