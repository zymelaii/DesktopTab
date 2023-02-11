cmake_minimum_required(VERSION 3.5)

project(DesktopTab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(3rdparty/QHotkey-1.5.0)

aux_source_directory(. SOURCE_FILES)
file(GLOB RC_FILES assets/*.*rc)

qt_add_executable(
	${PROJECT_NAME}
	MANUAL_FINALIZATION
	${SOURCE_FILES} ${HEADER_FILES} ${RC_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt6::Widgets
    PRIVATE QHotkey::QHotkey
)

qt_finalize_executable(${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
