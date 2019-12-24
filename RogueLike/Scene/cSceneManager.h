#pragma once
#include <map>

class cScene;
class cSceneManager final
{
public:
    MAKE_SINGLETONFUNC(cSceneManager, GetInst)
private:
    cScene* m_nowScene;
    cScene* m_nextScene;
    std::map<wstring, cScene*> m_scenes;
public:
    cSceneManager();
    ~cSceneManager();

    cScene* AddScene(const wstring& _name, cScene* _scene);
    cScene* ChangeScene(const wstring& _name);
    cScene* FindScene(const wstring& _name);
public:
    void Update();
};

#define g_Scene cScene::GetInst()