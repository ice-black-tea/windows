#include <process.h>
#include "ThreadPool.h"

const int MAX_SEMAPHORE = 0x7FFFFFFF;


CThreadPool::CThreadPool()
    : m_nCount(0)
    , m_lpThread(NULL)
    , m_hEvent(NULL)
    , m_hSemaphore(NULL)
{
}


CThreadPool::~CThreadPool()
{
    Destory();
}


bool CThreadPool::Create(int nCount)
{
    bool bRet = false;

    if (nCount <= 0)
    {
        return false;
    }

    m_nCount = nCount;

    //�����˳��¼�
    m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hEvent == NULL)
    {
        goto EXIT_LABLE;
    }

    //�����ź���
    m_hSemaphore = CreateSemaphore(NULL, 0, MAX_SEMAPHORE, NULL);
    if (m_hSemaphore == NULL)
    {
        goto EXIT_LABLE;
    }

    //�߳�����
    m_lpThread = new HANDLE[m_nCount];
    if (m_lpThread == NULL)
    {
        goto EXIT_LABLE;
    }

    //�����߳�
    for (int i = 0; i < m_nCount; i++)
    {
        m_lpThread[i] = (HANDLE)_beginthreadex(
            NULL, 0, ThreadProc, this, 0, NULL);

        if (m_lpThread[i] == INVALID_HANDLE_VALUE)
        {
            m_nCount = i;
            goto EXIT_LABLE;
        }
    }

    bRet = true;

EXIT_LABLE:

    if (!bRet)
    {
        Destory();
    }

    return bRet;
}


void CThreadPool::Destory()
{
    if (m_hEvent != NULL)
    {
        //֪ͨ�߳��˳�
        SetEvent(m_hEvent);

        if (m_lpThread != NULL && m_nCount > 0)
        {
            //�ȴ��߳��˳�
            DWORD dwRet = WaitForMultipleObjects(
                m_nCount, m_lpThread, TRUE, INFINITE);

            if (dwRet != WAIT_FAILED)
            {
                for (int i = 0; i < m_nCount; i++)
                {
                    CloseHandle(m_lpThread[i]);
                }
            }
        }

        CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }

    if (m_hSemaphore != NULL)
    {
        CloseHandle(m_hSemaphore);
        m_hSemaphore = NULL;
    }

    if (m_lpThread != NULL)
    {
        delete[] m_lpThread;
        m_lpThread = NULL;
    }

    if (!m_TaskManager.IsEmpty())
    {
        m_TaskManager.Clear();
    }
}


bool CThreadPool::PostTask(CSmartPtr<IThreadTask> lpTask)
{
    m_TaskManager.Insert(lpTask);

    ReleaseSemaphore(m_hSemaphore, 1, NULL);

    return true;
}


enum
{
    ID_EVENT,
    ID_SEMAPHORE,
    OBJECT_COUNT
};


unsigned int __stdcall CThreadPool::ThreadProc(void* lpParam)
{
    CThreadPool* pThreadPool = (CThreadPool*)lpParam;

    HANDLE arySyncObj[OBJECT_COUNT] = {
        pThreadPool->m_hEvent,
        pThreadPool->m_hSemaphore
    };

    while (true)
    {
        DWORD dwRet = WaitForMultipleObjects(
            OBJECT_COUNT, arySyncObj, FALSE, INFINITE);

        //�ȴ������ʱ�˳����߳�
        if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT)
        {
            return 0;
        }

        int nIndex = dwRet - WAIT_OBJECT_0;

        //�¼����󱻴����˳��߳�
        if (nIndex == ID_EVENT)
        {
            return 0;
        }

        //ִ�в���
        CSmartPtr<IThreadTask> lpTask = pThreadPool->m_TaskManager.GetNext();

        if (lpTask != NULL)
        {
            try
            {
                lpTask->Excute();
            }
            catch (...)
            {
            	// ignore
            }
        }
    }

    return 0;
}
