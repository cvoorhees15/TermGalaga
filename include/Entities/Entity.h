#include <utility>
#include <vector>

// ================================
// Base class for all game entities
// ================================
class Entity {
public:
  Entity();

  ~Entity();

  std::pair<int, int> getPosition();

  void modifyPosition(int x, int y);

  void setLife(bool isAlive);

  bool getLifeStatus();

protected:
  struct Projectile {
    int x, y, frameCt;
    bool active;

    Projectile(int x, int y, int frameCt, bool active)
        : x(x), y(y), frameCt(frameCt), active(active) {}
  };

  std::vector<Projectile> _activeProjectiles;

  char _missle = '*';
  bool _isAlive;
  int _xPos;
  int _yPos;
};
