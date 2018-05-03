#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Video.h"
#include "SDL_mixer\include\SDL_mixer.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->win->SetTitle("Video Player");
	// TODO 1: Call the initialize function from the video module in the start of the scene.
	App->video->Initialize("video/sample(good).avi");
	// TODO 8: Play the music of the video using the audio module. 
	App->audio->PlayMusic("video/sample.ogg", 0.0f);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (!App->video->isVideoFinished)
	{
		App->video->GrabAVIFrame();

	}
	else 
		Mix_PauseMusic();


	if (App->input->GetKey(SDL_SCANCODE_1) && App->video->isVideoFinished) {
		App->video->Initialize("video/sample(good).avi");
		App->audio->PlayMusic("video/sample.ogg", 0.0f);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->video->CloseAVI();
	return true;
}
