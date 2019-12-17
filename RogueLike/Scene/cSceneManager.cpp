#include "pch.h"
#include "cSceneManager.h"
#include "cScene.h"

cSceneManager::cSceneManager()
    : m_nowScene(nullptr), m_nextScene(nullptr)
{
}

cSceneManager::~cSceneManager()
{
    for (auto iter : m_scenes)
        SAFE_DELETE(iter.second);
}

cScene* cSceneManager::AddScene(const wstring& _name, cScene* _scene)
{
    if (_scene == nullptr)
        return nullptr;
    if (m_scenes.find(_name) != m_scenes.end())
        return nullptr;

    m_scenes.emplace(_name, _scene);
    return _scene;
}

cScene* cSceneManager::ChangeScene(const wstring& _name)
{
    if (auto find = m_scenes.find(_name); find != m_scenes.end())
    {
        m_nextScene = find->second;
        return m_nextScene;
    }
    return nullptr;
}

cScene* cSceneManager::FindScene(const wstring& _name)
{
    if (auto find = m_scenes.find(_name); find != m_scenes.end())
    {
        return find->second;
    }
}

void cSceneManager::Update()
{
    if (m_nextScene)
    {
        if (m_nowScene)
        {
            m_nowScene->Release();
        }
        m_nextScene->Init();
        m_nowScene = m_nextScene;
        m_nextScene = nullptr;
    }
    else if (m_nowScene)
    {
        m_nowScene->Update();
    }
}
