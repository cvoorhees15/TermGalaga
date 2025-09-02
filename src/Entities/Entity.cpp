#include "../../include/Entities/Entity.h"

using namespace std;

Entity::Entity() { setLife(true); }

Entity::~Entity() {}

// ====================================
// Get life status of this entity
// Output: bool alive/dead
// ====================================
bool Entity::getLifeStatus() { return this->_isAlive; }

// ====================================
// Desc: Set life status of this entity
// Input: bool alive/dead
// ====================================
void Entity::setLife(bool isAlive) { this->_isAlive = isAlive; }

// ===================================================
// Desc: Return an x, y pair for this entities position
// Output: int pair
// ===================================================
pair<int, int> Entity::getPosition() { return {this->_xPos, this->_yPos}; }

// ===========================================
// Desc: Set an x, y position for this entity
// Input: int x coordinate, int y coordinate
// ===========================================
void Entity::modifyPosition(int x, int y) {
  if (x != 0)
    this->_xPos += x;
  if (y != 0)
    this->_yPos += y;
}
