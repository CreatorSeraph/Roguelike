#pragma once
#include <vector>
#include <thread>
#include <future>//반환값을 나중에 확인할수 있게 만드는 기능을 포함(std::future, std::promiss)
#include <queue>

class cThreadPool
{
private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_funcQueue;//먼저 들어온 함수가 먼저 실행되는 자료구조를 사용하기 위함
    bool m_isPaused;//일시정지를 할 이유가 있을지 모르겠네
    bool m_isDestroy;//threadPool이 사라질때 다른함수들에 알려줄 방법이 필요하다

    mutable std::mutex m_mutex;
public:
    //threadSize : 사용할 스레드의 개수
    //기본값 : hardware_concurrency : 논리 프로세서 개수
    cThreadPool(int threadSize = std::thread::hardware_concurrency());
    ~cThreadPool();
private:
public:
    //호출할수 있는 함수이고, 반환값이 void인 함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func을 Args...로 호출할수 있는가?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func로 Args...를 호출했을때 반환되는 결과값
    void AddFunc(Func func, Args&& ... args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
        m_funcQueue.push(std::bind(func, args...));
    }

    //호출할 수 있는 함수이고, 반환값이 void가 아닌함수
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(Func func, Args&&... args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::packaged_task<func_return_type()>(std::bind(func, args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard를 사용하기 위한 스코프
            std::lock_guard<std::mutex> lock(m_mutex);//m_funcQueue는 여러스레드에서 동시에 접근할것이다. 변수를 다루기 전에 문을 닫고 혼자 사용하자
            m_funcQueue.push([newTaskPtr = std::move(newTaskPtr)]() { (newTaskPtr)(); });
        }

        return func_result;
    }
    //이미 실행중인 함수를 제외한 이후 함수들은 작동을 일시정지 한다
    inline void Pause() { m_isPaused = true; }
    //일시정지 상태를 해제한다
    inline void Resume() { m_isPaused = false; }
    //이미 실행중인 함수를 제외한 이후 함수들을 제거한다
    //void Reset();
    //이미 실행중인 함수를 detach하고 이후 함수들을 제거한다
    //void Reset();
public:
    inline bool isEmpty() const { return m_funcQueue.empty(); }
    inline bool isPaused() const { return m_isPaused; }
};

