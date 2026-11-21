#pragma once

#include <vector>
#include <string>
#include <format>
#include <raylib.h>

constexpr float TOAST_DURATION = 5;

struct ToastNotification {
	std::string text;
	float x;
	float time_left = TOAST_DURATION;
};

struct ToastManager {
	std::vector<ToastNotification> notifications;
	float fontsize = 14;

	void update(float dt);

	template<typename ...Args>
	void notify(std::format_string<Args...> fmt, Args &&...args)
	{
		std::string s = std::format(fmt, std::forward<Args>(args)...);
		float len = MeasureText(s.c_str(), fontsize);

		ToastNotification no = {
			.text = s,
			.x = GetScreenWidth() - (10 + len),
		};
		notifications.push_back(no);
	}

	void drawOverlay() const;
};

extern ToastManager toast_manager;
