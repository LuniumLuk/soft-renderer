#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "api.hpp"
#include "platform.hpp"

using namespace Lurdr;

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
void mouseScrollEventCallback(AppWindow *window, float offset);
void mouseDragEventCallback(AppWindow *window, float x, float y);

void testVertexShader(SHADER_PARAM)
{
    Vector3 *a_pos   = layout_in(Vector3, 0);

    Vector4 *position = layout_out(Vector4, 0);
    *position = Vector4(*a_pos, 1.0f);
}

float triangle_vertices[][3] = {
    { 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
};

size_t triangle_indices[][3] = {
    { 1, 2, 3 },
    { 1, 3, 4 },
};

AppWindow *window;
FrameBuffer frame_buffer(512, 512);
Model model;
Scene scene;
Camera camera;

float rotate_angle = PI;
Vector3 camera_pos;
Vector3 camera_dir;
Vector3 camera_tar;

int main() {

#ifdef DEBUG
    printf("-------- MAIN (DEBUG) ---------\n");
#else
    printf("------------ MAIN -------------\n");
#endif

    initializeApplication();

    // OBJMesh obj_mesh("assets/armadillo.obj");
    // UniformMesh uni_mesh(obj_mesh);
    // uni_mesh.printMeshInfo();

    // camera_tar = uni_mesh.getCenter() + Vector3(0.0f, -0.2f, 0.0f);
    // camera_dir = Vector3(0.0f, 0.0f, 2.0f);
    // camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);

    // model.addMesh(&uni_mesh);
    // Matrix4 model_transform = Matrix4::IDENTITY;
    // model_transform.scale(Vector3(0.5f, 0.5f, 0.5f));
    // model.setTransform(model_transform);
    // scene.addModel(&model);
    // camera.setTransform(camera_pos, camera_tar);

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    // -------------- TEST : SHADER --------------

    VertexArray vertex_array;
    ArrayBuffer array_buffer;
    ElementBuffer element_buffer;
    array_buffer.setBufferData(12, &triangle_vertices[0][0]);
    array_buffer.setDataPointers(0, 3, 3, 0);
    element_buffer.setBufferData(6, &triangle_indices[0][0]);
    vertex_array.bindDataArray(&array_buffer);
    vertex_array.bindIndiciesArray(&element_buffer);

    Program shader_program;
    shader_program.linkShader(VERTEX_SHADER, testVertexShader);

    // -------------------------------------------

    int _fps = 0;
    clock_t last_frame_timestamp = clock();
    clock_t last_fps_update = clock();
    while (!windowShouldClose(window))
    {
        last_frame_timestamp = clock();
        
        frame_buffer.clearColorBuffer(COLOR_BLACK);
        frame_buffer.clearDepthBuffer(1.0f);

        // scene.drawSceneByFixedPipeline(frame_buffer, camera);

        if (clock() - last_fps_update > CLOCKS_PER_SEC)
        {
            _fps = CLOCKS_PER_SEC / max(1.0f, (clock() - last_frame_timestamp));
            last_fps_update = clock();
        }
        drawTriangles(frame_buffer, vertex_array, shader_program);

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "LU RENDERER", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawString(
            frame_buffer, 10.0f, 35.0f,
            "FPS", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawInteger(
            frame_buffer, 60.0f, 35.0f, 
            _fps, 10.0f, RGBColor(255.0f, 0.0f, 0.0f));

        updateView(window);
        pollEvent();
    }

    printf("------------ ---- -------------\n");
    return 0;
}

void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed)
{
    assert(window);
    if (pressed)
    {
        switch (key)
        {
            case KEY_A:
                rotate_angle -= 0.05f;
                break;
            case KEY_S:
                camera_dir.z += 0.02f;
                break;
            case KEY_D:
                rotate_angle += 0.05f;
                break;
            case KEY_W:
                if (camera_dir.z > 0.02f)
                {
                    camera_dir.z -= 0.02f;
                }
                break;
            case KEY_SPACE:
                printf("Press SPACE\n");
                break;
            case KEY_ESCAPE:
                exit(0);
                break;
            default:
                return;
        }
        camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);
        camera.setTransform(camera_pos, camera_tar);
        // scene.drawScene(frame_buffer, camera);
    }
}
void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed)
{
    assert(window);
    printf("mouseButtonEventCallback : %p %d %u\n", window, button, pressed);
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    assert(window);
    printf("mouseScrollEventCallback : %p %f\n", window, offset);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    assert(window);
    printf("mouseDragEventCallback : %p %f %f\n", window, x, y);
}