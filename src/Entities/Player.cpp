#include "../../include/Entities/Player.h"
#include <ncurses.h>

Player::Player() {
  // initialize player position
  int width, height;
  getmaxyx(stdscr, height, width); // get screen size

  this->_xPos = width / 2;
  this->_yPos = height / 2;
}

Player::~Player() {}

// ================================================
// Desc: Move player ship to new spot in the window
// Input: int x coordinate, int y coordinate
// Output: bool success/fail
// ================================================
bool Player::updatePlayerPosition(int x, int y) {
  this->modifyPosition(x, y);

  attron(COLOR_PAIR(2)); // Green color

  // Row 1: Nose
  mvaddch(this->_yPos - 1, this->_xPos, shipParts.nose);

  // Row 2: Cockpit
  mvaddch(this->_yPos, this->_xPos - 1, shipParts.wall);
  mvaddch(this->_yPos, this->_xPos, shipParts.cockpit);
  mvaddch(this->_yPos, this->_xPos + 1, shipParts.wall);

  // Row 3: Base with wings
  mvaddch(this->_yPos + 1, this->_xPos - 2, shipParts.leftWing);
  mvaddch(this->_yPos + 1, this->_xPos - 1, shipParts.wall);
  mvaddch(this->_yPos + 1, this->_xPos, shipParts.base);
  mvaddch(this->_yPos + 1, this->_xPos + 1, shipParts.wall);
  mvaddch(this->_yPos + 1, this->_xPos + 2, shipParts.rightWing);

  attroff(COLOR_PAIR(2));

  mvprintw(0, 10, "%d, %d", _xPos, _yPos);

  return true;
}

// ================================================
// Desc: Delete the player ship from the window
// ================================================
bool Player::erasePlayer() {
  // Clear all positions where the ship was drawn
  mvaddch(this->_yPos - 1, this->_xPos, ' ');     // Nose
  mvaddch(this->_yPos, this->_xPos - 1, ' ');     // Cockpit left
  mvaddch(this->_yPos, this->_xPos, ' ');         // Cockpit center
  mvaddch(this->_yPos, this->_xPos + 1, ' ');     // Cockpit right
  mvaddch(this->_yPos + 1, this->_xPos - 2, ' '); // Left wing
  mvaddch(this->_yPos + 1, this->_xPos - 1, ' '); // Left body
  mvaddch(this->_yPos + 1, this->_xPos, ' ');     // Bottom
  mvaddch(this->_yPos + 1, this->_xPos + 1, ' '); // Right body
  mvaddch(this->_yPos + 1, this->_xPos + 2, ' '); // Right wing

  return true;
}

// ===================================================
// Desc: Manage the lifetime of the active projectiles
// ===================================================
void Player::managePlayerProjectiles() {
  // for each active projectile
  for (auto &proj : this->_activeProjectiles) {
    proj.frameCt++;

    // if the right amount of frames have passed
    if (proj.frameCt > 3) {
      attron(COLOR_PAIR(1));

      // erase the current projectile
      mvaddch(proj.y, proj.x, ' ');

      // move the projectile
      proj.y--;

      // if the projectile has not reached the border
      if (proj.y > 1) {
        // redraw the projectile at the new position
        mvaddch(proj.y, proj.x, this->_missle);
        attroff(COLOR_PAIR(1));
      }
      // if the projectile has reached the edge
      else {
        // set the projectile inactive
        proj.active = false;
      }
    }
  }

  // erase any inactive projectiles
  this->_activeProjectiles.erase(
      std::remove_if(this->_activeProjectiles.begin(),
                     this->_activeProjectiles.end(),
                     [](const Projectile p) { return !p.active; }),
      this->_activeProjectiles.end());
}

// =========================================================
// Desc: Add a projectile to the list of active projectiles
// =========================================================
void Player::firePlayerProjectile() {
  this->_activeProjectiles.push_back(
      Projectile(this->_xPos, this->_yPos, 0, true));
}
