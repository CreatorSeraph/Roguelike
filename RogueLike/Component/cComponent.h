#pragma once

class cObject;
class cComponent
{
protected:
    cObject* m_obj;
    //여기에 멤버변수를 기재
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

