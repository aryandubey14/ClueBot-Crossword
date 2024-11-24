#include <freeglut.h> 
#include <iostream>
#include <memory>
#include <windows.h> 
#include <mmsystem.h> 
#include <string>
#pragma comment(lib, "winmm.lib") 

#define STB_IMAGE_IMPLEMENTATION
#include "Header1.h"  


enum class GameState { INTRO, GAME, HOME };
    
class CrosswordGame {
public:
    CrosswordGame(int width, int height);
    void initOpenGL();
    void display();
    void handleMouse(int button, int state, int x, int y);
    void update();
    void playBackgroundMusic(const char* filename);

private:
    GLuint loadTexture(const char* filename);
    void drawBackground();
    void drawButtonBackground(); 
    void drawPlayButton();
    void drawHTPButton();
    void drawGrid();
    void fillGridWithWords();
    void drawExitButton(); 
    void drawProp();
    void drawNewProp();
    void drawTopLeftProp();
    void drawSettingsProp();
    void drawEasyButton();
    void drawHardButton();


    int windowWidth, windowHeight;
    
    char grid[10][10];
    const int buttonBackgroundWidth = 600;  
    const int buttonBackgroundHeight = 800;                         
    const int Horizontaloffset = 80;
    const int verticalOffset = -70;
    const int HTPverticaloffset = 20;
    const int exitButtonSize = 50; 

    int buttonX, buttonY, buttonWidth, buttonHeight;
    GLuint backgroundTexture;
    GLuint playPropTexture;
    GLuint buttonTexture; 
    GLuint HTPButtonTexture;
    GLuint buttonBackgroundTexture; 
    GLuint exitButtonTexture; 
    GLuint propTexture;
    GLuint propTexture2;
    GLuint newPropTexture;
    GLuint topLeftPropTexture;  
    GLuint settingsPropTexture;
    GLuint EzPropTexture;
    GLuint hardButtonTexture;


    GameState currentState;
    const int gridSize = 10;  
};


// Constructor
CrosswordGame::CrosswordGame(int width, int height)
    : windowWidth(width), windowHeight(height),
    buttonWidth(300), buttonHeight(150),
    currentState(GameState::INTRO) {
    
    buttonX = (windowWidth - buttonWidth) / 2;
    buttonY = (windowHeight - buttonHeight) / 2 - 280;

}

void CrosswordGame::playBackgroundMusic(const char* filename) {
    std::wstring wideFilename = std::wstring(filename, filename + strlen(filename));
    PlaySound(wideFilename.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

// Initialize OpenGL settings
void CrosswordGame::initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  

    // Load textures
    backgroundTexture = loadTexture("D:/Minor_Project/Props/Bck.png");
    buttonTexture = loadTexture("D:/Minor_Project/Props/h.png");
    buttonBackgroundTexture = loadTexture("D:/Minor_Project/Props/c.png");
    HTPButtonTexture = loadTexture("D:/Minor_Project/Props/h.png");
    exitButtonTexture = loadTexture("D:/Minor_Project/Props/L.png"); 
    playPropTexture = loadTexture("D:/Minor_Project/Props/Play.png");
    propTexture = loadTexture("D:/Minor_Project/Props/Bot.png"); 
    //propTexture2 = loadTexture("D:/Minor_Project/Props/p1.png"); // Load the new prop texture
    newPropTexture = loadTexture("D:/Minor_Project/Props/j1.png"); 
    topLeftPropTexture = loadTexture("D:/Minor_Project/Props/y1.png");
    settingsPropTexture = loadTexture("D:/Minor_Project/Props/z1-removebg-preview.png");
    EzPropTexture = loadTexture("D:/Minor_Project/Props/ez.png");
    hardButtonTexture = loadTexture("D:/Minor_Project/Props/hard.png");


    if (!backgroundTexture || !buttonTexture || !buttonBackgroundTexture || !HTPButtonTexture ||
        !exitButtonTexture || !playPropTexture || !propTexture || !newPropTexture || !topLeftPropTexture ||
        !settingsPropTexture) {
        std::cerr << "Failed to load textures." << std::endl;
        exit(1);
    }


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLuint CrosswordGame::loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    stbi_image_free(data);
    return texture;
}


void CrosswordGame::drawBackground() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);

    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void CrosswordGame::drawButtonBackground() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonBackgroundTexture);

   
    float adjustedButtonX = buttonX - Horizontaloffset;
    float adjustedButtonY = buttonY + verticalOffset;

    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((adjustedButtonX - (buttonBackgroundWidth - buttonWidth) / 2) / (windowWidth / 2.0) - 1.0,
        adjustedButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((adjustedButtonX + buttonBackgroundWidth) / (windowWidth / 2.0) - 1.0,
        adjustedButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((adjustedButtonX + buttonBackgroundWidth) / (windowWidth / 2.0) - 1.0,
        (adjustedButtonY + buttonBackgroundHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((adjustedButtonX - (buttonBackgroundWidth - buttonWidth) / 2) / (windowWidth / 2.0) - 1.0,
        (adjustedButtonY + buttonBackgroundHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void CrosswordGame::drawPlayButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);

    
    int adjustedPlayButtonY = buttonY + buttonHeight + HTPverticaloffset; 

    // Draw the Play button
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, adjustedPlayButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, adjustedPlayButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, (adjustedPlayButtonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, (adjustedPlayButtonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    
    glBindTexture(GL_TEXTURE_2D, playPropTexture);

    
    float propWidth = buttonWidth * 1.0f;
    float propHeight = buttonHeight * 1.0f;
    float propX = buttonX + (buttonWidth - propWidth) / 2.0f;
    float propY = adjustedPlayButtonY + (buttonHeight - propHeight) / 2;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    // Draw the new prop texture above the play button
   /* glBindTexture(GL_TEXTURE_2D, propTexture2);

    // Set the position for the new prop above the Play button
    float newPropWidth = 600.0f; // Width of the new prop
    float newPropHeight = 200.0f; // Height of the new prop
    float newPropX = buttonX + (buttonWidth - newPropWidth) / 2.0f; // Centered horizontally
    float newPropY = adjustedPlayButtonY + buttonHeight + 100.0f; // Slightly above the Play button

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(newPropX / (windowWidth / 2.0) - 1.0, newPropY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((newPropX + newPropWidth) / (windowWidth / 2.0) - 1.0, newPropY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((newPropX + newPropWidth) / (windowWidth / 2.0) - 1.0, (newPropY + newPropHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(newPropX / (windowWidth / 2.0) - 1.0, (newPropY + newPropHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();
    */

    glDisable(GL_TEXTURE_2D);
}


void CrosswordGame::drawProp() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, propTexture);

   
    float adjustedPlayButtonY = buttonY + buttonHeight + HTPverticaloffset; 

    // Draw the prop
    float propWidth = 700.0f; 
    float propHeight = 250.0f; 
    float centerX = buttonX + (buttonWidth / 2.0f); 
    float propX = centerX - (propWidth / 2.0f);
    float propY = adjustedPlayButtonY + 200.0f; 

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0,
        propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0,
        propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0,
        (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0,
        (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}


void CrosswordGame::drawNewProp() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, newPropTexture);

    // Calculate the position to draw the new prop above the HTP button
    int adjustedHTPButtonY = buttonY + buttonHeight + HTPverticaloffset; // Position above HTP button

    // Set the size of the new prop texture
    float newPropWidth = 300.0f;
    float newPropHeight = 150.0f;
    float centerX = buttonX + (buttonWidth / 2.0f); // Center X position of the button
    float newPropX = centerX - (newPropWidth / 2.0f);
    float newPropY = adjustedHTPButtonY - 170.0f; // Adjust Y position above the HTP button

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(newPropX / (windowWidth / 2.0) - 1.0,
        newPropY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((newPropX + newPropWidth) / (windowWidth / 2.0) - 1.0,
        newPropY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((newPropX + newPropWidth) / (windowWidth / 2.0) - 1.0,
        (newPropY + newPropHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(newPropX / (windowWidth / 2.0) - 1.0,
        (newPropY + newPropHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}



void CrosswordGame::drawHTPButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);

    // Use the original Y position of the Play button for HTP
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, buttonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, buttonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, (buttonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, (buttonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void CrosswordGame::drawEasyButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);


    int adjustedPlayButtonY = buttonY + buttonHeight + HTPverticaloffset;

    // Draw the Play button
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, adjustedPlayButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, adjustedPlayButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, (adjustedPlayButtonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, (adjustedPlayButtonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, EzPropTexture);


    float propWidth = buttonWidth * 1.0f;
    float propHeight = buttonHeight * 1.0f;
    float propX = buttonX + (buttonWidth - propWidth) / 2.0f;
    float propY = adjustedPlayButtonY + (buttonHeight - propHeight) / 2;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();


    glDisable(GL_TEXTURE_2D);
}

void CrosswordGame::drawHardButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);

    // Use the original Y position of the Play button for HTP
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, buttonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, buttonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((buttonX + buttonWidth) / (windowWidth / 2.0) - 1.0, (buttonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(buttonX / (windowWidth / 2.0) - 1.0, (buttonY + buttonHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, hardButtonTexture);
    float propWidth = buttonWidth * 1.0f;
    float propHeight = buttonHeight * 1.0f;
    float propX = buttonX + (buttonWidth - propWidth) / 2.0f;
    float propY = buttonY + (buttonHeight - propHeight) / 2;

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, (propY + propHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// New function to draw the exit button
void CrosswordGame::drawExitButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, exitButtonTexture);

    // Position for the exit button in the top right corner
    int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
    int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(exitButtonX / (windowWidth / 2.0) - 1.0, exitButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((exitButtonX + exitButtonSize) / (windowWidth / 2.0) - 1.0, exitButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((exitButtonX + exitButtonSize) / (windowWidth / 2.0) - 1.0, (exitButtonY + exitButtonSize) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(exitButtonX / (windowWidth / 2.0) - 1.0, (exitButtonY + exitButtonSize) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void CrosswordGame::drawTopLeftProp() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, topLeftPropTexture);

    // Define the position for the top-left corner (close to the top-left of the screen)
    float propWidth = 100.0f;  // Adjust size if necessary
    float propHeight = 100.0f; // Adjust size if necessary
    float propX = windowWidth / 2 - 720.0f; // Adjust positioning based on window dimensions
    float propY = windowHeight / 2 + 433.0f; // Adjust positioning based on window dimensions

    // Correct the texture coordinates by flipping them vertically
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, propY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f((propX + propWidth) / (windowWidth / 2.0) - 1.0, (propY - propHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(propX / (windowWidth / 2.0) - 1.0, (propY - propHeight) / (windowHeight / 2.0) - 1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void CrosswordGame::drawSettingsProp() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, settingsPropTexture);

    // Adjust the size and position for the settings prop
    float settingsPropWidth = 0.22f;  // Width of the prop (adjust as needed)
    float settingsPropHeight = 0.22f; // Height of the prop (adjust as needed)

    // Position the prop beside the top-left prop
    float settingsPropX = -0.8f; // Adjust the X position as needed
    float settingsPropY = 1.001f;  // Adjust the Y position to align it properly

    // Calculate the aspect ratio of the window
    float aspectRatio = (float)windowWidth / (float)windowHeight;

    // Adjust width and height based on the aspect ratio
    float adjustedWidth = settingsPropWidth / aspectRatio;
    float adjustedHeight = settingsPropHeight;

    // Begin drawing the quad with texture (fixing the inverted texture issue)
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(settingsPropX, settingsPropY);  // Top-left corner
    glTexCoord2f(1.0f, 0.0f); glVertex2f(settingsPropX + adjustedWidth, settingsPropY);  // Top-right corner
    glTexCoord2f(1.0f, 1.0f); glVertex2f(settingsPropX + adjustedWidth, settingsPropY - adjustedHeight);  // Bottom-right corner
    glTexCoord2f(0.0f, 1.0f); glVertex2f(settingsPropX, settingsPropY - adjustedHeight);  // Bottom-left corner
    glEnd();

    glDisable(GL_TEXTURE_2D);
}







void CrosswordGame::drawGrid() {
    
}

void CrosswordGame::fillGridWithWords() {
    // Example: Insert random words
    std::string words[] = { "OPENGL", "MOUSE", "GAME", "GRAPHICS" };
    // Add logic to place words randomly in the grid
    for (auto& word : words) {
        // Example: Place horizontally for simplicity
        int row = rand() % 10;
        int col = rand() % (10 - word.length());
        for (int i = 0; i < word.length(); i++) {
            grid[row][col + i] = word[i];
        }
    }
    // Fill remaining cells with random letters
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == '\0') {
                grid[i][j] = 'A' + rand() % 26; // Random letter
            }
        }
    }
}

// Display function to render scenes
void CrosswordGame::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentState == GameState::INTRO) {
        drawBackground();
        drawButtonBackground(); // Draw the button background first

        drawHTPButton();
        drawPlayButton();
        drawExitButton(); // Draw the exit button
        drawProp();
        drawNewProp();
        drawTopLeftProp();
        drawSettingsProp();
    }
    else if (currentState == GameState::GAME) {
        drawBackground();
       
        drawButtonBackground();
        drawProp();
        drawEasyButton();
        drawHardButton();
        drawTopLeftProp();
        drawSettingsProp();
        drawTopLeftProp();
        drawSettingsProp();
        drawExitButton();
    }
    else if (currentState == GameState::HOME) {
        drawBackground();
        drawExitButton();

    }

    glutSwapBuffers();
}

// Handle mouse clicks for the play button and exit button
void CrosswordGame::handleMouse(int button, int state, int x, int y) {
    if (currentState == GameState::INTRO && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int mouseX = x;
        int mouseY = windowHeight - y;  // Convert to OpenGL coordinates

        // Check if the play button is clicked
        if (mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
            mouseY >= buttonY + buttonHeight + HTPverticaloffset && mouseY <= buttonY + buttonHeight * 2 + HTPverticaloffset) {
            currentState = GameState::GAME;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

        // Check if the exit button is clicked
        int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
        int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

        if (mouseX >= exitButtonX && mouseX <= exitButtonX + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            exit(0); // Exit the application
        }
        float propWidth = 100.0f;  // Adjust size if necessary
        float propHeight = 100.0f; // Adjust size if necessary
        float propX = windowWidth / 2 - 720.0f; // Adjust positioning based on window dimensions
        float propY = windowHeight / 2 + 433.0f; // Adjust positioning based on window dimensions
        // Check if the mouse click is within the bounds of topLeftProp
        if (x >= propX && x <= propX + propWidth && y >= propY && y <= propY + propHeight) {
            currentState = GameState::HOME;
        }
    }
}


// Update function
void CrosswordGame::update() {
    glutPostRedisplay();
}

// Global game instance
std::unique_ptr<CrosswordGame> gameInstance;

// Wrapper functions for OpenGL callbacks
void displayWrapper() { gameInstance->display(); }
void mouseWrapper(int button, int state, int x, int y) { gameInstance->handleMouse(button, state, x, y); }
void idleWrapper() { gameInstance->update(); }

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("AI Crossword Game");
    glutInitWindowSize(800, 600);
    glutFullScreen();

    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    gameInstance = std::make_unique<CrosswordGame>(screenWidth, screenHeight);
    gameInstance->initOpenGL();

    glutDisplayFunc(displayWrapper);
    glutMouseFunc(mouseWrapper);
    glutIdleFunc(idleWrapper);

    glutMainLoop();
    return 0;
}