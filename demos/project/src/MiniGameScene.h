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
    std::unique_ptr<AffineSprite> player;

    std::unique_ptr<Sprite> projectile;
    std::vector<std::unique_ptr<Sprite>> projectiles = {};

    CharSelectScene::CharSelection character;

    int projectileCooldown = 3;
    int lives;
    int score = 0;
    int pos = 1;
    bool pressedUp, pressedDown, pressedA;
    bool dead = false;
    bool won = false;

    int savedScore;
    int savedPosX;
    int savedPosY;

    std::unique_ptr<SpriteBuilder<Sprite>> spriteBuilder;

public:
    MiniGameScene(const std::shared_ptr<GBAEngine> &engine);

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;

    void setCharacter(CharSelectScene::CharSelection character);

    void setData(int i, int i1, int i2, int i3);

    void youDied();

    void removeProjectiles();

    std::unique_ptr<Sprite> createProjectile();

    void youWon();
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MINIGAMESCENE_H
