#pragma once

namespace Minesweeper {

	enum class GameBlockStatus : unsigned {
		eUnknown = 0,
		eKnown = 1,
		eFlag = 2,
	};
	
}