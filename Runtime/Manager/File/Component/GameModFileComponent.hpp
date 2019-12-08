#pragma once

#include "../Component/FileComponent.hpp"

#include <vector>
#include <string>

namespace Minesweeper {

	struct GameMod {
		std::string mDescription;
		std::string mAuthor;
		std::string mName;
		std::string mPath;

		bool legal() const noexcept;
	};
	
	class GameModFileComponent : public FileComponent {
	public:
		GameModFileComponent();

		void load(const std::string& fileName) override;

		void save(const std::string& fileName) override;

		auto mods() const noexcept -> const std::vector<GameMod>&;
	private:
		std::vector<GameMod> mGameMods;
	};
	
}