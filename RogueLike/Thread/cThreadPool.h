#pragma once
#include <vector>
#include <thread>
#include <future>//��ȯ���� ���߿� Ȯ���Ҽ� �ְ� ����� ����� ����(std::future, std::promiss)
#include <queue>

class cThreadPool
{
protected:
    using voidFunc = std::function<void()>;
    using costType = unsigned int;
    struct sPriorityFunction
    {
    public:
        std::function<void()> m_func;
        costType m_cost;
    public:
        bool operator<(const sPriorityFunction& _other) const
        {
            return m_cost > _other.m_cost;//cost�� ū�༮�� ������ ���� �ؾ��Ѵ�.
        }
        void operator()() const
        {
            m_func();
        }
    public:
        sPriorityFunction(voidFunc _func, costType _cost = 0)
            : m_func(std::move(_func)), m_cost(_cost)
        {
        }
        sPriorityFunction(const sPriorityFunction& _other) = default;
        sPriorityFunction(sPriorityFunction&& _other) = default;
        sPriorityFunction& operator=(const sPriorityFunction& _other)
        {
            this->m_cost = _other.m_cost;
            this->m_func = _other.m_func;

            return *this;
        }
    };
    std::vector<std::thread> m_threads;
    std::priority_queue<sPriorityFunction> m_funcQueue;
    bool m_isPaused;
    bool m_isDestroy;

    std::condition_variable m_convar;
    mutable std::mutex m_mutex;
public:
    //threadSize : ����� �������� ����
    //�⺻�� : hardware_concurrency : �� ���μ��� ����
    cThreadPool(int threadSize = std::thread::hardware_concurrency());
    ~cThreadPool();
public:
    //ȣ���Ҽ� �ִ� �Լ��̰�, ��ȯ���� void�� �Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func�� Args...�� ȣ���Ҽ� �ִ°�?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func�� Args...�� ȣ�������� ��ȯ�Ǵ� �����
        void AddFunc(costType _cost, Func _func, Args&& ... _args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
        m_funcQueue.emplace(std::bind(_func, _args...), _cost);

        m_convar.notify_one();
    }
    //ȣ���Ҽ� �ִ� �Լ��̰�, ��ȯ���� void�� �Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func�� Args...�� ȣ���Ҽ� �ִ°�?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func�� Args...�� ȣ�������� ��ȯ�Ǵ� �����
    void AddFunc(Func _func, Args&& ... _args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
        m_funcQueue.emplace(std::bind(_func, _args...));

        m_convar.notify_one();
    }

    //ȣ���� �� �ִ� �Լ��̰�, ��ȯ���� void�� �ƴ��Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(costType _cost, Func _func, Args&&... _args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::make_shared<std::packaged_task<func_return_type()>>(std::bind(_func, _args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard�� ����ϱ� ���� ������
            std::lock_guard<std::mutex> lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
            m_funcQueue.emplace([newTaskPtr]() { (*newTaskPtr)(); }, _cost);
        }

        m_convar.notify_one();

        return func_result;
    }

    //ȣ���� �� �ִ� �Լ��̰�, ��ȯ���� void�� �ƴ��Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(Func _func, Args&&... _args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::make_shared<std::packaged_task<func_return_type()>>(std::bind(_func, _args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard�� ����ϱ� ���� ������
            std::lock_guard lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
            m_funcQueue.emplace([newTaskPtr]() { (*newTaskPtr)(); });
        }

        m_convar.notify_one();

        return func_result;
    }
    //�̹� �������� �Լ��� ������ ���� �Լ����� �۵��� �Ͻ����� �Ѵ�
    void Pause() { m_isPaused = true; }
    //�Ͻ����� ���¸� �����Ѵ�
    void Resume() { m_isPaused = false; }
    //�̹� �������� �Լ��� detach�ϰ� ���� �Լ����� �����Ѵ�
    void Reset();
    size_t GetRemainFuncCount() { return m_funcQueue.size(); }
public:
    bool isEmpty() const { return m_funcQueue.empty(); }
    bool isPaused() const { return m_isPaused; }
};

