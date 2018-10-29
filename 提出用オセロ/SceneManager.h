#pragma once
#include <string>
#include <unordered_map>

// 前方宣言
class Scene;

class SceneManager
{
private:
	static SceneManager * s_instance;
	std::unordered_map<std::string, Scene *> m_scenes;
	Scene * m_currScene;
	Scene * m_nextScene;

private:
	SceneManager();
	~SceneManager() = default;
	SceneManager(const SceneManager &) = delete;
	SceneManager(SceneManager &&) = delete;
	void operator =(const SceneManager &) = delete;
	void operator =(SceneManager &&) = delete;

public:
	static void Startup();
	static void Shutdown();
	static SceneManager * GetInstance();

	// シーンを登録します。
	void RegisterScene(Scene * scene);

	//最初のシーンを起動する関数
	void RunFirstScene(const char * sceneName);

	//次のシーンを予約する関数
	void ReserveNextScene(const char * sceneName);

	//シーンを遷移する関数
	void TransitScene();

	//現在のシーンを描画する関数
	void RenderCurrentScene();

	//現在のシーンを更新する関数
	void UpdateCurrentScene(float deltaTime);

private:
	// シーン名を指定してシーンオブジェクトを検索します。
	Scene * GetSceneByName(const char * sceneName) const;
};