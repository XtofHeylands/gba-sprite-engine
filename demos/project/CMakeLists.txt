project(marioParty)

add_executable(${PROJECT_NAME}.elf
        src/recourses/backgrounds/charSelectScreen.h
        src/recourses/backgrounds/gameOverScreen.h
        src/recourses/backgrounds/miniGameScreen.h
        src/recourses/backgrounds/shroomCity.h
        src/recourses/backgrounds/startScreen.h

        src/recourses/sounds/backgroundMusic.h
        src/recourses/sounds/gameOverSound.h
        src/recourses/sounds/ShroomCityMusic.h
        src/recourses/sounds/startScreenMusic.h

        src/recourses/sprites/bulletBil.h
        src/recourses/sprites/hand.h
        src/recourses/sprites/mario.h
        src/recourses/sprites/luigi.h
        src/recourses/sprites/peach.h
        src/recourses/sprites/yoshi.h
        src/recourses/sprites/start.h

        src/CharSelectScene.h
        src/CharSelectScene.cpp
        src/GameOverScene.h
        src/GameOverScene.cpp
        src/MainGameScene.h
        src/MainGameScene.cpp
        src/MiniGameScene.h
        src/MiniGameScene.cpp
        src/StartScene.h
        src/StartScene.cpp
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )