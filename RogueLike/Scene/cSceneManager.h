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

    cScene* AddScene(const wstring_view& _name, cScene* _scene);
    cScene* ChangeScene(const wstring_view& _name);
    cScene* FindScene(const wstring_view& _name);
};

#define g_Scene cScene::GetInst()