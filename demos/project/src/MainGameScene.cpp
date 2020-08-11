//
// Created by xtofh on 09/08/2020.
//

#include "MainGameScene.h"
#include "CharSelectScene.h"

#include "recourses/backgrounds/shroomCity.h"





MainGameScene::MainGameScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> MainGameScene::backgrounds() {
    return {shroomCity.get()};
}

std::vector<Sprite *> MainGameScene::sprites() {
    return {};
}

void MainGameScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(shroomCityPal, sizeof(shroomCityPal)));
    shroomCity = std::unique_ptr<Background>(new Background(1, shroomCityTiles, sizeof(shroomCityTiles), shroomCityMap, sizeof(shroomCityMap), 0, 2, MAPLAYOUT_64X64));

    backgroundPalette->persistToBank(0);
}

void MainGameScene::tick(u16 keys) {

}

void MainGameScene::setCharacter(CharSelectScene::CharSelection character ) {
    this->character = character;
}

