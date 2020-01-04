#include "pch.h"
#include "cThreadPool.h"

cThreadPool::cThreadPool(int threadSize)
    : m_isPaused(false), m_isDestroy(false)
{
    m_threads.reserve(threadSize);//�̸� vector�� ������ ��Ƶδ� ������ �Ѵ�. ���Ҵ��� �Ͼ�� �ʰ� �����Ҽ� �ִ�.
    for (int i = 0; i < threadSize; ++i)
    {
        m_threads.emplace_back([this]() {
            while (true)
            {
                std::unique_lock lock(m_mutex);
                //lock_gurad�� ������ �Ҹ��ڿ����� unlock�� ȣ�� �� �� ������ lock_guard�� unlock�� ȣ�� �� �� �ִ�.

                //�۾��� ���� ���� ������ �����¿� ���ִ´�.(cpu������ �����ֱ� ����)
                m_convar.wait(lock, [this]()
                {
                    return !m_funcQueue.empty() || m_isDestroy;
                });

                if (m_funcQueue.empty() && m_isDestroy)
                {
                    return;//���ϵ� ���� ���� Pool�� ���� �ɶ��� �Ǿ����� thread�� ���� �׸�����..
                }

                auto targetFunc = m_funcQueue.top();
                m_funcQueue.pop();//���ϴ� �Լ��� ã�Ƽ� ���� ����

                lock.unlock();//���� ���ݰ� ���ͼ� ���ؾ���

                targetFunc();
            }
        });
    }
}

cThreadPool::~cThreadPool()
{
    m_isDestroy = true;
    m_convar.notify_all();

    for (auto& iter : m_threads)
        iter.join();
}

void cThreadPool::Reset()
{
    std::lock_guard lock(m_mutex);
    //ť�� ����ִ� ���·� �ʱ�ȭ
    m_funcQueue = std::priority_queue<sPriorityFunction>();
}
