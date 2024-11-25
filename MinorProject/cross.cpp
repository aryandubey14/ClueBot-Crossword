#include <freeglut.h> 
#include <iostream>
#include <memory>
#include <windows.h> 
#include <mmsystem.h> 
#include <string>
#pragma comment(lib, "winmm.lib") 
#include <irrKlang.h>

using namespace irrklang;

#define STB_IMAGE_IMPLEMENTATION
#include "Header1.h" 

ISoundEngine* engine; // Declare the sound engine globally


enum class GameState { INTRO, GAME, HOME ,SIXTEEN , TWENTY, HTP };
    
class CrosswordGame {
public:
    CrosswordGame(int width, int height);
    void initOpenGL();
    void display();
    void handleMouse(int button, int state, int x, int y);
    void update();

private:
    GLuint loadTexture(const char* filename);
    void drawBackground();
    void drawMap();
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
    const int MapWidth = 700;
    const int MapHeight = 850;

    int easyButtonX, easyButtonY, easyButtonWidth, easyButtonHeight;
    int hardButtonX, hardButtonY, hardButtonWidth, hardButtonHeight;


    int buttonX, buttonY, buttonWidth, buttonHeight, buttonHY ;
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
    GLuint MapTexture;


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
    buttonHY = (windowHeight - buttonHeight) / 2 - 450;
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
    settingsPropTexture = loadTexture("D:/Minor_Project/Props/u1-removebg-preview.png");
    EzPropTexture = loadTexture("D:/Minor_Project/Props/ert.png");
    MapTexture = loadTexture("D:/Minor_Project/Props/dfr.png");
    hardButtonTexture = loadTexture("D:/Minor_Project/Props/hardd.png");


    if (!backgroundTexture || !buttonTexture || !buttonBackgroundTexture || !HTPButtonTexture ||
        !exitButtonTexture || !playPropTexture || !propTexture || !newPropTexture || !topLeftPropTexture ||
        !settingsPropTexture || !hardButtonTexture || !MapTexture) {
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

void CrosswordGame::drawMap() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, MapTexture);


    float adjustedButtonX = buttonX - Horizontaloffset;
    float adjustedButtonY = buttonY + verticalOffset;


    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((adjustedButtonX - (MapWidth - buttonWidth) / 2) / (windowWidth / 2.0) - 1.0,
        adjustedButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((adjustedButtonX + MapWidth) / (windowWidth / 2.0) - 1.0,
        adjustedButtonY / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((adjustedButtonX + MapWidth) / (windowWidth / 2.0) - 1.0,
        (adjustedButtonY + MapHeight) / (windowHeight / 2.0) - 1.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((adjustedButtonX - (MapWidth - buttonWidth) / 2) / (windowWidth / 2.0) - 1.0,
        (adjustedButtonY + MapHeight) / (windowHeight / 2.0) - 1.0);
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


    float propWidth = buttonWidth * 0.8f;
    float propHeight = buttonHeight * 0.8f;
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
    float propWidth = buttonWidth * 0.8f;
    float propHeight = buttonHeight * 0.8f;
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
        
        drawSettingsProp();
        drawExitButton();
    }
    else if (currentState == GameState::HOME) {
        drawBackground();
        drawExitButton();

    }

    else if (currentState == GameState::SIXTEEN) {
        drawBackground();
        drawExitButton();
        drawTopLeftProp();
    }
    else if (currentState == GameState::TWENTY) {
        drawBackground();
        drawExitButton();
        drawTopLeftProp();
       
    }
    else if (currentState == GameState::HTP) {
        drawBackground();
        drawExitButton();
        drawTopLeftProp();
        drawMap();

    }

    glutSwapBuffers();
}

// Handle mouse clicks for the play button and exit button
void CrosswordGame::handleMouse(int button, int state, int x, int y) {
    if (currentState == GameState::GAME && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int normX = x;
        int normY = windowHeight - y;  // Convert to OpenGL coordinates
  
        // Check if the easy button is clicked
        if (normX >= buttonX && normX <= buttonX + buttonWidth &&
            normY >= buttonY + buttonHeight + HTPverticaloffset && normY <= buttonY + buttonHeight * 2 + HTPverticaloffset) {
            currentState = GameState::SIXTEEN;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

        // Check if the hard button is clicked
        if (normX >= buttonX && normX <= buttonX + buttonWidth &&
            normY >= buttonHY + buttonHeight + HTPverticaloffset && normY <= buttonHY + buttonHeight * 2 + HTPverticaloffset) {
            currentState = GameState::TWENTY;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

        int exitButtonXx = windowWidth - exitButtonSize - 1300; // 20 pixels from the right
        int exitButtonYy = windowHeight - exitButtonSize - 20;

        if (normX >= exitButtonXx && normX <= exitButtonXx + exitButtonSize &&
            normY >= exitButtonYy && normY <= exitButtonYy + exitButtonSize) {
            currentState = GameState::HTP;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

        int exitButtonXxY = windowWidth - exitButtonSize - 1400; // 20 pixels from the right

        if (normX >= exitButtonXxY && normX <= exitButtonXxY + exitButtonSize &&
            normY >= exitButtonYy && normY <= exitButtonYy + exitButtonSize) {
            currentState = GameState::INTRO;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

        // Check if the exit button is clicked
        int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
        int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

        if (normX >= exitButtonX && normX <= exitButtonX + exitButtonSize &&
            normY >= exitButtonY && normY <= exitButtonY + exitButtonSize) {
            exit(0); // Exit the application
        }
    }

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
        // Check if the htp button is clicked
        int exitButtonXx = windowWidth - exitButtonSize - 1300; // 20 pixels from the right

        if (mouseX >= exitButtonXx && mouseX <= exitButtonXx + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            currentState = GameState::HTP;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }
    }
    
    if (currentState == GameState::SIXTEEN && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int mouseX = x;
        int mouseY = windowHeight - y;  // Convert to OpenGL coordinates
        // Check if the exit button is clicked
        int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
        int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

        if (mouseX >= exitButtonX && mouseX <= exitButtonX + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            exit(0); // Exit the application
        }
        int exitButtonXxY = windowWidth - exitButtonSize - 1400; // 20 pixels from the right

        if (mouseX >= exitButtonXxY && mouseX <= exitButtonXxY + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            currentState = GameState::INTRO;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }
    }
    if (currentState == GameState::TWENTY && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int mouseX = x;
        int mouseY = windowHeight - y;  // Convert to OpenGL coordinates
        // Check if the exit button is clicked
        int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
        int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

        if (mouseX >= exitButtonX && mouseX <= exitButtonX + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            exit(0); // Exit the application
        }
        int exitButtonXxY = windowWidth - exitButtonSize - 1400; // 20 pixels from the right

        if (mouseX >= exitButtonXxY && mouseX <= exitButtonXxY + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            currentState = GameState::INTRO;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
        }

    }
    if (currentState == GameState::HTP && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int mouseX = x;
        int mouseY = windowHeight - y;  // Convert to OpenGL coordinates
        // Check if the exit button is clicked
        int exitButtonX = windowWidth - exitButtonSize - 20; // 20 pixels from the right
        int exitButtonY = windowHeight - exitButtonSize - 20; // 20 pixels from the top

        if (mouseX >= exitButtonX && mouseX <= exitButtonX + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            exit(0); // Exit the application
        }

        int exitButtonXxY = windowWidth - exitButtonSize - 1400; // 20 pixels from the right

        if (mouseX >= exitButtonXxY && mouseX <= exitButtonXxY + exitButtonSize &&
            mouseY >= exitButtonY && mouseY <= exitButtonY + exitButtonSize) {
            currentState = GameState::INTRO;  // Transition to the crossword game
            glutPostRedisplay();  // Ensure the screen refreshes after state change
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


void initSound() {
    engine = createIrrKlangDevice();
    if (!engine) {
        std::cerr << "Error initializing sound engine!" << std::endl;
        exit(-1);
    }
    engine->play2D("media/Optimistic-background-music.wav", true); // Play music in a loop
}

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
    initSound();
    glutDisplayFunc(displayWrapper);
    glutMouseFunc(mouseWrapper);
    glutIdleFunc(idleWrapper);

    glutMainLoop(); // This loop now handles rendering and input
    engine->drop(); // Cleanup sound engine (never reached due to infinite loop)
   
    return 0;
}