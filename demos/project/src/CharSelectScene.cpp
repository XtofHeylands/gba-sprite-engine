//
// Created by xtofh on 06/08/2020.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/background/text_stream.h>

#include "CharSelectScene.h"
#include "recourses/sounds/backgroundMusic.h"
#include "recourses/backgrounds/charSelectScreen.h"
#include "recourses/sprites/mario.h"
#include "recourses/sprites/luigi.h"
#include "recourses/sprites/peach.h"
#include "recourses/sprites/yoshi.h"
#include "recourses/sprites/hand.h"
#include "StartScene.h"
#include "MainGameScene.h"

CharSelectScene::CharSelectScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> CharSelectScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> CharSelectScene::sprites() {
    return {mario.get(), luigi.get(), peach.get(), yoshi.get(), hand.get()};
}

void CharSelectScene::load() {
    charPosition = 1;

    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(charSelectPal, sizeof(charSelectPal)));
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(charSelectSharedPal, sizeof(charSelectSharedPal)));

    bg = std::unique_ptr<Background>(new Background(1, charSelectTiles, sizeof(charSelectTiles), charSelectMap, sizeof(charSelectMap), 16, 1, MAPLAYOUT_32X32));

    SpriteBuilder<Sprite> builder;

    mario = builder
            .withData(marioSelTiles, sizeof(marioSelTiles))
            .withSize(SIZE_32_32)
            .withLocation(50,60)
            .buildPtr();

    luigi = builder
            .withData(luigiSelTiles, sizeof(luigiSelTiles))
            .withSize(SIZE_32_32)
            .withLocation(85, 60)
            .buildPtr();

    peach = builder
            .withData(peachSelTiles, sizeof(peachSelTiles))
            .withSize(SIZE_32_32)
            .withLocation(120, 60)
            .buildPtr();

    yoshi = builder
            .withData(yoshiSelTiles, sizeof(yoshiSelTiles))
            .withSize(SIZE_32_32)
            .withLocation(155, 60)
            .buildPtr();

    hand = builder
            .withData(handSelTiles, sizeof(handSelTiles))
            .withSize(SIZE_16_16)
            .withLocation(55, 40)
            .buildPtr();

    engine -> enqueueMusic(backgroundMusic, backgroundMusic_bytes, 45000);
}

void CharSelectScene::tick(u16 keys) {

    if (!selected){
        if (pressingLeft && !(keys & KEY_LEFT)) {
            if (charPosition == 1){
                charPosition = 4;
            } else {
                charPosition = charPosition - 1;
            }
            pressingLeft = false;
        }

        if (pressingRight && !(keys & KEY_RIGHT)) {
            if (charPosition == 4){
                charPosition = 1;
            } else {
                charPosition = charPosition + 1;
            }
            pressingRight = false;
        }

        if ((keys & KEY_B) && !(engine->isTransitioning())){
            engine->transitionIntoScene(new StartScene(engine), new FadeOutScene(5));
        }

        if (pressingA && !(keys & KEY_A)){

            TextStream::instance().setText(" are you sure?",13,8);
            TextStream::instance().setFontColor(BLD_BLACK);

            TextStream::instance().setText(" yes", 15,10);
            TextStream::instance().setFontColor(BLD_BLACK);

            TextStream::instance().setText(">no", 15,17);
            TextStream::instance().setFontColor(BLD_BLACK);

            selected = true;
            confirmation = false;

            pressingA = false;

        } else if (keys & KEY_LEFT){
            pressingLeft = true;
        } else if (keys & KEY_RIGHT){
            pressingRight = true;
        } else if (keys & KEY_A){
            pressingA = true;
        }
    } else {
        if ((pressingLeft && !(keys & KEY_LEFT)) || (pressingRight && !(keys & KEY_RIGHT))) {
            TextStream::instance().clear();
            TextStream::instance().setText(" are you sure?",13,8);
            TextStream::instance().setFontColor(BLD_BLACK);

            if(!confirmation){
                TextStream::instance().setText(">yes", 15,10);
                TextStream::instance().setFontColor(BLD_BLACK);

                TextStream::instance().setText(" no", 15,17);
                TextStream::instance().setFontColor(BLD_BLACK);
                confirmation = true;
            } else {
                TextStream::instance().setText(" yes", 15,10);
                TextStream::instance().setFontColor(BLD_BLACK);

                TextStream::instance().setText(">no", 15,17);
                TextStream::instance().setFontColor(BLD_BLACK);
                confirmation = false;
            }

            pressingLeft = false;
            pressingRight = false;
        } else if (keys & KEY_LEFT) {
            pressingLeft = true;
        } else if (keys & KEY_RIGHT){
            pressingRight = true;
        } else if (keys & KEY_A){
            pressingA = true;
        }

        if(pressingA && !(keys & KEY_A)){

            if(confirmation){
                if (charPosition == 1) {
                    charSelect = CharSelection::mario_char;
                } else if (charPosition == 2) {
                    charSelect = CharSelection::luigi_char;
                } else if (charPosition == 3) {
                    charSelect = CharSelection::peach_char;
                } else if (charPosition == 4) {
                    charSelect = CharSelection::yoshi_char;
                }

                //character selection meegeven aan de volgende scene
                auto mainGameScene = new MainGameScene(engine);
                mainGameScene->setCharacter(charSelect);

                engine->transitionIntoScene(mainGameScene, new FadeOutScene(5));
            } else {
                selected = false;
                TextStream::instance().clear();
            }
            pressingA = false;
        }
    }

    if (charPosition == 1) {
        hand->moveTo(55, 40);
    } else if (charPosition == 2) {
        hand->moveTo(90, 40);
    } else if (charPosition == 3) {
        hand->moveTo(125, 40);
    } else if (charPosition == 4) {
        hand->moveTo(160,40);
    }
}