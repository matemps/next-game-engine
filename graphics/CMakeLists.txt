add_library(graphics STATIC
    pch.cpp
    DeviceResources.cpp
    Cube.cpp
    Sphere.cpp
    Shader.cpp
    Renderer.cpp
)

target_include_directories(graphics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../public/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/DirectXTK/Inc
)

target_link_libraries(graphics PUBLIC DirectXTK d3d11.lib dxgi.lib dxguid.lib d3dcompiler.lib)

target_compile_definitions(graphics PUBLIC UNICODE _UNICODE)
