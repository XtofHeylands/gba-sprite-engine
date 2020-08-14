//
// Created by xtofh on 09/08/2020.
//

#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>

#include "MainGameScene.h"
#include "GameOverScene.h"
#include "MiniGameScene.h"

#include "recourses/backgrounds/shroomCity.h"
#include "recourses/sprites/mario.h"
#include "recourses/sprites/luigi.h"
#include "recourses/sounds/ShroomCityMusic.h"


MainGameScene::MainGameScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> MainGameScene::backgrounds() {
    return {shroomCity.get()};
}

std::vector<Sprite *> MainGameScene::sprites() {
    return {player.get()};
}

void MainGameScene::load() {
    
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(shroomCityPal, sizeof(shroomCityPal)));
    shroomCity = std::unique_ptr<Background>(new Background(1, shroomCityTiles, sizeof(shroomCityTiles), shroomCityMap, sizeof(shroomCityMap), 16, 1, MAPLAYOUT_64X64));

    SpriteBuilder<AffineSprite> affineBuilder;

    if(character == CharSelectScene::mario_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(mario_sharedPal, sizeof(mario_sharedPal)));

        player = affineBuilder
                .withData(marioTiles, sizeof(marioTiles))
                .withSize(SIZE_32_64)
                .withLocation(72 + (posX*16),225 + (posY*16))
                .buildPtr();

    } else if(character == CharSelectScene::luigi_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(luigi_sharedPal, sizeof(luigi_sharedPal)));

        player = affineBuilder
                .withData(luigiTiles, sizeof(luigiTiles))
                .withSize(SIZE_32_64)
                .withLocation(72 + (posX*16),225 + (posY*16))
                .buildPtr();

    } else if(character == CharSelectScene::peach_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    } else if(character == CharSelectScene::yoshi_char){
        foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    }

    player->makeAnimated(0,2,10);
    engine -> enqueueMusic(ShroomCityMusic, ShroomCityMusic_bytes, 45000);

}

void MainGameScene::tick(u16 keys) {

    TextStream::instance().setText("score: " + std::to_string(score), 1, 1);
    TextStream::instance().setText("lives: " + std::to_string(lives),1,20);
    TextStream::instance().setFontColor(BLD_BLACK);

    //check if the player is still alive
    if(lives <= 0) {
        if (!(engine->isTransitioning())) {
            auto gameOver = new GameOverScene(engine);
            gameOver->setFinalScore(score);
            engine->transitionIntoScene(gameOver, new FadeOutScene(5));
        }
    }

    if((pressingUp && !(keys & KEY_UP))){
        player->makeAnimated(2,4,2);
        movePlayer(up);
        idle = false;
        pressingUp = false;
    } else if (pressingDown && !(keys & KEY_DOWN)){
        player->makeAnimated(6,4,2);
        movePlayer(down);
        idle = false;
        pressingDown = false;
    } else if (pressingRight && !(keys & KEY_RIGHT)){
        player->makeAnimated(10,4,2);
        movePlayer(right);
        idle = false;
        pressingRight = false;
    } else if (pressingLeft && !(keys & KEY_LEFT)) {
        player->makeAnimated(14, 4, 2);
        movePlayer(left);
        idle = false;
        pressingLeft = false;
    } else if (pressingA && !(keys & KEY_A)) {
        startActivity();
        pressingA = false;
    } else if (pressingB && !(keys & KEY_B)) {
        pressingB = false;
    } else {
        if(!idle){
            player->makeAnimated(8,2,15);
            idle = true;
        }
    }

    //check for keypresses
    if (keys & KEY_A) {
        pressingA = true;
    } else if (keys & KEY_B){
        pressingB = true;
    } else if (keys & KEY_UP){
        pressingUp = true;
    } else if (keys & KEY_DOWN){
        pressingDown = true;
    } else if (keys & KEY_LEFT){
        pressingLeft = true;
    } else if (keys & KEY_RIGHT){
        pressingRight = true;
    }

    shroomCity->scroll(player->getX(),player->getY()-80);
}

void MainGameScene::setCharacter(CharSelectScene::CharSelection character ) {
    this->character = character;
}

void MainGameScene::movePlayer(Direction direct) {
    if (direct == up){
        if(posY > 0){
            if(featureMap[posY-1][posX] != 0){
                posY = posY - 1;
                player->moveTo(player->getX(),player->getY() - 16);
            }
        }
    } else if (direct == down){
        if(posY < 6){
            if(featureMap[posY+1][posX] != 0){
                posY = posY + 1;
                player->moveTo(player->getX(),player->getY() + 16);
            }
        }
    } else if (direct == left){
        if(posX > 0){
            if(featureMap[posY][posX-1] != 0){
                posX = posX - 1;
                player->moveTo(player->getX() - 16,player->getY());
            }
        }
    } else if (direct == right){
        if(posX < 8){
            if(featureMap[posY][posX+1] != 0){
                posX = posX + 1;
                player->moveTo(player->getX() + 16,player->getY());
            }
        }
    }

}

void MainGameScene::startActivity() {

    if(featureMap[posY][posX] == 1){
        //do nothing
    } else if (featureMap[posY][posX] == 2){
        //skip a turn
    } else if (featureMap[posY][posX] == 3){
        //roll the dice again
    } else if (featureMap[posY][posX] == 4){
        if(!engine->isTransitioning()) {
            auto miniGameScene = new MiniGameScene(engine);
            miniGameScene->setCharacter(character);
            miniGameScene->setData(lives, score, posX, posY);
            engine->transitionIntoScene(miniGameScene, new FadeOutScene(5));
        }
    }
}

void MainGameScene::setData(int lives, int score, int posX, int posY) {
    this->lives = lives;
    this->score = score;
    this->posX = posX;
    this->posY = posY;
}

//    currentLocation = {((player->getX()-72)/16),((player->getY()-225)/16)};
//
//    if(direction == right) {
//        if (currentLocation.x == 8 || featureMap[currentLocation.x + 1][currentLocation.y] == 0) {
//            if (featureMap[currentLocation.x][currentLocation.y + 1] == 0) {
//                player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y - 1) * 16) + 72));
//                direction = up;
//            } else if (featureMap[currentLocation.x][currentLocation.y - 1] == 0) {
//                player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y + 1) * 16) + 72));
//                direction = down;
//            }
//        } else {
//            player->moveTo((((currentLocation.x + 1) * 16) + 72), player->getY());
//        }
//    } else if(direction == left) {
//        if (currentLocation.x == 0 || featureMap[currentLocation.x - 1][currentLocation.y] == 0) {
//            if (featureMap[currentLocation.x][currentLocation.y + 1] == 0) {
//                player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y - 1) * 16) + 72));
//                direction = up;
//            } else if (featureMap[currentLocation.x][currentLocation.y - 1] == 0) {
//                player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y + 1) * 16) + 72));
//                direction = down;
//            }
//        } else {
//            player->moveTo((((currentLocation.x - 1) * 16) + 72), player->getY());
//        }
//    } else if(direction == up) {
//        if (currentLocation.y == 0 || featureMap[currentLocation.x][currentLocation.y - 1] == 0) {
//            if (featureMap[currentLocation.x - 1][currentLocation.y] == 0) {
//                player->moveTo((((currentLocation.x + 1) * 16) + 72), player->getY());
//                direction = right;
//            } else if (featureMap[currentLocation.x + 1][currentLocation.y] == 0) {
//                player->moveTo((((currentLocation.x - 1) * 16) + 72), player->getY());
//                direction = left;
//            }
//        } else {
//            player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y - 1) * 16) + 72));
//        }
//    } else if(direction == down) {
//            if (currentLocation.y == 6 || featureMap[currentLocation.x][currentLocation.y + 1] == 0) {
//                if (featureMap[currentLocation.x - 1][currentLocation.y] == 0) {
//                    player->moveTo((((currentLocation.x + 1) * 16) + 72), player->getY());
//                    direction = right;
//                } else if (featureMap[currentLocation.x + 1][currentLocation.y] == 0) {
//                    player->moveTo((((currentLocation.x - 1) * 16) + 72), player->getY());
//                    direction = left;
//                }
//            } else {
//                player->moveTo((((currentLocation.x) * 16) + 72), (((currentLocation.y + 1) * 16) + 72));
//            }
//    }

//int MainGameScene::rollTheDice() {
//    //pseudo-random number
//    int number = (rand() % (6 + 1 - 1)) + 1;
//    return number;
//}

















