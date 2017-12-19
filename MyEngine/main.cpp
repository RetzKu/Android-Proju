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

// Jos haluat printit p‰‰lle, t‰ss‰ 1, jos et valitse 0
#define DEBUG 1
#if DEBUG
	#define CONSOLE(x) std::cout << x
	#define CONSOLEND(x) std::cout << x << std::endl
#else
	#define CONSOLE(x)
	#define CONSOLEND(x)
#endif

//Constructs GLFW Window publicly
Window* window = new Window("Engine", SCREENWIDTH, SCREENHEIGHT);

class Game
{
public:
	Game() {}
	~Game()
	{
		if (Layers.size() > 0)
		{
			for (TileLayer* LayerObject : Layers)
			{
				delete LayerObject;
			}
			Layers.clear();
		}
		if (Groups.size() > 0)
		{
			for (Group* GroupObject : Groups)
			{
				delete GroupObject;
			}
			Groups.clear();
		}
		delete shader;
		delete ControlFunktionality;
	}

	/*USED CLASSES FOR GAMEOBJECT*/
	Shader* shader;
	TestClass* ControlFunktionality;
	std::vector<TileLayer*> Layers;
	std::vector<Group*> Groups;


	/*FUNCTIONS THAT CONTROL UPDATES*/
	void RenderLayers()
	{
		if (Layers.size() > 0)
		{
			for (Layer* RenderableLayers : Layers)
			{
				RenderableLayers->render();
			}
		}
	}
};

/*GAME LOOP FUNKTIONS*/
void Menu();
void InitGame();
int GameLoop(Game* GameObject);

#if 1
#endif
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

	std::cout << "Window Width: " << window->getWidth() << +" Height: " << window->getHeight() << std::endl;
	Menu();

	// Tausta v‰ri
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Ladataan shaderit
	Shader* s = new Shader("basic.vert", "basic.frag");
	Shader& shader = *s;
	shader.enable();

	// Heitet‰‰n shadereill‰ valotusta
	shader.setUniformMat2f("light_pos", vec2(4.0f, 1.5f));


	// Tehd‰‰n layeri
	TileLayer layer(&shader);

	// Lasketaan montako sprite‰ ruudulle menee
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


	// Nestattu for looppi miss‰ pusketaan layeriin spritej‰
	// Y suunnassa
	// -9 ikkunan alaosa ja +9 ikkunan yl‰osa.. eli alhaalta ylˆs

	//for (float y = -9.0f; y < 9.0f; y++)
	//{
	//	// sama X suunnassa -16 to +16
	//	// luonnollisesti 16:9 kuva suhteella niin y akseli 9 ja x 16
	//	for (float x = -16.0f; x < 16.0f; x++)
	//	{
	//		// Paikka xy, koko 0.9f, j‰‰ pieni marginaali/borderi, 1.0f olisi vierivieress‰, v‰ri randomilla neliˆille
	//		// Arvotaan piirret‰‰nkˆ tekstuuria vai v‰rineliˆt‰
	//		if (rand() % 4 == 0)
	//			// Piirret‰‰n "v‰rineliˆit‰" ja v‰rit randomilla
	//			layer.add(new Sprite(x, y, 0.9f, 0.9f, Maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1), false));
	//		else
	//			// Piirret‰‰n tekstuuriarraysta randomilla jotain (meill‰ on 4 tekstuuria siell‰)
	//			layer.add(new Sprite(x, y, 0.9f, 0.9f, textures[rand() % 4], false));
	//		// Otetaan ylˆs montako sprite‰ piirret‰‰n
	//		spritecount++;
	//	}
	//}
	//Group* g = new Group(Maths::mat4::translation(Maths::vec3(-15.8f, 7.0f, 0.0f)));

	//g->add(new Sprite(0, 0, 5, 1.5f, Maths::vec4(0.3f, 0.3f, 0.3f, 0.3f), false));//lis‰t‰‰n haalea neliˆ grouppiin
	//g->add(new Label("A!k", 0.1f, 0.3f, vec4(1, 1, 1, 1))); //lis‰t‰‰n teksti sen p‰‰lle ja offsetataan groupista 0.1 ja 0.3 verran
	//Maths::mat4* tmp = g->GetMatrix();
	//*tmp = tmp->rotation(45.0f, vec3(0, 0, 1));
	//g->SetMatrix(*tmp);
	//g->Rotate(24.0f, vec3(0, 0, 1));

	//layer.add(g); //Labelin voi suoraan adata layeriin
	std::cout << "Sprite count on screen: " << spritecount << std::endl;

#define PELAAJA
	Sprite* Player = new Sprite(0, 0, 1, 1, new Texture("Pekka2.bmp"), true);
	Group* PlayerGroup = new Group(Maths::mat4::translation(Maths::vec3( 3.0f, 0.0f, 0.0f)));
	layer.add(new Sprite(.25f, .25f, 0.5f, 0.5f, vec4(1, 1, 1, 1), false));
	PlayerGroup->add(Player);
	layer.add(PlayerGroup);
	//PlayerGroup->RotateAt(vec2(0, 0), 45.0f);

#else 

	// // // // // // 
	// Paikan m‰‰rittely spriteiss‰
	//
	// Origo 0, 0
	// Vasen yl‰ kulma -16, 9
	// Vasen sein‰ keskell‰ -16, 0
	// Vasen ala kulma -16, -9
	// Keski yl‰ kohta 0, 9
	// Keski ala kohta 0, -9
	// Oikee yl‰ kulma 16, 9
}
// Oikee sein‰ keskell‰ 16, 0
// Oikee ala kulma 16, -9
// 
// Koko selitetty esim1
// // // // // // 

// // // // // //
// ESIMERKIKSI 1 100% kuva
// L‰het‰‰n piirt‰m‰‰n vasemmasta alareunasta (-16, -9)
// Piirret‰‰n koko ruudun kokoinen kuva (32, 18)
// Miksi 32 ? Menn‰‰n x suunnassa -16 ->  0 = +16
//									0 -> 16 = +16
//								   16  + 16 = 32
// Miksi 18 ? Sama homma y suunnassa
// Pit‰‰ p‰‰st‰ -9 aina +9 asti joten koko -> 18
//
// Tekstuuri ladataan ‰rr‰yst‰ ja pekka on nelj‰s ([3])
//layer.add(new Sprite(-16, -9, 32, 18, textures[3]));
// // // // // //

// // // // // //
// ESIMERKIKSI 4 25% kuvaa
//
// Vasen ala kuva
//layer.add(new Sprite(-16, -9, 16, 9, textures[0]));
// Oikee yl‰ kuva
//layer.add(new Sprite(0, 0, 16, 9, textures[1]));
// Oikea ala kuva
//layer.add(new Sprite(0, -9, 16, 9, textures[2]));
// Vasen yl‰ kuva
//layer.add(new Sprite(-16, 0, 16, 9, textures[3]));
// // // // // //

#endif

	// V‰liaikainene texture ID systeemi, n‰‰ nousee 32 asti ainakin tulevaisuudessa
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

TestClass* MikanTestit = new TestClass(window, &shader); //i put test here until i decide they are ready.
float TimeInteval = (int)((1.0f / 60.0f) * 1000);//giving deltatime tickrate; this is good until hitting under 60fps; ;
std::chrono::time_point<std::chrono::system_clock> DeltaTime = std::chrono::system_clock::now();//start point for deltatime;
vec2 MousePos; //initting vector that will take in coordinate updates from deltatimed loop;
//fps->Text = "fps";

float degrees = 0;
Group* LeftColliders = new Group(Maths::mat4::translation(Maths::vec3(10.0f, 0.0f, 0.0f)));
LeftColliders->add(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true));


Group* UpColliders = new Group(Maths::mat4::translation(Maths::vec3(0.0f, 10.0f, 0.0f)));
UpColliders->add(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true));

Group* DownColliders = new Group(Maths::mat4::translation(Maths::vec3(0, -10.0f, 0.0f)));
DownColliders->add(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true));
//Colliders->add(new Sprite(0, -2.0f, 1, 4, vec4(1, 1, 1, 1), true));

Group* RightColliders = new Group(Maths::mat4::translation(Maths::vec3(-10.0f, 0.0f, 0.0f)));
RightColliders->add(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true));

layer.add(RightColliders);
layer.add(LeftColliders);
layer.add(UpColliders);
layer.add(DownColliders);
//while (!window->closed())
//{
//	window->clear();
//	auto ms = MikanTestit->MouseWorldLocation();
//	auto Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DeltaTime);//calculating delta
//	if ((float)Delta.count() > TimeInteval) //this if statement limits program to 60fps; Only for stuff inside this loop. everything else is running freely
//	{
//		DeltaTime += std::chrono::milliseconds((int)TimeInteval); //adding that
//		
//		if (LeftColliders != nullptr)
//		{
//			degrees += 0.1f;
//			LeftColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(10 - degrees, 0.5f, 0.0f)));
//			RightColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(-10 + degrees, 0.5f, 0.0f)));
//			UpColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(.5f, 10 - degrees, 0.0f)));
//			DownColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(.5f, -10 + degrees, 0.0f)));
//
//			//Colliders->CheckCollisionWithPoint(PlayerGroup->GetCenter(Player->getPosition(),Player->getSize()));
//			//Used to rotatecamera with time
//			MikanTestit->GetCameraMovement();
//			//Used to add extra rotation to player with time
//			MikanTestit->RotatePlayerTimed(PlayerGroup, vec2(0, 0), vec2(Player->getSize().y, Player->getSize().y));
//
//			//COLLISION CHECKS
//			if (UpColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
//			if (DownColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
//			if (LeftColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
//			if (RightColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
//		}
//		PlayerGroup->GetCenter(Player->getPosition(), Player->getSize());
//		MikanTestit->RotatePlayer(PlayerGroup, vec2(0,0), vec2(Player->getSize().y , Player->getSize().y));
//		//xrange = abs(middle.x - point.x
//		//y -||-
//		//return xrange < s‰de.x && yrange < s‰dey
//		//MikanTestit->GetCameraMovement();//funktion that enables WASD movement within world space
//		MousePos = MikanTestit->MouseWorldLocation();//Testversion for correct coordinate handling in world perspective
//	}
//	if (window->isKeyTyped(GLFW_KEY_K))
//	{
//		LeftColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true), Maths::vec3(degrees, 0, 0));
//		RightColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true), Maths::vec3(-degrees, 0, 0));
//		UpColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true), Maths::vec3(0, degrees, 0));
//		DownColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true), Maths::vec3(0, -degrees, 0));
//	}
//
//	if (window->isKeyTyped(GLFW_KEY_1))
//	{
//		//hakee ladatun audio filun ja laittaa sen soimaan
//		SoundManager::get("testi")->play();
//		//asettaa gainiksi nolla eli jos taustalla soi musa 30db niin p‰‰lle tuleva ei tule 30db + 30db.
//		//voit kokeilla kommentoida gain rivin ja painella 1 nappia
//		gain == 0.0f;
//		//gainia ei voi asettaa jos mik‰‰n ei soi.
//		SoundManager::get("testi")->setGain(gain);
//	}
//	if (window->isKeyTyped(GLFW_KEY_2))
//	{
//		SoundManager::get("testi")->stop();
//	}
//	if (window->isKeyTyped(GLFW_KEY_3))
//	{
//		SoundManager::get("testi")->pause();
//	}
//	if (window->isKeyTyped(GLFW_KEY_4))
//	{
//		SoundManager::get("testi")->resume();
//
//		gain == 0.0f;
//		SoundManager::get("testi")->setGain(gain);
//	}
//	// Shaderit p‰‰lle ja valotus seuraamaan hiirt‰
//	shader.enable();
//	shader.setUniformMat2f("light_pos", vec2((float)(MousePos.x* 32.0f / window->getWidth() - 16.0f), (float)(9.0f - MousePos.y * 18.0f / window->getHeight())));
//	// Piirret‰‰n layeri
//	layer.render();
//	SoundManager::update();
//	window->update();
}

// Tuhotaan meid‰n ladatut tekstuurit
//for (int i = 0; i < 3; i++)
//	{
//		delete textures[i];
//	}
//}

void Menu()
{
	bool WaitingForAnswer = true;
	Shader* menushader = new Shader("basic.vert", "basic.frag");
	menushader->enable();
	menushader->setUniformMat2f("light_pos", vec2(4.0f, 1.5f));

	Layer* menulayer = new TileLayer(menushader);
	menulayer->add(new Label("Press Space to start", 0, 0, vec4(1, 1, 1, 1)));
	
	GLint texIDs[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	menushader->enable();
	// shader.h:ssa pieni selitys uniformeista
	menushader->setUniformMat1iv("textures", texIDs, 10);

	while (WaitingForAnswer == true)
	{
		window->clear();
		if (window->isKeyTyped(GLFW_KEY_SPACE))
		{
			//Resets everything and re-enters gameloop
			delete menulayer;
			InitGame();
		}
		if (window->isKeyTyped(GLFW_KEY_ESCAPE))
		{
			CONSOLEND("Exiting Game");
			WaitingForAnswer = false;
		}
		menulayer->render();
		window->update();
	}
}
void InitGame()
{
	Game* GameStruct = new Game();
	GameStruct->shader = new Shader("basic.vert", "basic.frag");
	GameStruct->shader->enable();
	// Heitet‰‰n shadereill‰ valotusta
	GameStruct->shader->setUniformMat2f("light_pos", vec2(4.0f, 1.5f));
	// Tehd‰‰n layeri
	GameStruct->Layers.push_back(new TileLayer(GameStruct->shader));
	GameStruct->ControlFunktionality = new TestClass(window, GameStruct->shader);
	GameLoop(GameStruct);
}
int GameLoop(Game* GameObject)
{
	using namespace Audio;
	bool Running = true;
	float gain = 0.3f;

	/*Updatable Variables*/
	Maths::vec2 MousePos;

	glClearColor(.15f, .15f, .15f, 1);
	SoundManager::add(new Sound("testi", "testi.wav"));
	if (SoundManager::get("testi")->isPlaying() == false)
	{
		SoundManager::get("testi")->loop();
	}
	else
	{
		SoundManager::get("testi")->stop();
		SoundManager::get("testi")->loop();
	}
	float degrees = 0;
	Group* LeftColliders = new Group(Maths::mat4::translation(Maths::vec3(10.0f, 0.0f, 0.0f)));


	Group* UpColliders = new Group(Maths::mat4::translation(Maths::vec3(0.0f, 10.0f, 0.0f)));

	Group* DownColliders = new Group(Maths::mat4::translation(Maths::vec3(0, -10.0f, 0.0f)));
	//Colliders->add(new Sprite(0, -2.0f, 1, 4, vec4(1, 1, 1, 1), true));

	Group* RightColliders = new Group(Maths::mat4::translation(Maths::vec3(-10.0f, 0.0f, 0.0f)));
	
	GameObject->Layers[0]->add(RightColliders);
	GameObject->Layers[0]->add(LeftColliders);
	GameObject->Layers[0]->add(UpColliders);
	GameObject->Layers[0]->add(DownColliders);

	Sprite* Player = new Sprite(0, 0, 1, 1, new Texture("Pekka2.bmp"), true);
	Group* PlayerGroup = new Group(Maths::mat4::translation(Maths::vec3(3.0f, 0.0f, 0.0f)));
	GameObject->Layers[0]->add(new Sprite(.25f, .25f, 0.5f, 0.5f, vec4(1, 1, 1, 1), false));
	PlayerGroup->add(Player);
	GameObject->Layers[0]->add(PlayerGroup);
	std::chrono::time_point<std::chrono::system_clock> DeltaTime = std::chrono::system_clock::now();//start point for deltatime;
	std::chrono::time_point<std::chrono::system_clock> DeltaTimeSeconds = std::chrono::system_clock::now();//start point for deltatime;
	float TimeInteval = (int)((1.0f / 60.0f) * 1000);//giving deltatime tickrate; this is good until hitting under 60fps; ;
	float Loops = 0;
	float Score = 0;
	while (Running)
	{
		window->clear();

		auto Delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - DeltaTime);//calculating delta
		if ((float)Delta.count() > TimeInteval) //this if statement limits program to 60fps; Only for stuff inside this loop. everything else is running freely
		{
			Loops++;
			
			DeltaTime += std::chrono::milliseconds((int)TimeInteval); //adding tha
			if (Loops >= 30-Score)
			{
				Loops = 0;
				Score += 0.1f;
				std::cout << "Score: " + std::to_string(Score) << std::endl;
				if (rand() %4 == 0)
				{
					DownColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(0.25f, 1, 1, 1), true), Maths::vec3(0, -degrees, 0));
				}
				if (rand() % 4 == 1)
				{
					RightColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(.25f, 1, 1, 1), true), Maths::vec3(-degrees, 0, 0));
				}
				if (rand() % 4 == 2)
				{
					UpColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(.25f, 1, 1, 1), true), Maths::vec3(0, degrees, 0));
				}
				if (rand() % 4 == 3)
				{
					LeftColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(.25f, 1, 1, 1), true), Maths::vec3(degrees, 0, 0));
				}
			}
			degrees += 0.1f;
			LeftColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(10 - degrees, 0.5f, 0.0f)));
			RightColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(-10 + degrees, 0.5f, 0.0f)));
			UpColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(.5f, 10 - degrees, 0.0f)));
			DownColliders->SetMatrix(Maths::mat4::translation(Maths::vec3(.5f, -10 + degrees, 0.0f)));

			//Used to rotatecamera with time
			GameObject->ControlFunktionality->GetCameraMovement();
			//Used to add extra rotation to player with time
			GameObject->ControlFunktionality->RotatePlayerTimed(PlayerGroup, vec2(0, 0), vec2(Player->getSize().y, Player->getSize().y));

			//COLLISION CHECKS
			if (UpColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
			if (DownColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
			if (LeftColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
			if (RightColliders->AABBCollisionCheck(*PlayerGroup->GetMatrix(), Player->getPosition(), Player->getSize())) { Menu(); }
			PlayerGroup->GetCenter(Player->getPosition(), Player->getSize());
			GameObject->ControlFunktionality->RotatePlayer(PlayerGroup, vec2(0, 0), vec2(Player->getSize().y, Player->getSize().y));
			//xrange = abs(middle.x - point.x
			//y -||-
			//return xrange < s‰de.x && yrange < s‰dey
			//MikanTestit->GetCameraMovement();//funktion that enables WASD movement within world space
			MousePos = GameObject->ControlFunktionality->MouseWorldLocation();//Testversion for correct coordinate handling in world perspective




		}
		if (window->isKeyTyped(GLFW_KEY_1))
		{
			//hakee ladatun audio filun ja laittaa sen soimaan
			SoundManager::get("testi")->loop();
			//asettaa gainiksi nolla eli jos taustalla soi musa 30db niin p‰‰lle tuleva ei tule 30db + 30db.
			//voit kokeilla kommentoida gain rivin ja painella 1 nappia
			gain == 0.0f;
			//gainia ei voi asettaa jos mik‰‰n ei soi.
			SoundManager::get("testi")->setGain(gain);
		}
		if (window->isKeyTyped(GLFW_KEY_2))
		{
			SoundManager::get("testi")->stop();
		}
		if (window->isKeyTyped(GLFW_KEY_3))
		{
			SoundManager::get("testi")->pause();
		}
		if (window->isKeyTyped(GLFW_KEY_4))
		{
			SoundManager::get("testi")->resume();

			gain == 0.0f;
			SoundManager::get("testi")->setGain(gain);
		}

		if (window->isKeyTyped(GLFW_KEY_K))
		{
			DownColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true), Maths::vec3(0, -degrees, 0));
		}
		if (window->isKeyTyped(GLFW_KEY_L))
		{
			RightColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true), Maths::vec3(-degrees, 0, 0));
		}
		if (window->isKeyTyped(GLFW_KEY_J))
		{
			LeftColliders->AddFromOrigin(new Sprite(0, -2, 1, 4, vec4(1, 1, 1, 1), true), Maths::vec3(degrees, 0, 0));
		}
		if (window->isKeyTyped(GLFW_KEY_I))
		{
			UpColliders->AddFromOrigin(new Sprite(-2, 0, 4, 1, vec4(1, 1, 1, 1), true), Maths::vec3(0, degrees, 0));
		}
		
		/*RENDERING UPDATES*/
		MousePos = GameObject->ControlFunktionality->MouseWorldLocation();
		GameObject->shader->enable();
		GameObject->shader->setUniformMat2f("light_pos", vec2(0,0));
		// Piirret‰‰n layeri
		//GameObject->RenderLayers();
		GameObject->Layers[0]->render();
		Audio::SoundManager::update();
		window->update();
	}
	return 0;
}
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
#endif 0
