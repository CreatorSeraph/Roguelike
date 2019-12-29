#pragma once
#include <list>
#include <thread>
#include <mutex>

class cComponent;
class cComponentThread;
class cComponentManager
{
public:
    MAKE_SINGLETONFUNC(cComponentManager, GetInst)
protected:
    std::list<cComponent*> m_components;
    std::list<cComponent*>::iterator m_endIter;

    std::list<cComponent*> m_reservedComponents;//������ �߰���
    std::vector<cComponentThread*> m_componentThreads;
    std::size_t m_threadCount;
    std::condition_variable m_cv;
protected:

public:
    cComponentManager(size_t _threadCount = std::thread::hardware_concurrency());
    ~cComponentManager();
public:
    void AddComponent(cComponent* _component);
    //�߰������� ������Ʈ�� �߰�
    void BeforeUpdate();
    void Update();
    void LateUpdate();
    //��������� ������Ʈ�� ����
    void AfterUpdate();
};

#define COMPONENTMANAGER cDevice::GetInst()