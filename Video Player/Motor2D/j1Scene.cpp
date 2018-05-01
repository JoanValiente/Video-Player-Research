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
	App->video->Initialize("video/sample.avi");
	introTime.Start();
	//App->win->SetTitle("Video Player");


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
	if (introTime.ReadSec() < 5.0)
	{
		App->video->GrabAVIFrame();
	}

	if (App->input->GetKey(SDL_SCANCODE_1)) {
		App->video->CloseAVI();
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
