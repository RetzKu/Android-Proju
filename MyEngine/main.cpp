#include "window.h"
#include <windows.h>
#include <iostream>
#include "Maths.h"
#include "fileutils.h"
#include "Shader.h"
#include "buffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "renderer2d.h"
#include "renderable2d.h"
#include "simple2drenderer.h"
#include "static_sprite.h"
#include <chrono>
#include "BatchRenderer2D.h"
#include "sprite.h"
#include <time.h>
#include "timer.h"
#include <thread>
#include "MikaTestijuttuja.h"
#include "tilelayer.h"


// Jos haluat printit p‰‰lle, t‰ss‰ 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif


int main()
{
	#define SCREENWIDTH 960
	#define SCREENHEIGHT 540

	using namespace Engine;
	using namespace Graphics;
	using namespace Maths;
	// Asetetaan ikkunan parametrit
	Window window("Engine", SCREENWIDTH, SCREENHEIGHT);
	// Tausta v‰ri
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	// Luodaan matriisi
	float x_axis = 0;
	float y_axis = 0;
	mat4 ortho = mat4::orthographic(x_axis, 16.0f, y_axis, 9.0f, -1.0f, 1.0f);

	// Ladataan 2x samanlaisia shadereita
	Shader* s = new Shader("basic.vert", "basic.frag");
	Shader* s2 = new Shader("basic.vert", "basic.frag");
	// Vaihdetaan nime‰mist‰ laiskasti koska ei jaksa muuttaa . merkint‰‰ ->
	Shader& shader = *s;
	Shader& shader2 = *s2;
	shader.enable();
	shader2.enable();

	// Heitet‰‰n shadereill‰ valotusta
	shader.setUniformMat2f("light_pos", vec2(4.0f, 1.5f));
	shader2.setUniformMat2f("light_pos", vec2(4.0f, 1.5f));

	// Tehd‰‰n layeri
	TileLayer layer(&shader);

	// Laitetaan sinne se sama ~vajaa 60k neliˆˆ
	for (float y = -9.0f; y < 9.0f; y += 0.1f)
	{
		for (float x = -16.0f; x < 16.0f; x += 0.1f)
		{
			layer.add(new Sprite(x, y, 0.09f, 0.09f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}

	// Toinen layeri
	TileLayer layer2(&shader2);
	// Yks iso neliˆ
	layer2.add(new Sprite(-2, -2, 4, 4, Maths::vec4(1, 0, 1, 1)));
	
	//TestClass* MikanTestit = new TestClass(&window,&shader);
	
	// Jatkuva aika
	Timer time;
	float timer = 0.0f;
	unsigned int frames = 0;

	//int spritelistsize = sprites.size();
	using namespace std::chrono_literals;
	float TimeInteval = 1 / 60;
	std::chrono::time_point<std::chrono::system_clock> DeltaTime = std::chrono::system_clock::now();
	while (!window.closed())
	{
		window.clear();
		/*
		vec2 MouseLoc = MikanTestit->MouseWorldLocation();
		auto Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DeltaTime);
		if ((float)Delta.count() > TimeInteval)
		{
			DeltaTime += std::chrono::milliseconds(10);
		    //xrange = abs(middle.x - point.x
			//y -||-
			//return xrange < s‰de.x && yrange < s‰dey
			MikanTestit->GetCameraMovement();
			MikanTestit->MouseWorldLocation();
			MikanTestit->MouseUILocation();
		}
		*/
		double x, y;
		window.GetMousePosition(x, y);
		// Shaderit p‰‰lle ja valotus seuraamaan hiirt‰
		shader.enable();
		shader.setUniformMat2f("light_pos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));
		shader2.enable();
		shader2.setUniformMat2f("light_pos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));

		// Pyˆritet‰‰n toisen layerin isoa neliˆta
		mat4 mat = mat4::rotation(time.elapsed() * 25.0f, vec3(0, 0, 1));
		shader2.setUniformMat4("ml_matrix", mat);

		// Piirret‰‰n molemmat layerit
		layer.render();
		layer2.render();

		window.update();
		frames++;
		if(time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d FPS\n", frames);
			frames = 0;
		}
	}
	return 0;
}

/*

// shadereitten latauksen j‰lkeen

shader.enable();
// Heitet‰‰n matriisi shaderille
shader.setUniformMat4("pr_matrix", ortho);
// Vaihdetaan kuvion paikkaa, 0 0 0 vasen alareuna
//shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));

// Esimerkki rotatesta, 45 astetta z suunnassa
//shader.setUniformMat4("ml_matrix", mat4::rotation(45.0f, vec3(0, 0, 1)));

// Luodaan uusia spritej‰
// M‰‰ritell‰‰n paikka, koko, v‰ri ja shader
//shader.setUniformMat4("ml_matrix", mat4::rotation(15.0f, vec3(0, 0, 1)));

std::vector<Renderable2D*> sprites;

srand(time(NULL));

FileUtils::LoadTextureFromFile("Pekka.png");


Sprite sprite(5, 5, 4, 4, Maths::vec4(1, 0, 1, 1));
Sprite sprite3(7, 1, 7, 2, Maths::vec4(0.2f, 0, 1, 1));
BatchRenderer2D renderer;
for (float y = 0; y < 9.0f; y += 0.05f)
{
for(float x = 0; x < 16.0f; x += 0.05f)
{
sprites.push_back(new Sprite(x, y, 0.04f, 0.04f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
}
}


// Valot shaderiin, 0.0f 0.0f on vasen alareuna
// Jos objekti on vaikka 4.0f 2.0f kokoinen, luonnollisesti valo keskell‰ 2.0f 1.0f
//shader.setUniformMat2f("light_pos", vec2(4.0f, 2.0f));
// Shaderin v‰rin vaihto
//shader.setUniformMat4f("colour", vec4(0.2f, 0.3f, 0.8f, 1.0f));


printf("Sprites: %d\n", sprites.size());


// ikkuna loopista

mat4 mat = mat4::translation(vec3(5, 5, 5));
mat = mat * mat4::rotation(time.elapsed() * 10.0f, vec3(0, 0, 1));
mat = mat * mat4::translation(vec3(-5, -5, -5));
shader.setUniformMat4("ml_matrix", mat);

renderer.begin();
for(int i = 0; i < spritelistsize; i++)
{
renderer.submit(sprites[i]); //choke point
}
renderer.end();
renderer.flush();

*/