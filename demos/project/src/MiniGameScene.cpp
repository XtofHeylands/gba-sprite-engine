//
// Created by xtofh on 13/08/2020.
//
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>

#include "MiniGameScene.h"
#include "MainGameScene.h"
#include "recourses/backgrounds/miniGameScreen.h"
#include "recourses/sprites/luigi.h"
#include "recourses/sprites/mario.h"
#include "recourses/sprites/bulletBil.h"
#include "recourses/sounds/backgroundMusic.h"
#include "recourses/sounds/gameOverSound.h"


MiniGameScene::MiniGameScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> MiniGameScene::backgrounds() {
    return {miniGameBg.get()};
}

std::vector<Sprite *> MiniGameScene::sprites() {
    return {player.get(), projectile1.get(), projectile2.get()};
}

void MiniGameScene::load() {

    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(miniGameBgPal, sizeof(miniGameBgPal)));
    miniGameBg = std::unique_ptr<Background>(new Background(1, miniGameBgTiles, sizeof(miniGameBgTiles), miniGameBgMap, sizeof(miniGameBgMap), 16, 1, MAPLAYOUT_32X32));

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;

    if(character == CharSelectScene::mario_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(miniGamesharedPal, sizeof(miniGamesharedPal)));

        player = builder
                .withData(marioTankTiles, sizeof(marioTankTiles))
                .withSize(SIZE_32_32)
                .withAnimated(4, 5)
                .withLocation(50,65)
                .buildPtr();

    } else if(character == CharSelectScene::luigi_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(luigi_miniGameSharedPal, sizeof(luigi_miniGameSharedPal)));

        player = builder
                .withData(luigiTankTiles, sizeof(luigiTankTiles))
                .withSize(SIZE_32_32)
                .withAnimated(4,5)
                .withLocation(50,65)
                .buildPtr();

    } else if(character == CharSelectScene::peach_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    } else if(character == CharSelectScene::yoshi_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    }

    projectile1 = affineBuilder
            .withData(bulletBillTiles, sizeof(bulletBillTiles))
            .withSize(SIZE_32_16)
            .withAnimated(2,5)
            .withLocation(235,80)
            .buildPtr();

    projectile2 = affineBuilder
            .withData(bulletBillTiles, sizeof(bulletBillTiles))
            .withSize(SIZE_32_16)
            .withAnimated(2,5)
            .withLocation(235,120)
            .buildPtr();


    engine->enqueueMusic(backgroundMusic, sizeof(backgroundMusic), 45000);
    miniGameBg->scroll(5,0);

    projectile1->setVelocity(-1,0);
    projectile2->setVelocity(-2,0);

    pos = 1;
}

void MiniGameScene::tick(u16 keys) {

    if(dead && (keys & KEY_A)){
        if(!engine->isTransitioning()){
            auto mainGameScene = new MainGameScene(engine);
            mainGameScene->setCharacter(character);
            mainGameScene->setData(lives - 1, savedScore + score, savedPosX, savedPosY);
        }
    }

    if(dead){return;}

    TextStream::instance().setText("Evade bullet Bil!", 1, 1);
    TextStream::instance().setText("score: " + std::to_string(score), 2, 1);

    if (pressedUp && !(keys & KEY_UP)){
        if(pos > 0){
            pos = pos - 1;
            player->moveTo(player->getX(), player->getY() - 40);
        }
        pressedUp = false;
    } else if (pressedDown && !(keys & KEY_DOWN)){
        if(pos < 2){
            pos = pos + 1;
            player->moveTo(player->getX(), player->getY() + 40);
        }
        pressedDown = false;
    } else if (pressedA && !(keys & KEY_A)){
        pressedA = false;
    }

    if (keys & KEY_UP){
        pressedUp = true;
    } else if (keys & KEY_DOWN) {
        pressedDown = true;
    } 

    if(projectile1->getX() < 0){
        int rnd1 = 0 + rand() % ((2-0)+1);
        int rnd2 = 1 + rand() % ((3-1)+1);

        if (rnd1 = 0){
            projectile1->moveTo(234,40);
        } else if (rnd1 = 1){
            projectile1->moveTo(234,80);
        } else if (rnd1 = 2){
            projectile1->moveTo(234,120);
        }
        projectile1->setVelocity(-rnd2,0);
    }

    if(projectile2->getX() < 0){
        int rnd1 = 0 + rand() % ((2-0)+1);
        int rnd2 = 1 + rand() % ((3-1)+1);

        if (rnd1 = 0){
            projectile2->moveTo(234,40);
        } else if (rnd1 = 1){
            projectile2->moveTo(234,80);
        } else if (rnd1 = 2){
            projectile2->moveTo(234,120);
        }
        projectile2->setVelocity(-rnd2,0);
    }

    if(projectile1->collidesWith(*player) || projectile2->collidesWith(*player)){
        youDied();
    }
}

void MiniGameScene::youDied() {
    savedScore = savedScore + score;
    engine.get()->enqueueSound(GameOverSound, GameOverSound_bytes, 45000);
    projectile1->setVelocity(0,0);
    projectile2->setVelocity(0,0);
    TextStream::instance().clear();
    TextStream::instance().setText("You Died!", 15,15);
    TextStream::instance().setText("Press A to return", 16,15);
    dead = true;
}

void MiniGameScene::setCharacter(CharSelectScene::CharSelection character) {
    this->character = character;
}

void MiniGameScene::setData(int lives, int score, int posX, int posY) {
    this->lives = lives;
    savedScore = score;
    savedPosX = posX;
    savedPosY = posY;
}





