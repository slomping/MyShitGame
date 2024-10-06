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
#include <string.h>
static bool isDragging = false;
static glm::vec2 dragOffset;




const glm::vec2 sizeOfImg{ 420,420 };
const std::string path1 = "img1.jpg";
const std::string path2 = "img2.jpg";



struct BasicObj
{
	gl2d::Rect Rect = { }; //init default rect
	gl2d::Texture t;
	
	void initRect(gl2d::Rect rect, std::string file) {
		Rect = rect;
		std::string path = RESOURCES_PATH;
		std::string filename = file;
		std::string fullpath = path + filename;
		const char* c = fullpath.c_str();
		t.loadFromFile(c);
	}
};
struct GameData
{
	
	std::vector<BasicObj> testArr{ };
	
}gameData;

void initArr() {
	BasicObj rect1;
	rect1.initRect(gl2d::Rect{ 0,0,100,100 }, path1);
	gameData.testArr.push_back(rect1);
	BasicObj rect2;
	rect2.initRect(gl2d::Rect{ 50,50,100,100 },path2);
	gameData.testArr.push_back(rect2);
}


std::tuple<bool, int> checkIfCursorInRect(std::vector<BasicObj>& globalarr, glm::ivec2& mousePos) {
	//std::cout << "x = " << Rect.x << " y = " << Rect.y << " a = " << Rect.a << " b = " << Rect.b << " CursorPos = " << platform::getRelMousePosition().x << " " << platform::getRelMousePosition().y << std::endl;
	for (int i = 0; i <= (globalarr.size() - 1);  i++) {
		float farCx = globalarr[i].Rect.a + globalarr[i].Rect.x;
		float farCy = globalarr[i].Rect.b + globalarr[i].Rect.y;

		//std::cout << "farCx = " << farCx << " farCy = " << farCy <<" " << platform::getRelMousePosition().x << " " << platform::getRelMousePosition().y <<" rect w = " << Rect.w << std::endl;
		if (mousePos.x <= farCx && mousePos.y <= farCy && mousePos.x >= globalarr[i].Rect.x && mousePos.y >= globalarr[i].Rect.y)
		{
			return std::make_tuple(true, i);
		}
	}
	return std::make_tuple(false, 0);
}





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
	//platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));
	
	initArr();
	
	

	
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

	glm::ivec2 mousePos =  platform::getRelMousePosition();
	
	
	
	std::tuple cursorCheck = checkIfCursorInRect(gameData.testArr, mousePos);
	int i{ std::get<1>(cursorCheck) };
	if (std::get<0>(cursorCheck) && platform::isButtonHeld(platform::Button::Up))
	{
		expandRect(gameData.testArr[i].Rect, deltaTime);
	}
	if (std::get<0>(cursorCheck) && platform::isButtonHeld(platform::Button::Down))
	{
		shrinkRect(gameData.testArr[i].Rect, deltaTime);
	}




	if (std::get<0>(cursorCheck) && platform::isLMouseHeld()) {
		
		if (!isDragging) {
			// Start dragging: calculate the offset between mouse and rectangle position
			dragOffset = glm::vec2(mousePos.x, mousePos.y) - glm::vec2(gameData.testArr[i].Rect.x, gameData.testArr[i].Rect.y);
			isDragging = true;
		}
		// Update the rectangle position based on mouse movement and the drag offset
		gameData.testArr[i].Rect.x = mousePos.x - dragOffset.x;
		gameData.testArr[i].Rect.y = mousePos.y - dragOffset.y;
	}
	else {
		isDragging = false; // Stop dragging when the mouse is not held
	}
	
	
	
	
	//render rectangles
	for (int x = 0; x <= (gameData.testArr.size() - 1); x++) {
		renderer.renderRectangle(gameData.testArr[x].Rect, gameData.testArr[x].t);
	}
	
	
	

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
