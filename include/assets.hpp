#pragma once

#include <raylib.h>
#include <map>
#include <string>

struct AssetManager {
	void load();
	void unload();

	bool loaded = false;

	/*
	 * @filename: name of file excluding directory path
	 *
	 * return: texture of specified file name
	 */
	Texture2D getTexture(const std::string &filename) const
	{
		return textures.at(filename);
	}
	/*
	 * @filename: name of file excluding directory path
	 */
	const Music &getMusic(const std::string &filename) const
	{
		return music.at(filename);
	}

	void playSound(const std::string &filename, float volume = 1)
	{
		Sound &snd = sounds.at(filename);
		SetSoundVolume(snd, volume);
		PlaySound(snd);
	}

	std::map<std::string, Texture2D> textures;
	std::map<std::string, Music> music;
	std::map<std::string, Sound> sounds;
};

extern AssetManager asset_manager;
