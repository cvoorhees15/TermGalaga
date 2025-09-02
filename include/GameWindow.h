#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "Entities/Player.h"
// =============================
// Game window management class
// =============================
class GameWindow {
public:
  GameWindow();

  ~GameWindow();

  void run();

protected:
  Player activePlayer;

private:
  void handleInput();

  void drawBorder();

  bool _isRunning;
  int _screenHeight;
  int _screenWidth;
  int _initialPlayerX;
  int _initialPlayerY;
};
