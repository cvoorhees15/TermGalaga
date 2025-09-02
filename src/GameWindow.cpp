#include "../include/GameWindow.h"

GameWindow::GameWindow() {
  // initialize ncurses
  initscr();
  cbreak();              // Disable line buffering
  noecho();              // Don't echo pressed keys
  curs_set(0);           // Hide cursor
  keypad(stdscr, TRUE);  // Enable special keys
  nodelay(stdscr, TRUE); // Non-blocking input

  // initialize game state
  getmaxyx(stdscr, this->_screenHeight, this->_screenWidth); // get screen size

  this->_isRunning = true;
  this->_initialPlayerX = this->_screenWidth / 2;
  this->_initialPlayerY = this->_screenHeight / 2;

  // draw a visible border around the window
  drawBorder();

  // initialize player
  this->activePlayer = Player();
}

GameWindow::~GameWindow() {
  // cleanup ncurses
  endwin();
}

// ==================================
// Desc: Initiate main gameplay loop
// ==================================
void GameWindow::run() {
  // main gameplay loop
  while (this->_isRunning) {
    handleInput();
    activePlayer.managePlayerProjectiles();
    refresh();
    usleep(50000);
  }
}
// ================================================
// Desc: Handle player keyboard input and controls
// ================================================
void GameWindow::handleInput() {
  // get key input
  int ch = getch();
  int tempX = 0, tempY = 0;
  // update player position
  switch (ch) {
  case KEY_UP:
    activePlayer.erasePlayer();
    mvprintw(0, 0, "UP");
    tempY = -5;
    break;
  case KEY_DOWN:
    activePlayer.erasePlayer();
    mvprintw(0, 0, "DOWN");
    tempY = 5;
    break;
  case KEY_LEFT:
    activePlayer.erasePlayer();
    mvprintw(0, 0, "LEFT");
    tempX = -5;
    break;
  case KEY_RIGHT:
    activePlayer.erasePlayer();
    mvprintw(0, 0, "RIGHT");
    tempX = 5;
    break;
  case 32: // SPACE BAR
  {
    activePlayer.firePlayerProjectile();
    break;
  }
  case 'q':
  case 'Q':
    this->_isRunning = false;
    break;
  }

  activePlayer.updatePlayerPosition(tempX, tempY);
}

// =============================================================================================
// Desc: Draw a dynamically sized border around the players screen using '-' and
// '|' characters
// =============================================================================================
void GameWindow::drawBorder() {
  // Draw border
  attron(COLOR_PAIR(4));
  for (int x = 0; x < this->_screenWidth; x++) {
    mvaddch(0, x, '-');
    mvaddch(this->_screenHeight - 1, x, '-');
  }
  for (int y = 0; y < this->_screenHeight; y++) {
    mvaddch(y, 0, '|');
    mvaddch(y, this->_screenWidth - 1, '|');
  }
  attroff(COLOR_PAIR(4));
}
