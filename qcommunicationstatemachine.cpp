#include "qcommunicationstatemachine.h"

QCommunicationStateMachine*  QCommunicationStateMachine::_qcommstatemachine = NULL;

QCommunicationStateMachine*  QCommunicationStateMachine::getinstance()
{
    if(_qcommstatemachine==NULL)
        _qcommstatemachine = new QCommunicationStateMachine();
    return _qcommstatemachine;
}



QCommunicationStateMachine::QCommunicationStateMachine()
{
    /**
   *                    1                    3
   *    -------------  ---->  ------------ ---->  --------
   *   |not connected|   2   |not selected|  4   |selected|
   *    -------------  <----  ------------ <----  --------
   *           |         5         |
   *           -----------<--------|
   * 1 tcpip通讯建立
   * 2 tcpip断开连接
   * 3 select成功
   * 4 unselected或separate
   * 5 T7时间内未进行select
   */
    //原本想只设计一个触发信号来切换状态机的状态
    //但是如果当前状态下错误触发会导致出现错误状态
    _commstatemachine = new QStateMachine();
    _notconnected = new QState();
    _notselected = new QState();
    _selected = new QState();


    _commstatemachine->addState(_notconnected);
    _commstatemachine->addState(_notselected);
    _commstatemachine->addState(_selected);

    _notconnected->addTransition(this,SIGNAL(notconnected2notselected()),_notselected);
    _notconnected->assignProperty(_commstatemachine,"state",NOTCONNECTED);
    _notconnected->assignProperty(_commstatemachine,"statename","NOTCONNECTED");

    _notselected->addTransition(this,SIGNAL(notselected2notconnect()),_notconnected);
    _notselected->addTransition(this,SIGNAL(T7timeoutstatechange()),_notconnected);
    _notselected->addTransition(this,SIGNAL(notselected2selected()),_selected);
    _notselected->assignProperty(_commstatemachine,"state",NOTSELECTED);
    _notselected->assignProperty(_commstatemachine,"statename","NOTSELECTED");

    _selected->addTransition(this,SIGNAL(selected2notselected()),_notselected);
    _selected->assignProperty(_commstatemachine,"state",SELECTED);
    _selected->assignProperty(_commstatemachine,"statename","SELECTED");

    connect(_notconnected,&QState::entered,this,&QCommunicationStateMachine::EnterState);
    connect(_notselected,&QState::entered,this,&QCommunicationStateMachine::EnterState);
    connect(_selected,&QState::entered,this,&QCommunicationStateMachine::EnterState);

    _commstatemachine->setInitialState(_notconnected);
    _commstatemachine->start();
    qDebug()<<"currentstate"<<_commstatemachine->property("state");
}

QCommunicationStateMachine::~QCommunicationStateMachine()
{
    _notconnected->deleteLater();
    _notselected->deleteLater();
    _selected->deleteLater();
}

void QCommunicationStateMachine::NotConnected2NotSelected()
{
    emit notconnected2notselected();
}

void QCommunicationStateMachine::NotSelected2NotConnect()
{
    emit notselected2notconnect();
}

void QCommunicationStateMachine::NotSelected2Selected()
{
    emit notselected2selected();
}

void QCommunicationStateMachine::Selected2NotSelected()
{
    emit selected2notselected();
}
void QCommunicationStateMachine::UnselectedT7timeout()
{
    emit T7timeoutstatechange();
}

void QCommunicationStateMachine::EnterState()
{
    qDebug()<<_commstatemachine->property("state")<<_commstatemachine->property("statename");
}

CommunicationState QCommunicationStateMachine::GetCurrentState()
{
    return (CommunicationState)_commstatemachine->property("state").toInt();
}

