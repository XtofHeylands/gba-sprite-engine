//
// Created by xtofh on 06/08/2020.
//

#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/allocator.h>

#include "StartScene.h"

int main() {
    std::shared_ptr<GBAEngine> engine(new GBAEngine());

    auto startScene = new StartScene(engine);
    engine -> setScene(startScene);

    while (true){
        engine -> update();
    }
}
