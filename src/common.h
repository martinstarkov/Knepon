#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"
#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Hitbox.h"
#include "Vector2D.h"
#include "TileMap.h"
#include "Level.h"

#define AIR 0
#define GRASS 1
#define DIRT 2