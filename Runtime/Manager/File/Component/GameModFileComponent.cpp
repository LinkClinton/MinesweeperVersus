#include "GameModFileComponent.hpp"

#include "../../Extensions/Json/Json.hpp"

#include <filesystem>
#include <fstream>
#include <cassert>

bool Minesweeper::GameMod::legal() const noexcept
{
	return
		!mDescription.empty() && 
		!mAuthor.empty() && 
		!mName.empty() && 
		!mPath.empty();
}

Minesweeper::GameModFileComponent::GameModFileComponent() :
	FileComponent("Resources/Mods/", FileComponentMask::eLoad)
{
	
}

void Minesweeper::GameModFileComponent::load(const std::string& fileName)
{
	assert(fileName == "Resources/Mods/");
	
	if (!exists(std::filesystem::path(fileName))) 
		std::filesystem::create_directory(fileName);

	for (const auto& directory : std::filesystem::directory_iterator(fileName)) {
		if (!is_directory(directory)) continue;

		GameMod gameMod;
		
		for (const auto& file : std::filesystem::directory_iterator(directory)) {
			if (file.is_directory() || !file.path().has_extension()) continue;

			//when we get description file
			if (file.path().filename() == "description.json") {
				auto stream = std::ifstream(file);
				auto description = Json();

				stream >> description;

				gameMod.mDescription = description["description"];
				gameMod.mAuthor = description["author"];
				gameMod.mName = description["name"];
			}

			//when we get dll file
			if (file.path().filename() == "generator.dll") {
				gameMod.mPath = file.path().generic_string();
			}

			if (gameMod.legal()) mGameMods.push_back(gameMod);
		}
	}
}

void Minesweeper::GameModFileComponent::save(const std::string& fileName)
{
	
}

auto Minesweeper::GameModFileComponent::mods() const noexcept -> const std::vector<GameMod>& 
{
	return mGameMods;
}
