//
// Created by xtofh on 09/08/2020.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H

#include <libgba-sprite-engine/scene.h>
#include "CharSelectScene.h"

class MainGameScene : public Scene {
private:
    std::unique_ptr<Background> shroomCity;

    std::unique_ptr<Sprite> mario;
    std::unique_ptr<Sprite> luigi;
    std::unique_ptr<Sprite> peach;
    std::unique_ptr<Sprite> yoshi;
        
    CharSelectScene::CharSelection character;

public:
    MainGameScene(const std::shared_ptr<GBAEngine> &engine);

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;

    void setCharacter(CharSelectScene::CharSelection character);
};


#endif //GBA_SPRITE_ENGINE_PROJECT_MAINGAMESCENE_H
