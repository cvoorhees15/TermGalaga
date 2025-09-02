#include "Entity.h"
#include <ncurses.h>

// ========================
// Player entity class
// ========================
class Player : Entity {
public:
  Player();

  ~Player();

  bool updatePlayerPosition(int x, int y);

  bool erasePlayer();

  void managePlayerProjectiles();

  void firePlayerProjectile();

private:
  // ship defaults
  //   ^
  //  |*|
  // <|_|>
  struct Ship {
    char nose = '^';
    char wall = '|';
    char cockpit = '*';
    char leftWing = '<';
    char rightWing = '>';
    char base = '_';
  };

  Ship shipParts;
};
