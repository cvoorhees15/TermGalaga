#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>

using namespace std;

class GameWindow {
private:
	// state vars
	bool running;

	int screenWidth, screenHeight;

	// player vars
	int playerX, playerY; // position

	struct playerProjectiles { // projectiles
		int x, y, frameCt;
		bool active;
	};

	vector<playerProjectiles> activeProjectiles;

	// ship defaults
	//   ^
	//  |*|
	// <|_|>
	vector<char> shipParts = {'^', '|', '*', '|', '<', '|', '_', '|', '>'};

	char missle = '*';
public:
	GameWindow() {
	
		// initialize ncurses
        initscr();
        cbreak();				// Disable line buffering
        noecho();				// Don't echo pressed keys
        curs_set(0);			// Hide cursor
        keypad(stdscr, TRUE);	// Enable special keys
        nodelay(stdscr, TRUE);	// Non-blocking input
	
		// initialize game state
		getmaxyx(stdscr, screenHeight, screenWidth); // get screen size
		
		running = true;
		playerX = screenWidth/2;
		playerY = screenHeight/2;
		
		drawBorder();
	}

	~GameWindow() {

		// cleanup ncurses
		endwin(); 
	}

	// handle player input from the keyboard
	void handleInput() {
		// get key input
		int ch = getch();
		// update player position
		switch(ch) {
			case KEY_UP:
				erasePlayer();
				mvprintw(0, 0, "UP");
				playerY -= 5;
				break;
            case KEY_DOWN:
				erasePlayer();
				mvprintw(0, 0, "DOWN");
				playerY += 5;
				break;
            case KEY_LEFT:
				erasePlayer();
				mvprintw(0, 0, "LEFT");
				playerX -= 5;
				break;
            case KEY_RIGHT:
				erasePlayer();
				mvprintw(0, 0, "RIGHT");
				playerX += 5;
				break;
			case 32: { // SPACE BAR
				firePlayerProjectile();
				break;
			}
			case 'q':
			case 'Q':
                running = false;
            	break;
        	}
		
		updatePlayer(playerX, playerY);
	}

	// draw the player ship on screen
	void updatePlayer(int x, int y) {
		attron(COLOR_PAIR(2));  // Green color
        
        // Row 1: Top point
        mvaddch(y-1, x, shipParts[0]);
        
        // Row 2: Cockpit
        mvaddch(y, x-1, shipParts[1]);
        mvaddch(y, x,   shipParts[2]);
        mvaddch(y, x+1, shipParts[3]);
        
        // Row 3: Base with wings
        mvaddch(y+1, x-2, shipParts[4]);
        mvaddch(y+1, x-1, shipParts[5]);
        mvaddch(y+1, x,   shipParts[6]);
        mvaddch(y+1, x+1, shipParts[7]);
        mvaddch(y+1, x+2, shipParts[8]);
        
        attroff(COLOR_PAIR(2));
	}

	void erasePlayer() {
        // Clear all positions where the ship was drawn
        mvaddch(playerY-1, playerX,   ' ');  // Top point
        mvaddch(playerY,   playerX-1, ' ');  // Cockpit left
        mvaddch(playerY,   playerX,   ' ');  // Cockpit center
        mvaddch(playerY,   playerX+1, ' ');  // Cockpit right
        mvaddch(playerY+1, playerX-2, ' ');  // Left wing
        mvaddch(playerY+1, playerX-1, ' ');  // Left body
        mvaddch(playerY+1, playerX,   ' ');  // Bottom
        mvaddch(playerY+1, playerX+1, ' ');  // Right body
        mvaddch(playerY+1, playerX+2, ' ');  // Right wing
    }

	void drawBorder() {
		// Draw border
        attron(COLOR_PAIR(4));
        for (int x = 0; x < screenWidth; x++) {
            mvaddch(0, x, '-');
            mvaddch(screenHeight - 1, x, '-');
        }
        for (int y = 0; y < screenHeight; y++) {
            mvaddch(y, 0, '|');
            mvaddch(y, screenWidth - 1, '|');
        }
        attroff(COLOR_PAIR(4));
	}

	void firePlayerProjectile() {
		activeProjectiles.push_back({playerX, playerY, 0, true});
	}

	void managePlayerProjectiles() {
		// for each active projectile
		for (auto& proj : activeProjectiles){ 
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
					mvaddch(proj.y, proj.x, missle);
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
		activeProjectiles.erase(
            std::remove_if(activeProjectiles.begin(), activeProjectiles.end(),
                [](const playerProjectiles& p) { return !p.active; }),
            activeProjectiles.end()
        );
	}
	
	void run() {
		// place player ship at starting position
		updatePlayer(playerX,playerY);
		// main gameplay loop
		while(running) {
			handleInput();
			managePlayerProjectiles();
			refresh();
			usleep(50000);
		}
	}
};

	int main() {
		// start game
		GameWindow mainWindow;
		
		// main game loop
		mainWindow.run();

		return 0;
	}
    
