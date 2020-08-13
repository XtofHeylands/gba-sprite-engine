//
// Created by xtofh on 06/08/2020.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

#include "StartScene.h"
#include "CharSelectScene.h"

#include "recourses/backgrounds/startScreen.h"
#include "recourses/sprites/start.h"
#include "recourses/sounds/startScreenMusic.h"

StartScene::StartScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> StartScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> StartScene::sprites() {
    return {start.get()};
}

void StartScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(startScreenPal, sizeof(startScreenPal)));
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(startSharedPal, sizeof(startSharedPal)));

    bg = std::unique_ptr<Background>(new Background(1, startScreenTiles, sizeof(startScreenTiles), startScreenMap, sizeof(startScreenMap), 24, 0, MAPLAYOUT_32X32));

    SpriteBuilder<Sprite> builder;

    start = builder
            .withData(startTiles, sizeof(startTiles))
            .withSize(SIZE_64_32)
            .withAnimated(11, 5)
            .withLocation(88,110)
            .buildPtr();

    engine -> enqueueMusic(startScreenSound, startScreenSound_bytes, 45000);
}

void StartScene::tick(u16 keys) {
    if(keys & KEY_START) {
        if(!engine -> isTransitioning()) {
            engine -> transitionIntoScene(new CharSelectScene(engine), new FadeOutScene(5));
        }
    }
}