#include "hsmsgem.h"
#include "ui_hsmsgem.h"
#pragma execution_character_set("utf-8")

HSMSGEM::HSMSGEM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HSMSGEM)
{
    ui->setupUi(this);
    _socket = new QTcpSocket;
    listen(5000);
}

HSMSGEM::~HSMSGEM()
{
    delete ui;
    _tcpserver->deleteLater();
    _socket->deleteLater();
}

bool HSMSGEM::listen(quint16 port)
{
    _tcpserver= new QTcpServer(this);
    if(!_tcpserver->listen(QHostAddress::AnyIPv4, port))
    {
        qDebug()<<_tcpserver->errorString();    //错误信息
        return false;
    }
    connect(_tcpserver,&QTcpServer::newConnection,this,&HSMSGEM::OneNewConnection);
    qDebug()<<"开始监听端口";
    qDebug()<<"IP地址:"<<_tcpserver->serverAddress().toString();
    qDebug()<<"端口号:"<<QString::number(_tcpserver->serverPort());
    return true;
}

bool HSMSGEM::Connect2Server(QHostAddress ipaddress,quint16 port)
{
    if(_socket!=NULL)
    {
        _socket->connectToHost(ipaddress,port);
        if(!_socket->waitForConnected(30000))
        {
            connect(_socket,&QTcpSocket::readyRead,this,&HSMSGEM::ReadData);
            connect(_socket,&QTcpSocket::disconnected,this,&HSMSGEM::ClientDisconnect);
            return true;
        }
    }
    return false;
}

void HSMSGEM::OneNewConnection()
{
    _socket = _tcpserver->nextPendingConnection();
    qDebug()<<"新客户端连接";
    qDebug()<<"IP地址:"<<_socket->peerAddress();
    qDebug()<<"端口号:"<<QString::number(_socket->peerPort());
    connect(_socket,&QTcpSocket::readyRead,this,&HSMSGEM::ReadData);
    connect(_socket,&QTcpSocket::disconnected,this,&HSMSGEM::ClientDisconnect);
}

void HSMSGEM::ReadData()
{
    QByteArray data(_socket->readAll());
    if(!data.isEmpty())
    {
        DataProcess(data);
    }

}

void HSMSGEM::ClientDisconnect()
{
    qDebug()<<"clientdisconnect";
}

void HSMSGEM::DataProcess(QByteArray messagedata)
{
    bool whethervalid =true;
    HSMSMessageInfo newmessage =  BytearrayToStruct(messagedata,whethervalid);
    if(!whethervalid)
        return ;
    switch(newmessage.SType)
    {
    case DATA:
        //DECODE SnFn;
        break;
    case SELECTREQ:
        //selectrsp
        WriteSocket(SelectRsp(newmessage.SessionID,COMMUNICATIONESTABLISHED,newmessage.SystemBytes));
        break;
    case SELECTRSP:
        //maybe test the link
        break;
    case UNSELECTREQ:
        break;
        WriteSocket(UnselectRsp(newmessage.SessionID,COMMUNICATIONENDED,newmessage.SystemBytes));
        break;
    case UNSELECTRSP:
        break;
    case LINKTESTREQ:
        //testling
        WriteSocket(LinkTestRsp(newmessage.SystemBytes));
        break;
    case LINKTESTRSP:
        break;
    case REJECTREQ:
        break;
    case SEPARATEREQ:
        break;
    default:
        WriteSocket(RejectReq(newmessage.SessionID,newmessage.SType,0x01,newmessage.SystemBytes));
        //invalidmessage
    }

}



bool HSMSGEM::WriteSocket(QByteArray message)
{
    if(_socket!=NULL)
    {
        if(_socket->write(message)!=-1)
        {
            _socket->flush();
            return true;
        }
    }
    return false;
}



void HSMSGEM::on_pushButton_clicked()
{
    //qDebug()<<MessageType::DATA;
    //SelectReq(0x00000001);
    // BytearrayToStruct("");
}
