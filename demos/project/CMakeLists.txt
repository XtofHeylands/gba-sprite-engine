project(marioParty)

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/StartScene.cpp
        src/StartScene.h
        src/CharSelectScene.cpp
        src/CharSelectScene.h
        src/MainGameScene.h
        src/MainGameScene.cpp
        src/recourses/backgrounds/startScreen.h
        src/recourses/backgrounds/shroomCity.h)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )