#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include<random>
#include <algorithm>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

#include "shader/shaderManager.h"
#include "font/fontRenderer.h"
#include "shapes/rect.h"
#include "shapes/shape.h"

#include "Pirate.h"
#include "Marine.h"
#include "Attack.h"

using std::vector, std::unique_ptr, std::make_unique, glm::ortho, glm::mat4, glm::vec3, glm::vec4;
/**
 * @brief The Engine class.
 * @details The Engine class is responsible for initializing the GLFW window, loading shaders, and rendering the game state.
 */

//enum state {start, play0, play1, play2, play3, play4, play5, over};

class Engine {
private:
    /// @brief The actual GLFW window.
    GLFWwindow* window{};

    /// @brief The width and height of the window.
    const unsigned int width = 800, height = 600; // Window dimensions
    /// @brief The projection matrix
    const glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);


    /// @brief Keyboard state (True if pressed, false if not pressed).
    /// @details Index this array with GLFW_KEY_{key} to get the state of a key.
    bool keys[1024];

    /// @brief Responsible for loading and storing all the shaders used in the project.
    /// @details Initialized in initShaders()
    unique_ptr<ShaderManager> shaderManager;

    /// @brief Responsible for rendering text on the screen.
    /// @details Initialized in initShaders()
    unique_ptr<FontRenderer> fontRenderer;

    // Shapes
    //unique_ptr<Shape> spawnButton;
    unique_ptr<Shape> leftButton;
    unique_ptr<Shape> rightButton;
    unique_ptr<Shape> luffyButton;
    unique_ptr<Shape> zoroButton;
    unique_ptr<Shape> namiButton;
    unique_ptr<Shape> usoppButton;
    unique_ptr<Shape> sanjiButton;
    unique_ptr<Shape> battleButton;
    vector<unique_ptr<Shape>> confetti;
    vector<unique_ptr<Shape>> squares;
    vector<unique_ptr<Rect>> waves;

    // Shaders
    Shader shapeShader;
    Shader textShader;
    // unique_ptr<ShaderManager> shaderManager;
    // unique_ptr<FontRenderer> fontRenderer;
    //
    // vector<unique_ptr<Rect>> confetti;
    //declaring vectors for sprites
    vector<unique_ptr<Rect>> pirateShip;
    vector<unique_ptr<Rect>> marineShip;
    vector<unique_ptr<Rect>> wave;

    //bool keys[1024];
    double MouseX, MouseY;
    bool mousePressedLastFrame = false;
    const int SIDE_LENGTH = 20;
    /// @note Call glCheckError() after every OpenGL call to check for errors.
    GLenum glCheckError_(const char *file, int line);
    /// @brief Macro for glCheckError_ function. Used for debugging.
#define glCheckError() glCheckError_(__FILE__, __LINE__)

public:
    /// @brief Constructor for the Engine class.
    /// @details Initializes window and shaders.
    Engine();

    /// @brief Destructor for the Engine class.
    ~Engine();

    ///@breif makes a color darker
    ///@details takes a vec4 and subtracts from it to make it darker
    static vec4 colorConverterDark(vec4 color);

    ///@breif makes a color lighter
    ///@details takes a vec4 and adds to it to make it lighter
    static vec4 colorConverterLight(vec4 color);

    /// @brief Initializes the GLFW window.
    /// @return 0 if successful, -1 otherwise.
    unsigned int initWindow(bool debug = false);
    void initShaders();
    void initShapes();
    void processInput();
    void update();
    void render();

    /* deltaTime variables */
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame (used to calculate deltaTime)

    // -----------------------------------
    // Getters
    // -----------------------------------

    /// @brief Returns true if the window should close.
    /// @details (Wrapper for glfwWindowShouldClose()).
    /// @return true if the window should close
    /// @return false if the window should not close
    bool shouldClose();

    void passInVectors(vector<Pirate> p, vector<Marine> m);
    void buttonColorStuff(unique_ptr<Shape>& button, vec4 originalColor, bool& overlap, bool& mousePressedLastFrame, bool& mousePressed);
    void spawnConfetti();
    void readFromFile(string filepath, vector<unique_ptr<Rect>> sprites);

    /// Projection matrix used for 2D rendering (orthographic projection).
    /// We don't have to change this matrix since the screen size never changes.
    /// OpenGL uses the projection matrix to map the 3D scene to a 2D viewport.
    /// The projection matrix transforms coordinates in the camera space into normalized device coordinates (view space to clip space).
    /// @note The projection matrix is used in the vertex shader.
    // 4th quadrant
    mat4 PROJECTION = ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    // 1st quadrant
//        mat4 PROJECTION = ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

};

#endif // ENGINE_H#endif //GRAPHICS_ENGINE_H