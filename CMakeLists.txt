cmake_minimum_required(VERSION 3.16)

project(AnymateLab VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DANYMATE_DEBUG)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick ShaderTools)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_resources(RESOURCES resources.qrc)

qt_add_executable(appAnymateLab
    cubes.cpp
    cubes.h
    main.cpp
    features.cpp
    features.h
    motion.cpp
    motion.h
    cubes.cpp
    cubes.h
    curve.cpp
    curve.h
    texturerenderer.h
    texturerenderer.cpp
    graphic.h
    graphic.cpp
    color.h
    color.cpp
    earclipping.h
    earclipping.cpp
    pen.h
    pen.cpp
    anymatenamespace.h
    outline.h
    outline.cpp
    vertexgenerator.h
    vertexgenerator.cpp
    tools.h
    tools.cpp
    ${RESOURCES}
)

qt_add_qml_module(appAnymateLab
    URI AnymateLab
    VERSION 1.0
    QML_FILES
        Main.qml
        ProjectManager.qml
        MainMenu.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAnymateLab PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAnymateLab
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAnymateLab
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::Qml
    Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appAnymateLab
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_add_shaders(appAnymateLab "appAnymateLab_cube_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/cube.vert
        shaders/cube.frag
)

qt_add_shaders(appAnymateLab "appAnymateLab_shape_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/shape.vert
        shaders/shape.frag
)

qt_add_shaders(appAnymateLab "appAnymateLab_recttexture_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/recttexture.vert
        shaders/recttexture.frag
)

qt_add_shaders(appAnymateLab "appAnymateLab_motion_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/motion.vert
        shaders/motion.frag
)

qt_add_shaders(appAnymateLab "appAnymateLab_curve_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/curve.vert
        shaders/curve.frag
)

qt_add_shaders(appAnymateLab "appAnymateLab_color_shaders"
    PREFIX
        /AnymateLab
    FILES
        shaders/color.vert
        shaders/color.frag
)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/include)
