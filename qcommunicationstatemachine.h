#ifndef QCOMMUNICATIONSTATEMACHINE_H
#define QCOMMUNICATIONSTATEMACHINE_H

#include <QStateMachine>
#include <QState>
#include <QDebug>
enum CommunicationState
{
    NOTCONNECTED,
    NOTSELECTED,
    SELECTED

};

class QCommunicationStateMachine: public QObject
{
    Q_OBJECT
public:
    QCommunicationStateMachine();

   ~QCommunicationStateMachine();

    static QCommunicationStateMachine *getinstance();

    void NotConnected2NotSelected();
    void NotSelected2NotConnect();
    void NotSelected2Selected();
    void Selected2NotSelected();
    void UnselectedT7timeout();

     CommunicationState GetCurrentState();


private:


    QStateMachine *_commstatemachine;

    QState *_notconnected;
    QState *_notselected;
    QState *_selected;

    static QCommunicationStateMachine * _qcommstatemachine;

    void EnterState();



signals:
    void notconnected2notselected();
    void notselected2notconnect();
    void notselected2selected();
    void selected2notselected();
     void  T7timeoutstatechange();

};

#endif // QCOMMUNICATIONSTATEMACHINE_H
