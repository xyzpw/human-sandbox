#include "assets.hpp"
#include <raylib.h>
#include <filesystem>

AssetManager asset_manager{};

void AssetManager::load()
{
	namespace fs = std::filesystem;

	const std::string root = "assets/";
	auto iter = fs::recursive_directory_iterator(root);
	for (auto &it : iter) {
		if (!it.is_regular_file())
			continue;

		fs::path rel = fs::relative(it.path(), root);
		std::string file = it.path().string();
		std::string key = rel.filename().string();

		auto rel_first = rel.begin();
		if (*rel_first == "music") {
			music[key] = LoadMusicStream(file.c_str());
		} else if (*rel_first == "sound") {
			sounds[key] = LoadSound(file.c_str());
		} else if (rel.parent_path().empty()) {
			textures[key] = LoadTexture(file.c_str());
		}
	}

	loaded = true;
}

void AssetManager::unload()
{
	for (auto &[name, tex] : textures) {
		UnloadTexture(tex);
	}
	textures.clear();
	for (auto &[name, mu] : music) {
		UnloadMusicStream(mu);
	}
	music.clear();
	for (auto &[name, snd] : sounds) {
		UnloadSound(snd);
	}
	sounds.clear();

	loaded = false;
}
