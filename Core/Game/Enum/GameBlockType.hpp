#pragma once

namespace Minesweeper {

	enum class GameBlockType : unsigned {
		e0 = 0,
		e1 = 1,
		e2 = 2,
		e3 = 3,
		e4 = 4,
		e5 = 5,
		e6 = 6,
		e7 = 7,
		e8 = 8,
		eMine = 9
	};

	inline auto enumConvert(const GameBlockType& type) -> size_t {
		return static_cast<size_t>(type);
	}

	inline auto toGameBlockType(const size_t& value) -> GameBlockType {
		return static_cast<GameBlockType>(value);
	}
}