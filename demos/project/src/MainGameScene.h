//
// Created by xtofh on 09/08/2020.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/scene.h>
#include "CharSelectScene.h"

class MainGameScene : public Scene {
private:
    std::unique_ptr<Background> shroomCity;
    std::unique_ptr<AffineSprite> player;
    CharSelectScene::CharSelection character;

    enum Direction {up, down, left, right};
    Direction direction = right;

    bool pressingA, pressingB, pressingUp, pressingDown, pressingRight, pressingLeft;
    bool alive;
    bool idle;

    int lives = 3;
    int score = 0;

    //1 = nothing, 2 = skip turn, 3 = roll again, 4 = minigame
    int featureMap [7][9] =   {{ 0, 1, 1, 3, 4, 1, 0, 0, 0},
                               { 0, 2, 0, 0, 0, 1, 3, 1, 2},
                               { 0, 2, 0, 0, 0, 0, 0, 0, 1},
                               { 1, 1, 2, 0, 0, 0, 0, 4, 1},
                               { 0, 0, 1, 0, 0, 0, 0, 1, 0},
                               { 0, 0, 1, 0, 0, 0, 0, 1, 0},
                               { 0, 0, 1, 4, 3, 3, 1, 2, 0}};

    int posX = 0;
    int posY = 3;


public:
    MainGameScene(const std::shared_ptr<GBAEngine> &engine);

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;

    void setCharacter(CharSelectScene::CharSelection character);
    void movePlayer(Direction);

    void startActivity();
    void setData(int lives, int score, int posX, int posY);
};


#endif //GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H
