#pragma once

class cObject;
class cComponent
{
    friend cObject;
protected:
    const cObject* m_obj;
    //여기에 멤버변수를 기재
public:
    cComponent();
    virtual ~cComponent();

    cComponent(const cComponent& other) = delete;
    cComponent(cComponent&& other) = delete;
protected:
    void SetObject(cObject* _obj);

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void OnUpdate() {}
};

