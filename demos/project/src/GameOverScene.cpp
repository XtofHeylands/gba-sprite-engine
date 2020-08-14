//
// Created by xtofh on 13/08/2020.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <libgba-sprite-engine/background/text_stream.h>

#include "GameOverScene.h"
#include "recourses/backgrounds/gameOverScreen.h"
#include "recourses/sounds/gameOverSound.h"




GameOverScene::GameOverScene(const std::shared_ptr<GBAEngine> &engine) : Scene(engine) {}

std::vector<Background *> GameOverScene::backgrounds() {
    return {bg.get()};
}

std::vector<Sprite *> GameOverScene::sprites() {
    return {};
}

void GameOverScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(gameOverScreenPal, sizeof(gameOverScreenPal)));
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());

    bg = std::unique_ptr<Background>(new Background(1, gameOverScreenTiles, sizeof(gameOverScreenTiles), gameOverScreenMap, sizeof(gameOverScreenMap), 16, 1, MAPLAYOUT_32X32));
    bg->scroll(5, 30);

    engine->enqueueSound(GameOverSound, GameOverSound_bytes,45000);
}

void GameOverScene::tick(u16 keys) {
    TextStream::instance().clear();
    TextStream::instance().setText("final score: " + std::to_string(finalScore),13,7);
}

void GameOverScene::setFinalScore(int score) {
    finalScore = score;
}
