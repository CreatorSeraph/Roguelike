#pragma once

class cObject;
class cComponentManager;
class cComponent
{
    friend cObject;
    friend cComponentManager;
protected:
    cObject* m_obj;
    //여기에 멤버변수를 기재
    bool m_isWillDelete;
    bool m_isActive;
public:
    cComponent();
    virtual ~cComponent();

    cComponent(const cComponent& other) = delete;
    cComponent(cComponent&& other) = delete;
public:
    bool IsActive() { return m_isActive; }
    void SetActive(bool _active) { m_isActive = _active; }
    bool IsWillDelete() { return m_isWillDelete; }
    void Delete() { m_isWillDelete = true; }
protected:
    void SetObject(cObject* _obj) { m_obj = _obj; }

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void OnUpdate() {}
    virtual void OnLateUpdate() {}
};

