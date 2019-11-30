#pragma once

#include "../Interface/Noncopyable.hpp"

namespace Minesweeper {

	class GameContext : public Noncopyable {
	public:
		GameContext() = default;

		~GameContext() = default;
	};
	
}