#include "SceneManager.h"
#include "Scene.h"
#include <cassert>

SceneManager * SceneManager::s_instance = nullptr;

SceneManager::SceneManager()
	: m_currScene(nullptr)
	, m_nextScene(nullptr)
{
}


void SceneManager::Startup()
{
	assert(!s_instance);
	s_instance = new SceneManager();
}


void SceneManager::Shutdown()
{
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}


SceneManager * SceneManager::GetInstance()
{
	return s_instance;
}


void SceneManager::RegisterScene(Scene * scene)
{
	m_scenes.insert({ scene->GetSceneName(), scene });
	scene->OnCreate();
}


void SceneManager::RunFirstScene(const char * sceneName)
{
	ReserveNextScene(sceneName);
}


void SceneManager::ReserveNextScene(const char * sceneName)
{
	Scene * scene = GetSceneByName(sceneName);
	assert(scene);
	m_nextScene = scene;
}


Scene * SceneManager::GetSceneByName(const char * sceneName) const
{
	auto found = m_scenes.find(sceneName);
	if (found != m_scenes.end())
	{
		return (*found).second;
	}
	return nullptr;
}



void SceneManager::TransitScene()
{
	if (m_currScene != m_nextScene)
	{
		// 現在のシーンを終了させる
		if (m_currScene)
			m_currScene->OnStop();

		//次のシーンを初期化する
		m_nextScene->OnStart();

		//次のシーンに遷移する
		m_currScene = m_nextScene;
	}
}


void SceneManager::RenderCurrentScene()
{
	if (m_currScene)
		m_currScene->OnRender();
}


void SceneManager::UpdateCurrentScene(float deltaTime)
{
	if (m_currScene)
		m_currScene->OnUpdate(deltaTime);
}
