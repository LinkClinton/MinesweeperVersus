#pragma once

namespace Minesweeper {

	enum class GameBlockStatus : unsigned {
		eUnknown,
		eKnown,
		eHide,
		eFlag,
	};
	
}