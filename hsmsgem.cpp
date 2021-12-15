#include "hsmsgem.h"
#include "ui_hsmsgem.h"
#pragma execution_character_set("utf-8")

HSMSGEM::HSMSGEM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HSMSGEM)
{
    ui->setupUi(this);
    _socket = new QTcpSocket;
    //  listen(5000);
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
    //messagelength 4, header 10
   if(messagedata.size()<14)
   {
       qDebug()<<"消息错误:消息过短";
       return;
   }
   qDebug()<<messagedata.size();
   HSMSMessageInfo _hsmsmessageinfo={0};
   _hsmsmessageinfo.MessageLength = messagedata.left(4).toInt();
   if(_hsmsmessageinfo.MessageLength!=(messagedata.size()-4))
   {
       qDebug()<<"消息错误:消息不完整";
       return;
   }
   HSMSHeadr _hsmsheader={0};
   _hsmsheader.SessionID


}

QByteArray HSMSGEM::structToBytearry(HSMSMessageInfo _hsmsmessageinfo)
{
    QByteArray messagebytearry;
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsmessageinfo.MessageLength,8,16,QLatin1Char('0')).toUtf8()));
    qDebug()<<QByteArray::fromHex(QString("%1").arg(_hsmsmessageinfo.MessageLength,8,16,QLatin1Char('0')).toUtf8());
    messagebytearry.append(structToBytearry(_hsmsmessageinfo.Header));
    messagebytearry.append(_hsmsmessageinfo.MessageText);
    return messagebytearry;
}

QByteArray HSMSGEM::structToBytearry(HSMSHeadr _hsmsheader)
{
    //arg第二个参数filedwidth，转为16进制需要乘2
    QByteArray headerbytearry;
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.SessionID,4,16,QLatin1Char('0')).toUtf8()));
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.HeaderByte2,2,16,QLatin1Char('0')).toUtf8()));
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.HeaderByte3,2,16,QLatin1Char('0')).toUtf8()));
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.PType,2,16,QLatin1Char('0')).toUtf8()));
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.SType,2,16,QLatin1Char('0')).toUtf8()));
    headerbytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsheader.SystemBytes,8,16,QLatin1Char('0')).toUtf8()));
    return headerbytearry;
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

void HSMSGEM::SelectReq()
{
    //00 00 00 0a ff ff 00 00 00 01 00 00 00 01
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x01;
    _hsmsheader.SystemBytes = messagecount;
    _hsmsmessaginfo.Header = _hsmsheader;
    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    messagecount++;
    qDebug()<<messagebytearry;
}

void HSMSGEM::SelectRsp(uint16_t sessionid, uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 02 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=sessionid;//0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    /*HeaderByte3
     * 0 select成功
     * 1 连接和select已建立
     * 2 当前连接没有准备好select
     * 3 超过最大连接数量
     * 4-127 子标准定义
     * 128-255 预留
     * 此处要判断当前主被动及连接状态
     */
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x02;
    _hsmsheader.SystemBytes = messageid;
    _hsmsmessaginfo.Header = _hsmsheader;
    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    qDebug()<<messagebytearry;
}

void HSMSGEM::UnselectReq()
{
    //00 00 00 0a ff ff 00 00 00 03 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x03;
    _hsmsheader.SystemBytes =messagecount ;
    _hsmsmessaginfo.Header = _hsmsheader;
    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    messagecount++;
    qDebug()<<messagebytearry;
}

void HSMSGEM::UnselectRsp(uint16_t sessionid, uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 04 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;

    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=sessionid;
    _hsmsheader.HeaderByte2 = 0x00;
    /*Deselect状态
     * 0 通讯结束，deselect成功
     * 1 当前通讯没有进行select，或者之前已经deselect过
     * 2 当前sessionid仍然在使用，如果请求者想要终止通讯先发送separate.req
     * 3-127 子标准定义
     * 128-255预留
     */
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x04;
    _hsmsheader.SystemBytes = messageid;

    _hsmsmessaginfo.Header = _hsmsheader;

    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    qDebug()<<messagebytearry;
}

void HSMSGEM::LinkTestReq()
{
    //00 00 00 0a ff ff 00 00 00 05 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;

    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x05;
    _hsmsheader.SystemBytes =messagecount ;

    _hsmsmessaginfo.Header = _hsmsheader;

    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    messagecount++;
    qDebug()<<messagebytearry;
}

void HSMSGEM::LinkTestRsp(uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 06 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;

    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x06;
    _hsmsheader.SystemBytes =messageid;

    _hsmsmessaginfo.Header = _hsmsheader;

    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    qDebug()<<messagebytearry;
}

void HSMSGEM::RejectReq(uint16_t sessionid, uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 07 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;

    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=sessionid;
    /*Ptype不支持，为被拒绝消息的Ptype
     * Stype不支持，为被拒消息的Stype
     */
    _hsmsheader.HeaderByte2 = 0x00;
    /*拒绝原因
     * 1 Stype不支持
     * 2 Ptype不支持
     * 3 通讯未打开，收到没有匹配的请求消息的回复消息
     * 4 未Selected 收到data
     * 4-127 子标准定义
     * 128-255 预留
     */
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x07;
    _hsmsheader.SystemBytes =messageid;

    _hsmsmessaginfo.Header = _hsmsheader;

    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    qDebug()<<messagebytearry;

}

void HSMSGEM::SeparateReq()
{
    //00 00 00 0a ff ff 00 00 00 09 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;

    HSMSHeadr _hsmsheader={0};
    _hsmsheader.SessionID=0xFFFF;
    _hsmsheader.HeaderByte2 = 0x00;
    _hsmsheader.HeaderByte3 = 0x00;
    _hsmsheader.PType = 0x00;
    _hsmsheader.SType = 0x09;
    _hsmsheader.SystemBytes =messagecount;

    _hsmsmessaginfo.Header = _hsmsheader;

    QByteArray messagebytearry=structToBytearry(_hsmsmessaginfo);
    WriteSocket(messagebytearry);
    messagecount++;
    qDebug()<<messagebytearry;
}

void HSMSGEM::on_pushButton_clicked()
{
    SelectReq();
}
