#pragma once
#include <vector>
#include <thread>
#include <future>//��ȯ���� ���߿� Ȯ���Ҽ� �ְ� ����� ����� ����(std::future, std::promiss)
#include <queue>

class cThreadPool
{
private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_funcQueue;//���� ���� �Լ��� ���� ����Ǵ� �ڷᱸ���� ����ϱ� ����
    bool m_isPaused;//�Ͻ������� �� ������ ������ �𸣰ڳ�
    bool m_isDestroy;//threadPool�� ������� �ٸ��Լ��鿡 �˷��� ����� �ʿ��ϴ�

    mutable std::mutex m_mutex;
public:
    //threadSize : ����� �������� ����
    //�⺻�� : hardware_concurrency : �� ���μ��� ����
    cThreadPool(int threadSize = std::thread::hardware_concurrency());
    ~cThreadPool();
private:
public:
    //ȣ���Ҽ� �ִ� �Լ��̰�, ��ȯ���� void�� �Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>//is_invocable_v : Func�� Args...�� ȣ���Ҽ� �ִ°�?
        && std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>//invoke_result_t : Func�� Args...�� ȣ�������� ��ȯ�Ǵ� �����
    void AddFunc(Func func, Args&& ... args)
    {
        std::lock_guard lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
        m_funcQueue.push(std::bind(func, args...));
    }

    //ȣ���� �� �ִ� �Լ��̰�, ��ȯ���� void�� �ƴ��Լ�
    template<typename Func, typename ...Args,
        typename = typename std::enable_if_t<std::is_invocable_v<Func, Args...>
        && !std::is_same_v<std::invoke_result_t<Func, Args...>, void>>>
    std::future<std::invoke_result_t<Func, Args...>> AddFunc(Func func, Args&&... args)
    {
        using func_return_type = std::invoke_result_t<Func, Args...>;

        auto newTaskPtr = std::packaged_task<func_return_type()>(std::bind(func, args...));

        std::future<func_return_type> func_result = newTaskPtr->get_future();
        {//lock_guard�� ����ϱ� ���� ������
            std::lock_guard<std::mutex> lock(m_mutex);//m_funcQueue�� ���������忡�� ���ÿ� �����Ұ��̴�. ������ �ٷ�� ���� ���� �ݰ� ȥ�� �������
            m_funcQueue.push([newTaskPtr = std::move(newTaskPtr)]() { (newTaskPtr)(); });
        }

        return func_result;
    }
    //�̹� �������� �Լ��� ������ ���� �Լ����� �۵��� �Ͻ����� �Ѵ�
    inline void Pause() { m_isPaused = true; }
    //�Ͻ����� ���¸� �����Ѵ�
    inline void Resume() { m_isPaused = false; }
    //�̹� �������� �Լ��� ������ ���� �Լ����� �����Ѵ�
    //void Reset();
    //�̹� �������� �Լ��� detach�ϰ� ���� �Լ����� �����Ѵ�
    //void Reset();
public:
    inline bool isEmpty() const { return m_funcQueue.empty(); }
    inline bool isPaused() const { return m_isPaused; }
};

