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
#include "Label.h"
#include "texture.h"
#include "ext\gorilla-audio\ga.h"
#include "ext\gorilla-audio\gau.h"
#include <stdio.h>
#include "sound_manager.h"

// Jos haluat printit p��lle, t�ss� 1, jos et valitse 0
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
	using namespace Audio;
	using namespace Maths;

	FileUtils utils;
	// Asetetaan ikkunan parametrit
	Window window("Engine", SCREENWIDTH, SCREENHEIGHT);
	
	std::cout << "Window Width: " << window.getWidth() << +" Height: " << window.getHeight() << std::endl;

	// Tausta v�ri
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Ladataan shaderit
	Shader* s = new Shader("basic.vert", "basic.frag");
	Shader& shader = *s;
	shader.enable();

	// Heitet��n shadereill� valotusta
	shader.setUniformMat2f("light_pos", vec2(4.0f, 1.5f));


	// Tehd��n layeri
	TileLayer layer(&shader);

	// Lasketaan montako sprite� ruudulle menee
	int spritecount = 0; 
	
	// Ladataan tekstuurit tekstuuri* arrayhyn
	Texture* textures[] =
	{
		new Texture("ta.png"),
		new Texture("tb.png"),
		new Texture("tc.png"),
		new Texture("Pekka2.bmp")
	};

#define TUTORIAL 0

#if !TUTORIAL


	// Nestattu for looppi miss� pusketaan layeriin spritej�
	// Y suunnassa
	// -9 ikkunan alaosa ja +9 ikkunan yl�osa.. eli alhaalta yl�s

	for (float y = -9.0f; y < 9.0f; y++)
	{
		// sama X suunnassa -16 to +16
		// luonnollisesti 16:9 kuva suhteella niin y akseli 9 ja x 16
		for (float x = -16.0f; x < 16.0f; x++)
		{
			// Paikka xy, koko 0.9f, j�� pieni marginaali/borderi, 1.0f olisi vierivieress�, v�ri randomilla neli�ille
			// Arvotaan piirret��nk� tekstuuria vai v�rineli�t�
			if (rand() % 4 == 0)
				// Piirret��n "v�rineli�it�" ja v�rit randomilla
				layer.add(new Sprite(x, y, 0.9f, 0.9f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
			else
				// Piirret��n tekstuuriarraysta randomilla jotain (meill� on 4 tekstuuria siell�)
				layer.add(new Sprite(x, y, 0.9f, 0.9f, textures[rand() % 4]));
			// Otetaan yl�s montako sprite� piirret��n
			spritecount++;
		}
	}
	Group* g = new Group(Maths::mat4::translation(Maths::vec3(-15.8f, 7.0f, 0.0f)));
	
	g->add(new Sprite(0, 0, 5, 1.5f, Maths::vec4(0.3f,0.3f,0.3f,0.3f)));//lis�t��n haalea neli� grouppiin
	g->add(new Label("A!k", 0.1f, 0.3f, vec4(1, 1, 1, 1))); //lis�t��n teksti sen p��lle ja offsetataan groupista 0.1 ja 0.3 verran

 	layer.add(g); //Labelin voi suoraan adata layeriin
	std::cout << "Sprite count on screen: " << spritecount << std::endl;

#else 

	// // // // // // 
	// Paikan m��rittely spriteiss�
	//
	// Origo 0, 0
	// Vasen yl� kulma -16, 9
	// Vasen sein� keskell� -16, 0
	// Vasen ala kulma -16, -9
	// Keski yl� kohta 0, 9
	// Keski ala kohta 0, -9
	// Oikee yl� kulma 16, 9
	}
	// Oikee sein� keskell� 16, 0
	// Oikee ala kulma 16, -9
	// 
	// Koko selitetty esim1
	// // // // // // 
	
	// // // // // //
	// ESIMERKIKSI 1 100% kuva
	// L�het��n piirt�m��n vasemmasta alareunasta (-16, -9)
	// Piirret��n koko ruudun kokoinen kuva (32, 18)
	// Miksi 32 ? Menn��n x suunnassa -16 ->  0 = +16
	//									0 -> 16 = +16
	//								   16  + 16 = 32
	// Miksi 18 ? Sama homma y suunnassa
	// Pit�� p��st� -9 aina +9 asti joten koko -> 18
	//
	// Tekstuuri ladataan �rr�yst� ja pekka on nelj�s ([3])
	//layer.add(new Sprite(-16, -9, 32, 18, textures[3]));
	// // // // // //

	// // // // // //
	// ESIMERKIKSI 4 25% kuvaa
	//
	// Vasen ala kuva
	//layer.add(new Sprite(-16, -9, 16, 9, textures[0]));
	// Oikee yl� kuva
	//layer.add(new Sprite(0, 0, 16, 9, textures[1]));
	// Oikea ala kuva
	//layer.add(new Sprite(0, -9, 16, 9, textures[2]));
	// Vasen yl� kuva
	//layer.add(new Sprite(-16, 0, 16, 9, textures[3]));
	// // // // // //

#endif

	// V�liaikainene texture ID systeemi, n�� nousee 32 asti ainakin tulevaisuudessa
	GLint texIDs[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	shader.enable();
	// shader.h:ssa pieni selitys uniformeista
	shader.setUniformMat1iv("textures", texIDs, 10);
	
	//ladataan uusi audiofilu
	SoundManager::add(new Sound("testi", "testi.wav"));
	//gainille parametri
	float gain = 0.3f;

	

	TestClass* MikanTestit = new TestClass(&window,&shader); //i put test here until i decide they are ready.
	float TimeInteval = (int)((1.0f / 60.0f) * 1000);//giving deltatime tickrate; this is good until hitting under 60fps; ;
	std::chrono::time_point<std::chrono::system_clock> DeltaTime = std::chrono::system_clock::now();//start point for deltatime;
	vec2 MousePos; //initting vector that will take in coordinate updates from deltatimed loop;
	//fps->Text = "fps";
	while (!window.closed())
	{
		window.clear();
		auto ms = MikanTestit->MouseWorldLocation();
		auto Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DeltaTime);//calculating delta
		if ((float)Delta.count() > TimeInteval) //this if statement limits program to 60fps; Only for stuff inside this loop. everything else is running freely
		{
			DeltaTime += std::chrono::milliseconds((int)TimeInteval); //adding that 
		    //xrange = abs(middle.x - point.x
			//y -||-
			//return xrange < s�de.x && yrange < s�dey
			MikanTestit->GetCameraMovement();//funktion that enables WASD movement within world space
			MousePos = MikanTestit->MouseWorldLocation();//Testversion for correct coordinate handling in world perspective
			MikanTestit->MouseUILocation();//Testversion for correct coordinate handling in UI perspective or screen perspective;
		}
		if (window.isKeyTyped(GLFW_KEY_1))
		{
			//hakee ladatun audio filun ja laittaa sen soimaan
			SoundManager::get("testi")->play();
			//asettaa gainiksi nolla eli jos taustalla soi musa 30db niin p��lle tuleva ei tule 30db + 30db.
			//voit kokeilla kommentoida gain rivin ja painella 1 nappia
			gain == 0.0f;
			//gainia ei voi asettaa jos mik��n ei soi.
			SoundManager::get("testi")->setGain(gain);
		}
		if (window.isKeyTyped(GLFW_KEY_2))
		{
			SoundManager::get("testi")->stop();
		}
		if (window.isKeyTyped(GLFW_KEY_3))
		{
			SoundManager::get("testi")->pause();
		}
		if (window.isKeyTyped(GLFW_KEY_4))
		{
			SoundManager::get("testi")->resume();
			
			gain == 0.0f;
			SoundManager::get("testi")->setGain(gain);
		}
		// Shaderit p��lle ja valotus seuraamaan hiirt�
		shader.enable();
		shader.setUniformMat2f("light_pos", vec2((float)(MousePos.x* 32.0f / window.getWidth() - 16.0f), (float)(9.0f - MousePos.y * 18.0f / window.getHeight())));
		// Piirret��n layeri
		layer.render();
		SoundManager::update();
		window.update();	
	}

	// Tuhotaan meid�n ladatut tekstuurit
	for (int i = 0; i < 3; i++)
	{
		delete textures[i];
	}

	return 0;
}

/*

// shadereitten latauksen j�lkeen

shader.enable();
// Heitet��n matriisi shaderille
shader.setUniformMat4("pr_matrix", ortho);
// Vaihdetaan kuvion paikkaa, 0 0 0 vasen alareuna
//shader.setUniformMat4("ml_matrix", mat4::translation(vec3(4, 3, 0)));

// Esimerkki rotatesta, 45 astetta z suunnassa
//shader.setUniformMat4("ml_matrix", mat4::rotation(45.0f, vec3(0, 0, 1)));

// Luodaan uusia spritej�
// M��ritell��n paikka, koko, v�ri ja shader
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
// Jos objekti on vaikka 4.0f 2.0f kokoinen, luonnollisesti valo keskell� 2.0f 1.0f
//shader.setUniformMat2f("light_pos", vec2(4.0f, 2.0f));
// Shaderin v�rin vaihto
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
