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
#include "group.h"
#include "texture.h"
#include "ext\gorilla-audio\ga.h"
#include "ext\gorilla-audio\gau.h"
#include <stdio.h>

// musat false -> true
// laita myˆs alle if 0, t‰m‰ korvaa nykyisen mainin (toistaiseksi kunnes fontit on luotu)
//jos ei k‰‰nny, android projun alla on "OpenAL11CoreSDK.exe" setup
#define Musat 0
#if Musat 1
static void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
{
	gc_int32* flag = (gc_int32*)(in_context);
	*flag = 1;
	ga_handle_destroy(in_handle);
}
int main(int argc, char** argv)
{
	gau_Manager* mgr;
	ga_Mixer* mixer;
	ga_Sound* sound;
	ga_Handle* handle;
	gau_SampleSourceLoop* loopSrc = 0;
	gau_SampleSourceLoop** pLoopSrc = &loopSrc;
	gc_int32 loop = 0;
	gc_int32 quit = 0;

	/* Initialize library + manager */
	gc_initialize(0);
	mgr = gau_manager_create();
	mixer = gau_manager_mixer(mgr);

	/* Create and play shared sound */
	if (!loop)
		pLoopSrc = 0;
	sound = gau_load_sound_file("testi.wav", "wav");
	handle = gau_create_handle_sound(mixer, sound, &setFlagAndDestroyOnFinish, &quit, pLoopSrc);
	ga_handle_play(handle);

	/* Bounded mix/queue/dispatch loop */
	while (!quit)
	{
		gau_manager_update(mgr);
		printf("%d / %d\n", ga_handle_tell(handle, GA_TELL_PARAM_CURRENT), ga_handle_tell(handle, GA_TELL_PARAM_TOTAL));
		gc_thread_sleep(1);
	}

	/* Clean up sound */
	ga_sound_release(sound);

	/* Clean up library + manager */
	gau_manager_destroy(mgr);
	gc_shutdown();

	return 0;
}
#endif

// Jos haluat printit p‰‰lle, t‰ss‰ 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif

#if 1

int main()
{
	#define SCREENWIDTH 960
	#define SCREENHEIGHT 540

	using namespace Engine;
	using namespace Graphics;
	using namespace Maths;
	FileUtils utils;
	// Asetetaan ikkunan parametrit
	Window window("Engine", SCREENWIDTH, SCREENHEIGHT);
	// Tausta v‰ri
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	Shader* s = new Shader("basic.vert", "basic.frag");
	Shader& shader = *s;
	shader.enable();

	// Heitet‰‰n shadereill‰ valotusta
	shader.setUniformMat2f("light_pos", vec2(4.0f, 1.5f));

	// Tehd‰‰n layeri
	TileLayer layer(&shader);

	int asd = 0; //sexiest variable in whole program

	for (float y = -9.0f; y < 9.0f; y+= 0.1f)
	{
		for (float x = -16.0f; x < 16.0f; x+= 0.1f)
		{
			layer.add(new Sprite(x, y, 0.09f, 0.09f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
			asd++;
		}
	}
	

	std::cout << "Sprite count on screen: " << asd << std::endl;

	glActiveTexture(GL_TEXTURE0);
	Texture texture("test.png");//muutin hieman textureluku koodia mutta en saanut alphaa toimimaan oikein. Ekana tulee mieleen shaderit jotka saattaa jekkuilla sen kanssa.
	texture.bind();

	shader.enable();
	shader.setUniformMat1i("tex", 0);

	TestClass* MikanTestit = new TestClass(&window,&shader); //i put test here until i decide they are ready.
	float TimeInteval = (int)((1.0f / 60.0f) * 1000);//giving deltatime tickrate; this is good until hitting under 60fps; ;
	std::chrono::time_point<std::chrono::system_clock> DeltaTime = std::chrono::system_clock::now();//start point for deltatime;
	vec2 MousePos; //initting vector that will take in coordinate updates from deltatimed loop;
	
	while (!window.closed())
	{
		window.clear();
		auto Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DeltaTime);//calculating delta
		if ((float)Delta.count() > TimeInteval) //this if statement limits program to 60fps; Only for stuff inside this loop. everything else is running freely
		{
			DeltaTime += std::chrono::milliseconds((int)TimeInteval); //adding that 
		    //xrange = abs(middle.x - point.x
			//y -||-
			//return xrange < s‰de.x && yrange < s‰dey
			MikanTestit->GetCameraMovement();//funktion that enables WASD movement within world space
			MousePos = MikanTestit->MouseWorldLocation();//Testversion for correct coordinate handling in world perspective
			MikanTestit->MouseUILocation();//Testversion for correct coordinate handling in UI perspective or screen perspective;
		}
		utils.CoutFPS();
		
		// Shaderit p‰‰lle ja valotus seuraamaan hiirt‰
		shader.enable();
		shader.setUniformMat2f("light_pos", vec2((float)(MousePos.x* 32.0f / 960.0f - 16.0f), (float)(9.0f - MousePos.y * 18.0f / 540.0f)));

		// Piirret‰‰n molemmat layerit
		layer.render();
		//layer2.render();


		window.update();
		
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

#endif

#if 0
int main()
{
	const char* filename = "test.png";

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP *dib(0);

	BYTE* bits(0);

	unsigned int width(0), height(0);

	GLuint gl_texID;

	fif = FreeImage_GetFileType(filename, 0);

	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);

	if (fif == FIF_UNKNOWN)
		return false;

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);

	if (!dib)
		return false;

	bits = FreeImage_GetBits(dib);

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	unsigned int bitsPerPixel = FreeImage_GetBPP(dib);
	unsigned int pitch = FreeImage_GetPitch(dib);

	for (int y = 0; y < height;y++)
	{
		BYTE *pixel = (BYTE*)bits;
		for (int x = 0; x < width; x++)
		{
			std::cout << +pixel[FI_RGBA_RED] << " " << +pixel[FI_RGBA_GREEN] << " " << +pixel[FI_RGBA_BLUE] << std::endl;
			pixel += 3;
		}
		bits += pitch;
	}

	FreeImage_Unload(dib);

	system("PAUSE");
	return 0;
}
#endif
