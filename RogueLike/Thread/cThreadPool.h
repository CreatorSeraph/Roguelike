#pragma once
#include <vector>
#include <thread>
#include <future>//반환값을 나중에 확인할수 있게 만드는 기능을 포함(std::future, std::promiss)
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
            return m_cost > _other.m_cost;//cost가 큰녀석이 앞으로 오게 해야한다.
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
    //threadSize : 사용할 스레드의 개수
    //기본값 : hardware_concurrency : 논리 프로세서 개수
    cThreadPool(int threadSize = std::thread::hardware_concurrency());
    ~cThreadPool();
public:
    //호출할수 있는 함수이고, 반환값이 void인 함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func을 Args...로 호출할수 있는가?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func로 Args...를 호출했을때 반환되는 결과값
        void AddFunc(costType _cost, Func _func, Args&& ... _args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
        m_funcQueue.emplace(std::bind(_func, _args...), _cost);

        m_convar.notify_one();
    }
    //호출할수 있는 함수이고, 반환값이 void인 함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func을 Args...로 호출할수 있는가?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func로 Args...를 호출했을때 반환되는 결과값
    void AddFunc(Func _func, Args&& ... _args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
        m_funcQueue.emplace(std::bind(_func, _args...));

        m_convar.notify_one();
    }

    //호출할 수 있는 함수이고, 반환값이 void가 아닌함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(costType _cost, Func _func, Args&&... _args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::make_shared<std::packaged_task<func_return_type()>>(std::bind(_func, _args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard를 사용하기 위한 스코프
            std::lock_guard<std::mutex> lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
            m_funcQueue.emplace([newTaskPtr]() { (*newTaskPtr)(); }, _cost);
        }

        m_convar.notify_one();

        return func_result;
    }

    //호출할 수 있는 함수이고, 반환값이 void가 아닌함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(Func _func, Args&&... _args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::make_shared<std::packaged_task<func_return_type()>>(std::bind(_func, _args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard를 사용하기 위한 스코프
            std::lock_guard lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
            m_funcQueue.emplace([newTaskPtr]() { (*newTaskPtr)(); });
        }

        m_convar.notify_one();

        return func_result;
    }
    //이미 실행중인 함수를 제외한 이후 함수들은 작동을 일시정지 한다
    void Pause() { m_isPaused = true; }
    //일시정지 상태를 해제한다
    void Resume() { m_isPaused = false; }
    //이미 실행중인 함수를 detach하고 이후 함수들을 제거한다
    void Reset();
    size_t GetRemainFuncCount() { return m_funcQueue.size(); }
public:
    bool isEmpty() const { return m_funcQueue.empty(); }
    bool isPaused() const { return m_isPaused; }
};

