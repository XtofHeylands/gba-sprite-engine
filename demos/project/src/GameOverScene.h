//
// Created by xtofh on 13/08/2020.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H

#include <libgba-sprite-engine/scene.h>

class GameOverScene : public Scene {
private:
    std::unique_ptr<Background> bg;
    int finalScore;

public:
    GameOverScene(const std::shared_ptr<GBAEngine> &engine);

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;

    void setFinalScore(int score);
};

#endif //GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H
