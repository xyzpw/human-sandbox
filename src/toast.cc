#include "toast.hpp"
#include <raylib.h>
#include <cmath>

ToastManager toast_manager{};

void ToastManager::update(float dt)
{
	static auto rm_pred = [](ToastNotification &a) {
		return a.time_left == 0;
	};

	for (auto &no : notifications) {
		no.time_left = std::fmax(no.time_left - dt, 0);
	}

	if (!notifications.empty())
		std::erase_if(notifications, rm_pred);
}

void ToastManager::drawOverlay() const
{
	const float fontsize = 14;

	float y = GetScreenHeight();
	int size = notifications.size();
	for (int i = size - 1; i >= 0; i--) {
		const ToastNotification &no = notifications[i];
		float x = GetScreenWidth() -
			0.66f * fontsize * no.text.length();
		y -= fontsize * 1.1f;
		DrawText(no.text.c_str(), x, y, fontsize, WHITE);
	}
}
