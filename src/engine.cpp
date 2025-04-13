#include "engine.h"

enum state {start, charSelect, randomChar, selectChar, round0, encounter, round2, escapeFail, escape, fightChoose, battleScreen, over, fightWon, fightLost};
state screen = start;

vector<Pirate> pirates;
vector<Marine> marines;
Attack tempATK("temp", "temp", "temp", "temp");
Pirate player("temp", "temp", tempATK, "temp", 333); // this might not get completely overwritten
Player* enemy = nullptr;
bool frameOne = true;

// Colors for buttons
color originalFillL, originalFillR, luffy, zoro, nami, usopp, sanji, battle;

Engine::Engine() : keys()
{
    this->initWindow();
    this->initShaders();
    this->initShapes();

    // left and right button colors
    originalFillL.vec = vec4{1, 0, 0, 1};
    originalFillR.vec = vec4{0, 0, 1, 1};

    // crew colors
    luffy.vec = vec4{.8, 0, 0, 1};
    zoro.vec = vec4{0.33, 0.41, 0.18, 1};
    nami.vec = vec4{1, 0.54, 0, 1};
    usopp.vec = vec4{0.54, 0.27, 0.07, 1};
    sanji.vec = vec4{0.75, 0.75, 0, 1};

    // battle button color
    battle.vec = vec4{0.6, 0.1, 0.1, 1};


}

Engine::~Engine() {}

 vec4 Engine::colorConverterDark(vec4 color)
{
    vec4 newColor = vec4{0, 0, 0 ,0};
    for (int i = 0; i < 4; i++)// 4 is the length of color vectors
    {
        if (color[i] < 0.4)// .4 so it's not 0 (black) when .3 gets subtracted
            newColor[i] = .1;// .1 so it's still slightly visible on black background
        else
            newColor[i] = color[i] - 0.3;
    }
    return newColor;
}
vec4 Engine::colorConverterLight(vec4 color)
{
    vec4 newColor = vec4{0, 0, 0 ,0};
    for (int i = 0; i < 4; i++)
    {
        if (color[i] > 0.7)// .7 so it cant go over 1 when adding .3 in else statement
            newColor[i] = 1;
        else
            newColor[i] = color[i] + 0.3;
    }
    return newColor;
}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "One Piece Battle Game!", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert", "../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"), "../res/fonts/MxPlus_IBM_BIOS.ttf", 24);

    // Set uniforms
    textShader.setVector2f("vertex", vec4(100, 100, .5, .5));
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // red spawn button centered in the top left corner
    //spawnButton = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{100, 50}, color{1, 0, 0, 1});
    leftButton = make_unique<Rect>(shapeShader, vec2{width/4,height/2}, vec2{100, 50}, color{1, 0, 0, 1});
    rightButton = make_unique<Rect>(shapeShader, vec2{width*3/4,height/2}, vec2{100, 50}, color{1, 0, 0, 1});
    luffyButton = make_unique<Rect>(shapeShader, vec2{width*5/22,height/2}, vec2{100, 50}, luffy);
    zoroButton = make_unique<Rect>(shapeShader, vec2{width*8/22,height/2}, vec2{100, 50}, zoro);
    namiButton = make_unique<Rect>(shapeShader, vec2{width*11/22,height/2}, vec2{100, 50}, nami);
    usoppButton = make_unique<Rect>(shapeShader, vec2{width*14/22,height/2}, vec2{100, 50}, usopp);
    sanjiButton = make_unique<Rect>(shapeShader, vec2{width*17/22,height/2}, vec2{100, 50}, sanji);
    battleButton = make_unique<Rect>(shapeShader, vec2{width/2,height/2}, vec2{100, 50}, battle);

        // Relative file path from inside cmake-build-debug folder to the txt file
    readFromFile("../res/art/pirateShip.txt", pirateShip);
    readFromFile("../res/art/marineShip.txt", marineShip);
    readFromFile("../res/art/wave.txt", wave);
}   //add another txt file?


// <|:-D
void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Mouse position saved to check for collisions
    glfwGetCursorPos(window, &MouseX, &MouseY);

    // Mouse position is inverted because the origin of the window is in the top left corner
    MouseY = height - MouseY; // Invert y-axis of mouse position
    bool leftButtonOverlapsMouse = leftButton->isOverlapping(vec2(MouseX, MouseY));
    bool rightButtonOverlapsMouse = rightButton->isOverlapping(vec2(MouseX, MouseY));
    bool luffyButtonOverlapsMouse = luffyButton->isOverlapping(vec2(MouseX, MouseY));
    bool zoroButtonOverlapsMouse = zoroButton->isOverlapping(vec2(MouseX, MouseY));
    bool namiButtonOverlapsMouse = namiButton->isOverlapping(vec2(MouseX, MouseY));
    bool usoppButtonOverlapsMouse = usoppButton->isOverlapping(vec2(MouseX, MouseY));
    bool sanjiButtonOverlapsMouse = sanjiButton->isOverlapping(vec2(MouseX, MouseY));
    bool battleButtonOverlapsMouse = battleButton->isOverlapping(vec2(MouseX, MouseY));
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // If we're in the start screen and the user presses s, start game
    if (screen == start)
    {
        if (keys[GLFW_KEY_S]) screen = charSelect;
    }
    // let user select a character (from the first 5 strawhats) or be randomly assigned one
    if (screen == charSelect)
    {
        buttonColorStuff(leftButton, originalFillL.vec, leftButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(rightButton, originalFillR.vec, rightButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        // THIS IS WHERE BUTTON FUNCTIONALITY GOES
        if (!mousePressed && mousePressedLastFrame && leftButtonOverlapsMouse)
        {
            mousePressedLastFrame = false;
            screen = selectChar;
        }
        // THIS IS WHERE BUTTON FUNCTIONALITY GOES
        if (rightButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            mousePressedLastFrame = false;
            screen = randomChar;
        }
    }
    // random assignment
    if (screen == randomChar)
    {
        // get random pirate assign them to player and remove from vector
        const int index = rand() % pirates.size();
        player = pirates[index]; // TODO: MIGHT ALSO HAVE TO SET PLAYER'S ATTACK TO PIRATES ATTACK
        // remove that random pirate from vector so they cannot fight themselves
        pirates.erase(pirates.begin() + index);
        screen = round0;
    }
    // assignment of the user's choice
    if (screen == selectChar)
    {
        buttonColorStuff(luffyButton, luffy.vec, luffyButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(zoroButton, zoro.vec, zoroButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(namiButton, nami.vec, namiButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(usoppButton, usopp.vec, usoppButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(sanjiButton, sanji.vec, sanjiButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        bool picked = false;
        if (luffyButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            player = pirates[0]; // luffy is at index 0
            picked = true;
        }
        if (zoroButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            player = pirates[1];// zoro is at index 1
            picked = true;
        }
        if (namiButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            player = pirates[3];// nami is at index 3
            picked = true;
        }
        if (usoppButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            player = pirates[9];// usopp is at index 9
            picked = true;
        }
        if (sanjiButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            player = pirates[2];// sanji is at index 2
            picked = true;

        }
        if (picked)
        {
            // remove all strawhats from pirate vector so they cant fight each other
            // highest to lowest as to not mess with the vector order
            pirates.erase(pirates.begin() + 9);
            pirates.erase(pirates.begin() + 3);
            pirates.erase(pirates.begin() + 2);
            pirates.erase(pirates.begin() + 1);
            pirates.erase(pirates.begin() + 0);
            mousePressedLastFrame = false;
            screen = round0;
        }
    }
    // user presses s to start gameplay
    if (screen == round0)
    {
        // shuffle the vectors to make game more unique
        shuffle(std::begin(marines), std::end(marines), std::mt19937(std::random_device()()));
        shuffle(std::begin(pirates), std::end(pirates), std::mt19937(std::random_device()()));
        if (keys[GLFW_KEY_S]) screen = encounter;
    }
    // user gets told who is approaching, is given the option to fight or flee
    // if they flee there is a 50/50 chance they escape and go to escape screen
    // else they go to fight screen
    if (screen == encounter)
    {
        // setup
        // random chance to fight a pirate or marine
        if (frameOne) // checks if we are on frame one so it doesn't give you a new enemy every frame
        {
            if (rand() % 2 == 0) // pirate
            {
                enemy = &pirates[0]; // enemy points to new pirate
                pirates.erase(pirates.begin() + 0); //remove that guy from the vector
            }
            else // marine
            {
                enemy = &marines[0]; // enemy points to new marine
                marines.erase(marines.begin() + 0); //remove that guy from the vector
            }
            frameOne = false; // make sure to set this to true before leaving screen
        }

        // button stuff
        buttonColorStuff(leftButton, originalFillL.vec, leftButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        buttonColorStuff(rightButton, originalFillR.vec, rightButtonOverlapsMouse, mousePressedLastFrame, mousePressed);

        // THIS IS WHERE BUTTON FUNCTIONALITY GOES
        if (!mousePressed && mousePressedLastFrame && leftButtonOverlapsMouse)
        {
            if (rand() % 2 == 0)// 50/50 chance they run successfully
            {
                frameOne = true;
                mousePressedLastFrame = false;
                screen = escapeFail;
            }
            else // win
            {
                frameOne = false;
                mousePressedLastFrame = false;
                screen = escape;
            }
        }
        // THIS IS WHERE BUTTON FUNCTIONALITY GOES
        if (rightButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            frameOne = true;
            mousePressedLastFrame = false;
            screen = fightChoose;
        }
    }

    if (screen == battleScreen)
    {
        // player's name vs enemy name
        // button to spam click 10 times
        // if (after button clicked) your power is higher than enemy you win
        // else you lose

        buttonColorStuff(battleButton, battle.vec, battleButtonOverlapsMouse, mousePressedLastFrame, mousePressed);
        int playerAttack = player.getAttack().getAttackPower();
        int enemyAttack = enemy->getAttack().getAttackPower();
        //string playerPower = std::to_string(playerAttack);
        //this->fontRenderer->renderText(playerPower, width/4 - (12 * playerPower.length()), height/3, projection, 1, vec3{1, 1, 1});
        if (battleButtonOverlapsMouse && !mousePressed && mousePressedLastFrame)
        {
            mousePressedLastFrame = false;
            spawnConfetti();
        }
        // If the size of the confetti vector reaches 10, determine winner of battle and change screen
        if (confetti.size() > 10) {
            if (playerAttack >= enemyAttack)
            {
                screen = fightWon;
            }
            else // playerAttack < enemyAttack
            {
                screen = fightLost;
            }
            // maybe display user's and enemy's attack power and as the click, user attack increases
        }
    }

    if (screen == escapeFail)
    {
        if (keys[GLFW_KEY_S]) screen = battleScreen; // failed to escape so they go into battle
    }
    if (screen == fightChoose)
    {
        if (keys[GLFW_KEY_S]) screen = battleScreen; // chose to fight so they go into battle
    }
    if (screen == escape)
    {
        if (keys[GLFW_KEY_S]) screen = encounter; // escaped so they just get another encounter
    }

    if (screen == fightLost)
    {
        //TODO: put this in render


        if (keys[GLFW_KEY_S]) screen = encounter; //
    }

    // Save mousePressed for next frame
    mousePressedLastFrame = mousePressed;
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // End the game when the user spawns 100 confetti
    // If the size of the confetti vector reaches 100, change screen to over
    // if (confetti.size() > 100) {
    //     screen = over;
    // }

}

void Engine::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set shader to draw shapes
    shapeShader.use();

    // Render differently depending on screen
    switch (screen) {
        case start: {
            string message = "Press s to start";
            /*// (12 * message.length()) is the offset to center text.
            // 12 pixels is the width of each character scaled by 1.
            // NOTE: This line changes the shader being used to the font shader.
            //  If you want to draw shapes again after drawing text,
            //  you'll need to call shapeShader.use() again first.*/
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            break;
        }
        case charSelect: {
            // call set uniforms and draw on buttons for select and get character
            leftButton->setUniforms();
            leftButton->draw();
            rightButton->setUniforms();
            rightButton->draw();
            fontRenderer->renderText("select", leftButton->getPos().x - 30, leftButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("get", rightButton->getPos().x - 30, rightButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            string message = "Would you like to";
            string message2 = "SELECT your own or";
            string message3 = "GET a random character?";
            this->fontRenderer->renderText(message, width/2 - (8 * message.length()), height*14/16, projection, .7, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (8 * message2.length()), height*13/16, projection, .7, vec3{1, 1, 1});
            this->fontRenderer->renderText(message3, width/2 - (8 * message3.length()), height*12/16, projection, .7, vec3{1, 1, 1});
            break;
        }

        case selectChar: {
            // set uniforms and draw the crew
            luffyButton->setUniforms();
            luffyButton->draw();
            zoroButton->setUniforms();
            zoroButton->draw();
            namiButton->setUniforms();
            namiButton->draw();
            usoppButton->setUniforms();
            usoppButton->draw();
            sanjiButton->setUniforms();
            sanjiButton->draw();

            // add text to the buttons (separately so we don't have to keep typing shapeShader.use())
            fontRenderer->renderText("Luffy", luffyButton->getPos().x - 30, luffyButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("Zoro", zoroButton->getPos().x - 30, zoroButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("Nami", namiButton->getPos().x - 30, namiButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("Usopp", usoppButton->getPos().x - 30, usoppButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("Sanji", sanjiButton->getPos().x - 30, sanjiButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            string message = "Pick your character!";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height*2/3, projection, 1, vec3{1, 1, 1});
            break;
        }
        case round0: {
            string message ="Your Voyage Begins!";
            string message2 = "Press s to start";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (8 * message2.length()), height*3/16, projection, .7, vec3{1, 1, 1});
            string message3 = player.getName();
            this->fontRenderer->renderText(message3, width/2 - (8 * message3.length()), height*9/16, projection, .7, vec3{1, 1, 1});

            break;
        }
        case encounter: {
            // call set uniforms and draw on buttons for run or fight
            leftButton->setUniforms();
            leftButton->draw();
            rightButton->setUniforms();
            rightButton->draw();
            fontRenderer->renderText("Run", leftButton->getPos().x - 30, leftButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            fontRenderer->renderText("Fight", rightButton->getPos().x - 30, rightButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
            string message = "An enemy " + enemy->getAffiliation() + " ship approaches!";
            string message2 = "Will you";
            string message3 = "RUN or FIGHT?";
            this->fontRenderer->renderText(message, width/2 - (8 * message.length()), height*14/16, projection, .7, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (8 * message2.length()), height*13/16, projection, .7, vec3{1, 1, 1});
            this->fontRenderer->renderText(message3, width/2 - (8 * message3.length()), height*12/16, projection, .7, vec3{1, 1, 1});
            break;
        }
        case escapeFail:{
            string message ="You failed to get away!";
            string message2 = "Press S to start Battle!";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (8 * message2.length()), height*3/16, projection, .7, vec3{1, 1, 1});
            break;
        }

        case escape:{
            string message = "You got away safely!";
            string message2 = "Press S to start next encounter";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (12 * message2.length()), height/3, projection, 1, vec3{1, 1, 1});
            break;
        }

        case fightChoose:{
            string message ="You sail to attack!";
            string message2 = "Press S to start Battle!";
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            this->fontRenderer->renderText(message2, width/2 - (8 * message2.length()), height*3/16, projection, .7, vec3{1, 1, 1});
            break;
        }
        case over: {
            string message = "You win!";
            // Display the message on the screen
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
            break;
        }
        case fightLost:{
            string message;
            if (enemy->getAffiliation() == "Pirate")
            {
                message = "Enemy pirate won";
            }
            else // enemy is a marine
            {
                message = "Enemy marine won";
            }
            this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
                break;
        }

        case fightWon:{
                string message = "You won the battle!";

                this->fontRenderer->renderText(message, width/2 - (12 * message.length()), height/2, projection, 1, vec3{1, 1, 1});
                break;
        }
        case battleScreen:{

                string playerPower = std::to_string(player.getAttack().getAttackPower());
                string enemyPower = std::to_string(enemy->getAttack().getAttackPower());
            for (int i = 0; i < confetti.size(); i++ )
            {
                confetti[i]->setUniforms();
                confetti[i]->draw();
            }
                battleButton->setUniforms();
                battleButton->draw();
                fontRenderer->renderText("Battle!", battleButton->getPos().x - 38, battleButton->getPos().y - 5, projection, 0.5, vec3{1, 1, 1});
                //this->fontRenderer->renderText(playerPower, width/4 - (12 * playerPower.length()), height/3, projection, 1, vec3{1, 1, 1});
                //this->fontRenderer->renderText(enemyPower, width/4 - (12 * enemyPower.length()), height/3, projection, 1, vec3{1, 1, 1});
                for (int i = 0; i < wave.size(); ++i) {
                    // Move all the waves to the left
                    wave[i]->moveX(-1.5);
                    // If a wave has moved off the screen
                    if (wave[i]->getPosX() < -(wave[i]->getSize().x/2)) {
                        // Set it to the right of the screen so that it passes through again
                        int waveOnLeft = (wave[i] == wave[0]) ? wave.size()-1 : i - 1;
                        wave[i]->setPosX(wave[waveOnLeft]->getPosX() + wave[waveOnLeft]->getSize().x/2 + wave[i]->getSize().x/2 + 5);
                    }
                    wave[i]->setPosY(SIDE_LENGTH);
                    wave[i]->setUniforms();
                    wave[i]->draw();
                }
                for (int i = 0; i < pirateShip.size(); ++i) {
                    pirateShip[i]->setUniforms();
                    pirateShip[i]->draw();
                }
            break;
        }
    }

    glfwSwapBuffers(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT);

        // Set shader to use for all shapes
        shapeShader.use();
/**logic for drawing sprites (maybe works? IDK)
    for (unsigned int i = 0; i < pirateShip.size(); i++) {
        pirateShip[i]->setUniforms();
        pirateShip[i]->draw();
    }

    for (unsigned int i = 0; i < marineShip.size(); i++) {
        marineShip[i]->setUniforms();
        marineShip[i]->draw();
    }

    for (unsigned int i = 0; i < wave.size(); i++) {
        wave[i]->setUniforms();
        wave[i]->draw();
    } */

    glfwSwapBuffers(window);
}


void Engine::spawnConfetti() {
    vec2 pos = {rand() % (int)width, rand() % (int)height};
    // Make each piece of confetti a different size, getting bigger with each spawn.
    //  The smallest should be a square of size 1 and the biggest should be a square of size 100
    vec2 size = {confetti.size() + 10, confetti.size() + 10}; // placeholder
    color color = {float(rand() % 10 / 10.0), float(rand() % 10 / 10.0), float(rand() % 10 / 10.0), 1.0f};
    confetti.push_back(make_unique<Rect>(shapeShader, pos, size, color));
}
void Engine::readFromFile(std::string filepath, vector<unique_ptr<Rect>> sprites) {
    ifstream ins(filepath);
    if (!ins) {
        cout << "Error opening file" << endl;
    }
    ins >> noskipws;
    int xCoord = 0, yCoord = height-SIDE_LENGTH;
    char letter;
    bool draw;
    color c;
    while (ins >> letter) {
        draw = true;
        switch(letter) {
        case 'r': c = color(.3, 0, 0); break;
        case 'g': c = color(0, 1, 0); break;
        case 'b': c = color(0, 0, 1); break;
        case '8': c = color(0.4, .65, 1); break; //blue 2 (light)
        case '9': c = color(0, 0, .25); break; //blue 3 (dark)
        case 'y': c = color(1, 1, 0); break;
        case 'm': c = color(1, 0, 1); break;
        case 'c': c = color(0, 1, 1); break;
        case 'w': c = color(1, 1, 1); break;
        case '1': c = color(0.65, 0.50, 0.39); break; //brown1
        case '2': c = color(0.55, 0.27, 0.07); break; //brown2
        case '3': c = color(0.36, 0.25, 0.20); break; //brown3
        case '4': c = color(0.40, 0.26, 0.13); break; //brown4
        case '5': c = color(0.75, 0.75, 0.75); break; //grey1
        case '6': c = color(0.40, 0.40, 0.40); break; //grey2
        case '7': c = color(0.15, 0.15, 0.15); break; //gray3
        case ' ': c = color(0, 0, 0); break;
        default: // newline
            draw = false;
            xCoord = 0;
            yCoord -= SIDE_LENGTH;
        }
        if (draw) {
            squares.push_back(make_unique<Rect>(shapeShader, vec2(xCoord + SIDE_LENGTH/2, yCoord + SIDE_LENGTH/2), vec2(SIDE_LENGTH, SIDE_LENGTH), c));
            xCoord += SIDE_LENGTH;
        }
    }
    ins.close();
}



bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}
void Engine::passInVectors(vector<Pirate> p, vector<Marine> m)
{
    pirates = p;
    marines = m;
}
void Engine::buttonColorStuff(unique_ptr<Shape>& button, vec4 originalColor, bool& overlap, bool& mousePressedLastFrame, bool& mousePressed)
{
    if (overlap)
    {
        button->setColor(colorConverterLight(originalColor));
        if (mousePressed)
            button->setColor(colorConverterDark(originalColor));
    }
    else // Make sure the button is its original color when the user is not hovering or clicking on it.
        button->setColor(originalColor);
}

GLenum Engine::glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        cout << error << " | " << file << " (" << line << ")" << endl;
    }
    return errorCode;
}