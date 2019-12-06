#include "MinesweeperApp.hpp"

int main() {
	Minesweeper::MinesweeperApp app = Minesweeper::MinesweeperApp("Minesweeper", 1920, 1080);

	app.show();
	app.runLoop();
}