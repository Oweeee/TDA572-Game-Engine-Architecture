#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"

#include <set>
#include <deque>


const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;

const unsigned int  SCREEN_WIDTH = 768;
const unsigned int  SCREEN_HEIGHT = 768;

const unsigned int  SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;

const float         PLAYER_SPEED = 400.f;
const float         ROCKET_SPEED = 1200.f;
const float         SPIDER_SPEED = 100.f;
const float         FLEA_SPEED   = 200.f;

const float         SIDEKICK_SPEED = 100.f;

const double        PLAYER_INIT_X = SCREEN_WIDTH / 2;
const double        PLAYER_INIT_Y = SCREEN_HEIGHT - 32;


float game_speed = 1.f;

#include "component.h"
#include "game_object.h"

#include "player.h"
#include "sidekick.h"
#include "centipede.h"
//#include "mushroom.h"
#include "spider.h"
#include "flea.h"

#include "game.h"

int main(int argc, char** argv)
{
    AvancezLib system;

    system.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    Game game;
    game.Create(&system);
    game.Init();

    float lastTime = system.getElapsedTime();
    while(system.update())
    {
        float newTime = system.getElapsedTime();
        float dt = newTime - lastTime;
        dt = dt*game_speed;
        lastTime = newTime;

        game.Update(dt);

        game.Draw();
    }

    game.Destroy();
    system.destroy();

    return 0;
}
