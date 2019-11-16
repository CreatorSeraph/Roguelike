#pragma once

class cObject;
class cComponent
{
protected:
    cObject* m_obj;
    //���⿡ ��������� ����
public:
    cComponent() = default;
    ~cComponent() = default;

    cComponent(const cComponent& other) = delete;
    cComponent(cComponent&& other) = delete;
protected:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void Update();
};

