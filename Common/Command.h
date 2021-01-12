#pragma once

#include "list.h"

class ICommand;
typedef CMyList<ICommand*> CommandList;

/************************************************************************/
/*                                ����                                  */
/************************************************************************/
class ICommand 
{
public:
    ICommand();
    virtual ~ICommand();

public:
    virtual void UnExcute() = 0; //����
    virtual void Excute() = 0; //����
};


/************************************************************************/
/*                              �������                                */
/************************************************************************/
class CCommandManager
{
public:
    CCommandManager();
    virtual ~CCommandManager();

public:
    void UnExcute(); //����
    void Excute(); //����
    void Insert(ICommand *pCommand);

protected:
    void RemoveAll(CommandList& lstCommand);

protected:
    CommandList m_lstUnExcuteCommands; //��������
    CommandList m_lstExcuteCommands; //��������
};



/************************************************************************/
/*             ʵ��(Ϊ�˼���MFC��CList,�����к�����Ϊinline)            */
/************************************************************************/
inline ICommand::ICommand()
{
}


inline ICommand::~ICommand()
{
}


inline CCommandManager::CCommandManager()
{
}


inline CCommandManager::~CCommandManager()
{
    RemoveAll(m_lstUnExcuteCommands);
    RemoveAll(m_lstExcuteCommands);
}

//����
inline void CCommandManager::UnExcute()
{
    if (!m_lstUnExcuteCommands.IsEmpty())
    {
        ICommand *lpCommand = m_lstUnExcuteCommands.GetTail();
        lpCommand->UnExcute();

        m_lstUnExcuteCommands.RemoveTail();
        m_lstExcuteCommands.AddTail(lpCommand);
    }
}

//����
inline void CCommandManager::Excute()
{
    if (!m_lstExcuteCommands.IsEmpty())
    {
        ICommand *lpCommand = m_lstExcuteCommands.GetTail();
        lpCommand->Excute();

        m_lstExcuteCommands.RemoveTail();
        m_lstUnExcuteCommands.AddTail(lpCommand);
    }
}


inline void CCommandManager::Insert(ICommand *lpCommand)
{
    m_lstUnExcuteCommands.AddTail(lpCommand);
    RemoveAll(m_lstExcuteCommands);
}


inline void CCommandManager::RemoveAll(CommandList& lstCommand)
{
    CommandList::iterator itCommand = lstCommand.begin();
    for (; itCommand != lstCommand.end(); itCommand++)
    {
        delete *itCommand;
    }
    lstCommand.RemoveAll();
}