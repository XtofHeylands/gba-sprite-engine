//
// Created by xtofh on 13/08/2020.
//
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <algorithm>
#include <sstream>

#include "MiniGameScene.h"
#include "MainGameScene.h"
#include "recourses/backgrounds/miniGameScreen.h"
#include "recourses/sprites/luigi.h"
#include "recourses/sprites/mario.h"
#include "recourses/sprites/bulletBil.h"
#include "recourses/sounds/backgroundMusic.h"
#include "recourses/sounds/gameOverSound.h"
#include "recourses/sounds/youWin.h"


MiniGameScene::MiniGameScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> MiniGameScene::backgrounds() {
    return {miniGameBg.get()};
}

std::vector<Sprite *> MiniGameScene::sprites() {
    std::vector<Sprite *> sprites;
    for(auto& b : projectiles){
        sprites.push_back(b.get());
    }
    sprites.push_back(projectile.get());
    sprites.push_back(player.get());
    return sprites;
}

void MiniGameScene::removeProjectiles(){
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](std::unique_ptr<Sprite> &s) {return s.get()->getX()<0;}),projectiles.end());
}

void MiniGameScene::tick(u16 keys) {

    if(engine->getTimer()->getSecs() >= 30){
        youWon();
    }

    if(dead && (keys & KEY_A)){
        if(!engine->isTransitioning()){
            auto mainGameScene = new MainGameScene(engine);
            mainGameScene->setCharacter(character);
            mainGameScene->setData(lives - 1, savedScore + score, savedPosX, savedPosY);
            engine->transitionIntoScene(mainGameScene, new FadeOutScene(5));
        }
    } else if(won && (keys & KEY_A)){
        if(!engine->isTransitioning()){
            auto mainGameScene = new MainGameScene(engine);
            mainGameScene->setCharacter(character);
            mainGameScene->setData(lives, savedScore + score, savedPosX, savedPosY);
            engine->transitionIntoScene(mainGameScene, new FadeOutScene(5));
        }
    }

    if(dead || won){
        for (auto& b : projectiles) {
            b->setVelocity(0,0);
        }
        return;
    }

    int oldProjectileSize = projectiles.size();

    TextStream::instance().setText("Evade bullet Bil!", 1, 1);
    TextStream::instance().setText("score: " + std::to_string(score), 1, 20);
    TextStream::instance().setText("Time left: " + std::to_string(30 - engine->getTimer()->getSecs()), 2, 1);
    TextStream::instance().setFontColor(BLD_BLACK);

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

    if(projectileCooldown > 0){
        projectileCooldown--;
    } else if(projectileCooldown == 0 && projectiles.size() < 3){
        projectiles.push_back(createProjectile());
        auto &b = projectiles.at(projectiles.size() -1);

        int rnd = 1 + rand() % ((3-1)+1);

        b->setVelocity(-rnd,0);
    }

    removeProjectiles();

    if(oldProjectileSize != projectiles.size()){
        engine.get()->updateSpritesInScene();
    }

    for (auto& b : projectiles) {
        if(player->collidesWith(*b.get())){
            youDied();
        }
    }

//    if(projectile1->getX() < 0){
//        int rnd1 = 1 + rand() % ((3-1)+1);
//        int rnd2 = 1 + rand() % ((3-1)+1);
//
//        if (rnd1 = 1){
//            projectile1->moveTo(234,40);
//        } else if (rnd1 = 2){
//            projectile1->moveTo(234,80);
//        } else if (rnd1 = 3){
//            projectile1->moveTo(234,120);
//        }
//        projectile1->setVelocity(-rnd2,0);
//        score++;
//    }
//
//    if(projectile2->getX() < 0){
//        int rnd1 = 1 + rand() % ((3-1)+1);
//        int rnd2 = 1 + rand() % ((3-1)+1);
//
//        if (rnd1 = 1){
//            projectile2->moveTo(234,40);
//        } else if (rnd1 = 2){
//            projectile2->moveTo(234,80);
//        } else if (rnd1 = 3){
//            projectile2->moveTo(234,120);
//        }
//        projectile2->setVelocity(-rnd2,0);
//        score++;
//    }
//
//
}

std::unique_ptr<Sprite> MiniGameScene::createProjectile(){
    int xLocation;
    int yLocation;
    int rnd = 1 + rand() % ((3-1)+1);

    if (rnd == 1){
        xLocation = 234;
        yLocation = 40;
    } else if (rnd == 2) {
        xLocation = 234;
        yLocation = 80;
    } else if (rnd == 3) {
        xLocation = 234;
        yLocation = 120;
    }
    score++;
    return std::unique_ptr<Sprite>(spriteBuilder->withLocation(xLocation,yLocation).buildWithDataOf(*projectile.get()));
}


void MiniGameScene::load() {

    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(miniGameBgPal, sizeof(miniGameBgPal)));
    miniGameBg = std::unique_ptr<Background>(new Background(1, miniGameBgTiles, sizeof(miniGameBgTiles), miniGameBgMap, sizeof(miniGameBgMap), 16, 1, MAPLAYOUT_32X32));

    spriteBuilder = std::unique_ptr<SpriteBuilder<Sprite>>(new SpriteBuilder<Sprite>);
    SpriteBuilder<AffineSprite> affineBuilder;

    if(character == CharSelectScene::mario_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(miniGamesharedPal, sizeof(miniGamesharedPal)));

        player = affineBuilder
                .withData(marioTankTiles, sizeof(marioTankTiles))
                .withSize(SIZE_32_32)
                .withAnimated(4, 5)
                .withLocation(50,65)
                .buildPtr();

    } else if(character == CharSelectScene::luigi_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(luigi_miniGameSharedPal, sizeof(luigi_miniGameSharedPal)));

        player = affineBuilder
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

    projectile = spriteBuilder->withData(bulletBillTiles, sizeof(bulletBillTiles))
            .withSize(SIZE_32_16)
            .withAnimated(2,5)
            .withLocation(235,80)
            .buildPtr();

    engine->enqueueMusic(backgroundMusic, sizeof(backgroundMusic), 45000);
    miniGameBg->scroll(5,0);

    engine->getTimer()->reset();
    engine->getTimer()->start();
}

void MiniGameScene::youDied() {
    savedScore = savedScore + score;
    engine.get()->enqueueSound(GameOverSound, GameOverSound_bytes, 45000);
    TextStream::instance().clear();
    TextStream::instance().setText("You Died!", 10,11);
    TextStream::instance().setText("Press A to return", 11,7);
    TextStream::instance().setFontColor(BLD_BLACK);
    dead = true;
}

void MiniGameScene::youWon() {
    savedScore = savedScore + score;
    engine.get()->enqueueSound(youWin, youWin_bytes, 45000);
    TextStream::instance().clear();
    TextStream::instance().setText("You WON!", 10,11);
    TextStream::instance().setText("Press A to return", 11,7);
    TextStream::instance().setFontColor(BLD_BLACK);
    won = true;
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







