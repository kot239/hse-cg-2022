set(SRCS
    main.cpp
        GLLoader.cpp
        GLLoader.h
        ModelLoader.cpp
        ModelLoader.h

        shaders.qrc
        Shaders/lightning.fs
        Shaders/lightning.vs
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(demo-app ${SRCS})

target_link_libraries(demo-app
    PRIVATE
        Qt5::Widgets
        FGL::Base
        assimp
)