#include "test.hpp"

using namespace Lurdr;

static void keyboardEventCallback(AppWindow *window, KEY_CODE key, bool pressed);
static void mouseButtonEventCallback(AppWindow *window, MOUSE_BUTTON button, bool pressed);
static void mouseScrollEventCallback(AppWindow *window, float offset);
static void mouseDragEventCallback(AppWindow *window, float x, float y);

static AppWindow *window;
static FrameBuffer frame_buffer(512, 512);

static Model model;
static OldScene scene;
static Camera camera;

static float rotate_angle = PI;
static Vector3 camera_pos;
static Vector3 camera_dir;
static Vector3 camera_tar;

int test_main() {
    OBJMesh obj_mesh("assets/simple.obj");
    UniformMesh uni_mesh(obj_mesh);

    vec3 translate(1.0f, 2.0f, 3.0f);
    vec3 scale(4.4f, 5.5f, 1.2f);
    Quaternion rotation = Quaternion::fromEulerAngles(vec3(1.4f, 2.2f, 0.62f));
    mat4 transform = mat4(
        scale.x, 0.0f, 0.0f, translate.x,
        0.0f, scale.y, 0.0f, translate.y,
        0.0f, 0.0f, scale.z, translate.z,
        0.0f, 0.0f, 0.0f,    1.0f
    ).rotated(rotation);
    transform.print();

    camera_tar = uni_mesh.getCenter() + Vector3(0.0f, -0.2f, 0.0f);
    camera_dir = Vector3(0.0f, 0.0f, 2.0f);
    camera_pos = camera_tar + camera_dir.rotatedFromAxisAngle(Vector3::UNIT_Y, rotate_angle);

    model.addMesh(&uni_mesh);
    Matrix4 model_transform = Matrix4::IDENTITY;
    model_transform.scale(Vector3(0.5f, 0.5f, 0.5f));
    model.setTransform(model_transform);
    scene.addModel(&model);
    camera.setTransform(camera_pos, camera_tar);

    initializeApplication();

    const char * title = "Viewer @ Lu Renderer";
    window = createWindow(title, 512, 512, frame_buffer.colorBuffer());

    setKeyboardCallback(window, keyboardEventCallback);
    setMouseButtonCallback(window, mouseButtonEventCallback);
    setMouseScrollCallback(window, mouseScrollEventCallback);
    setMouseDragCallback(window, mouseDragEventCallback);

    long _fps = 0;
    clock_t last_frame_timestamp = clock();
    clock_t last_fps_update = clock();
    while (!windowShouldClose(window))
    {
        last_frame_timestamp = clock();
        
        if (clock() - last_fps_update > CLOCKS_PER_SEC)
        {
            _fps = CLOCKS_PER_SEC / max(1.0f, (clock() - last_frame_timestamp));
            last_fps_update = clock();
        }

        scene.drawSceneByFixedPipeline(frame_buffer, camera);

        drawString(
            frame_buffer, 10.0f, 10.0f,
            "LU RENDERER", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawString(
            frame_buffer, 10.0f, 35.0f,
            "FPS", 10.0f, RGBColor(255.0f, 255.0f, 255.0f));
        drawInteger(
            frame_buffer, 60.0f, 35.0f, 
            _fps, 10.0f, RGBColor(255.0f, 0.0f, 0.0f));

        swapBuffer(window);
        pollEvent();
    }

    terminateApplication();
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
            case KEY_ESCAPE:
                destroyWindow(window);
                break;
            case KEY_SPACE:
                {
                    Time time = getSystemTime();
                    printf("year[%ld] month[%ld] weekday[%ld] day[%ld] hour[%ld] minute[%ld] second[%ld] ms[%ld]\n", 
                        time.year, time.month, time.day_of_week, time.day, time.hour, time.minute, time.second, time.millisecond);
                }
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
    __unused_variable(window);
    printf("mouse button event : button[%d] pressed[%u]\n", button, pressed);
}
void mouseScrollEventCallback(AppWindow *window, float offset)
{
    __unused_variable(window);
    printf("mouse scroll event : offset[%.2f]\n", offset);
}
void mouseDragEventCallback(AppWindow *window, float x, float y)
{
    __unused_variable(window);
    printf("mouse drag event x[%.2f] y[%.2f]\n", x, y);
}