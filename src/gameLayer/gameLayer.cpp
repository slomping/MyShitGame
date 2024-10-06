#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>

gl2d::Texture t;
const glm::vec2 sizeOfImg{ 420,420 };


bool checkIfCursorInRect(gl2d::Rect& Rect) {
	std::cout << "x = " << Rect.x << " y = " << Rect.y << " a = " << Rect.a << " b = " << Rect.b << " CursorPos = " << platform::getRelMousePosition().x << " " << platform::getRelMousePosition().y << std::endl;
	float farCx = Rect.a + Rect.x;
	float farCy = Rect.b + Rect.y;
	std::cout << "farCx = " << farCx << " farCy = " << farCy <<" " << platform::getRelMousePosition().x << " " << platform::getRelMousePosition().y <<" rect w = " << Rect.w << std::endl;
	if (platform::getRelMousePosition().x <= farCx && platform::getRelMousePosition().y <= farCy && platform::getRelMousePosition().x >= Rect.x && platform::getRelMousePosition().y >= Rect.y)
	{
		std::cout << "Cursor in box";
	}
	
	return true;
}
//renderer.renderRectangle({ gameData.rectPos, 100, 100 }, t);
//void renderRectangle(const Rect transforms, const Texture texture,

struct GameData
{
	gl2d::Rect rect = {0,0,100,100};
	

}gameData;

gl2d::Renderer2D renderer;

void expandRect(gl2d::Rect& Rect, float deltaTime){
	Rect.a += deltaTime * 100;
	Rect.b += deltaTime * 100;
}
void shrinkRect(gl2d::Rect& Rect, float deltaTime) {
	Rect.a -= deltaTime * 100;
	Rect.b -= deltaTime * 100;
}
bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));
	t.loadFromFile(RESOURCES_PATH "test.jpg");
	return true;
}


//IMPORTANT NOTICE, IF YOU WANT TO SHIP THE GAME TO ANOTHER PC READ THE README.MD IN THE GITHUB
//https://github.com/meemknight/cmakeSetup
//OR THE INSTRUCTION IN THE CMAKE FILE.
//YOU HAVE TO CHANGE A FLAG IN THE CMAKE SO THAT RESOURCES_PATH POINTS TO RELATIVE PATHS
//BECAUSE OF SOME CMAKE PROGBLMS, RESOURCES_PATH IS SET TO BE ABSOLUTE DURING PRODUCTION FOR MAKING IT EASIER.

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion


	if (platform::isButtonHeld(platform::Button::A))
	{
		gameData.rect.x -= deltaTime * 100;
	}
	if (platform::isButtonHeld(platform::Button::D))
	{
		gameData.rect.x += deltaTime * 100;
	}
	if ( platform::isButtonHeld(platform::Button::W))
	{
		gameData.rect.y -= deltaTime * 100;
	}
	if (platform::isButtonHeld(platform::Button::S))
	{
		gameData.rect.y += deltaTime * 100;
	}
	if (platform::isButtonHeld(platform::Button::Up))
	{
		expandRect(gameData.rect, deltaTime);
	}
	if (platform::isButtonHeld(platform::Button::Down))
	{
		shrinkRect(gameData.rect, deltaTime);
	}
	
	
	
	if (checkIfCursorInRect(gameData.rect) && platform::isLMouseHeld()) {
		gameData.rect.x = platform::getRelMousePosition().x - gameData.rect.x;
		gameData.rect.y = platform::getRelMousePosition().y - gameData.rect.y;
	}
	renderer.renderRectangle(gameData.rect, t); 
	
	

	renderer.flush();




	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{

	//saved the data.
	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
