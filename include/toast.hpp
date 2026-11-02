#pragma once

#include <vector>
#include <string>
#include <format>

constexpr float TOAST_DURATION = 5;

struct ToastNotification {
	std::string text;
	float time_left = TOAST_DURATION;
};

struct ToastManager {
	std::vector<ToastNotification> notifications;

	void update(float dt);

	template<typename ...Args>
	void notify(std::format_string<Args...> fmt, Args &&...args)
	{
		ToastNotification no{
			.text = std::format(fmt, std::forward<Args>(args)...)
		};
		notifications.push_back(no);
	}

	void drawOverlay() const;
};

extern ToastManager toast_manager;
