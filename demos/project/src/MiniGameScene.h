//
// Created by xtofh on 13/08/2020.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MINIGAMESCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MINIGAMESCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/scene.h>
#include "CharSelectScene.h"

class MiniGameScene : public Scene {
private:
    std::unique_ptr<Background> miniGameBg;
    std::unique_ptr<Sprite> player;

    std::unique_ptr<AffineSprite> projectile1;
    std::unique_ptr<AffineSprite> projectile2;

    CharSelectScene::CharSelection character;

    int lives;
    int score;
    int pos;
    bool pressedUp, pressedDown, pressedA;
    bool dead;

    int savedScore;
    int savedPosX;
    int savedPosY;

public:
    MiniGameScene(const std::shared_ptr<GBAEngine> &engine);

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;

    void setCharacter(CharSelectScene::CharSelection character);

    void setData(int i, int i1, int i2, int i3);

    void youDied();
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MINIGAMESCENE_H
