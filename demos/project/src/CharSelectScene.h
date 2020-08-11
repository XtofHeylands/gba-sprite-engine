//
// Created by xtofh on 06/08/2020.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_CHARSELECTSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_CHARSELECTSCENE_H

#include <libgba-sprite-engine/scene.h>

class CharSelectScene : public Scene {
private:
    std::unique_ptr<Background> bg;
    std::unique_ptr<Sprite> mario;
    std::unique_ptr<Sprite> luigi;
    std::unique_ptr<Sprite> peach;
    std::unique_ptr<Sprite> yoshi;
    std::unique_ptr<Sprite> hand;

    bool pressingLeft, pressingRight, pressingA, selected, confirmation;

    int charPosition;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    CharSelectScene(const std::shared_ptr<GBAEngine> &engine);

    void load() override;
    void tick(u16 keys) override;

    enum CharSelection { mario_char, luigi_char, peach_char, yoshi_char };
    CharSelection charSelect;
    CharSelection getCharSelection();
};

#endif //GBA_SPRITE_ENGINE_PROJECT_CHARSELECTSCENE_H
