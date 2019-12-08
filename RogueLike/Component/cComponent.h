#pragma once

class cObject;
class cComponent
{
    friend cObject;
protected:
    const cObject* m_obj;
    //���⿡ ��������� ����
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

