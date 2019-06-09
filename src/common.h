#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "TileMap.h"
#include "Level.h"
#include "GameWorld.h"

#define PLAYERSPAWN 1000
#define AIR 0
#define GRASS 1
#define DIRT 2
#define STONE 3
#define DOOR 7
#define SNOW 9
#define PUMPKIN 10