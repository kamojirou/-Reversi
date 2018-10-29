#pragma once
#include <string>

class Scene
{
private:
	std::string m_name;
	std::string m_comment;

public:
	// シーン名をしてシーンオブジェクトを新規作成します。
	Scene(const char * name) : m_name(name) { }

	// シーン名を取得します。
	const char * GetSceneName() const { return m_name.c_str(); }

	// シーンコメントを設定します。
	void SetSceneComment(const char * comment) { m_comment = comment; }

	// シーンコメントを取得します。
	const char * GetSceneComment() const { return m_comment.c_str(); }

public:
	//シーンの初期化
	virtual void OnCreate() {}
	virtual void OnStart() {}
	//シーンの更新
	virtual void OnUpdate(float deltaTime) {}
	//シーンの描画
	virtual void OnRender() {}
	virtual void OnStop() {}
	virtual void OnDestroy() {}
};
