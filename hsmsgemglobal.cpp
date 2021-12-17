#include "hsmsgemglobal.h"

QByteArray   StructToBytearray(HSMSMessageInfo _hsmsmessageinfo)
{
    //arg第二个参数filedwidth，转为16进制需要乘2
    QByteArray messagebytearry;
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsmessageinfo.MessageLength,8,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg(_hsmsmessageinfo.SessionID,4,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg( _hsmsmessageinfo.HeaderByte2,2,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg( _hsmsmessageinfo.HeaderByte3,2,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg( _hsmsmessageinfo.PType,2,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg( _hsmsmessageinfo.SType,2,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(QByteArray::fromHex(QString("%1").arg( _hsmsmessageinfo.SystemBytes,8,16,QLatin1Char('0')).toUtf8()));
    messagebytearry.append(_hsmsmessageinfo.MessageText);
    qDebug()<<messagebytearry.toHex();
    return messagebytearry;
}

HSMSMessageInfo BytearrayToStruct(QByteArray messagebytearray,bool &flag)
{
    //isvalid?
    HSMSMessageInfo _HSMSMessageinfo={0};
    QDataStream mlen(messagebytearray.left(4));
    mlen>>_HSMSMessageinfo.MessageLength;

    qDebug()<<messagebytearray.size()<<_HSMSMessageinfo.MessageLength;
    if(messagebytearray.size()<14||(messagebytearray.size()-4)!=_HSMSMessageinfo.MessageLength)
        flag =false;

    QDataStream msid(messagebytearray.mid(4,2));
    msid>>_HSMSMessageinfo.SessionID;
    QDataStream mh2(messagebytearray.mid(6,1));
    mh2>>_HSMSMessageinfo.HeaderByte2;
    QDataStream mh3(messagebytearray.mid(7,1));
    mh3>>_HSMSMessageinfo.HeaderByte3;
    QDataStream mpt(messagebytearray.mid(8,1));
    mpt>>_HSMSMessageinfo.PType;
    QDataStream mst(messagebytearray.mid(9,1));
    mst>>_HSMSMessageinfo.SType;
    QDataStream msb(messagebytearray.mid(10,4));
    msb>>_HSMSMessageinfo.SystemBytes;
    _HSMSMessageinfo.MessageText = messagebytearray.mid(14);
    qDebug()<<_HSMSMessageinfo;
    return _HSMSMessageinfo;
}


QDebug operator<<(QDebug out, const HSMSMessageInfo& _HSMSMessageInfo)
{
    out <<QString("%1").arg(_HSMSMessageInfo.MessageLength,8,16,QLatin1Char('0')).toUtf8()
       <<QString("%1").arg(_HSMSMessageInfo.SessionID,4,16,QLatin1Char('0')).toUtf8()
      <<QString("%1").arg( _HSMSMessageInfo.HeaderByte2,2,16,QLatin1Char('0')).toUtf8()
     <<QString("%1").arg( _HSMSMessageInfo.HeaderByte3,2,16,QLatin1Char('0')).toUtf8()
    <<QString("%1").arg( _HSMSMessageInfo.PType,2,16,QLatin1Char('0')).toUtf8()
    <<QString("%1").arg( _HSMSMessageInfo.SType,2,16,QLatin1Char('0')).toUtf8()
    <<QString("%1").arg( _HSMSMessageInfo.SystemBytes,8,16,QLatin1Char('0')).toUtf8()
    <<_HSMSMessageInfo.MessageText;
    return out;
}





QByteArray  SelectReq( uint32_t messagecount)
{
    //00 00 00 0a ff ff 00 00 00 01 00 00 00 01
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    _hsmsmessaginfo.HeaderByte3 = 0x00;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x01;
    _hsmsmessaginfo.SystemBytes =messagecount;
    qDebug()<< _hsmsmessaginfo;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  SelectRsp(uint16_t sessionid,uint8_t selectstatus, uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 02 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000010A;

    _hsmsmessaginfo.SessionID=sessionid;//0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    /*HeaderByte3
     * 0 select成功
     * 1 连接和select已建立
     * 2 当前连接没有准备好select
     * 3 超过最大连接数量
     * 4-127 子标准定义
     * 128-255 预留
     * 此处要判断当前主被动及连接状态
     */
    _hsmsmessaginfo.HeaderByte3 = selectstatus;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x02;
    _hsmsmessaginfo.SystemBytes = messageid;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  UnselectReq( uint32_t messagecount)
{
    //00 00 00 0a ff ff 00 00 00 03 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    _hsmsmessaginfo.HeaderByte3 = 0x00;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x03;
    _hsmsmessaginfo.SystemBytes =messagecount ;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  UnselectRsp(uint16_t sessionid,uint8_t unselectstatus,uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 04 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=sessionid;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    /*Deselect状态
     * 0 通讯结束，deselect成功
     * 1 当前通讯没有进行select，或者之前已经unselect过
     * 2 当前sessionid仍然在使用，如果请求者想要终止通讯先发送separate.req
     * 3-127 子标准定义
     * 128-255预留
     */
    _hsmsmessaginfo.HeaderByte3 = unselectstatus;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x04;
    _hsmsmessaginfo.SystemBytes = messageid;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  LinkTestReq( uint32_t messagecount)
{
    //00 00 00 0a ff ff 00 00 00 05 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    _hsmsmessaginfo.HeaderByte3 = 0x00;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x05;
    _hsmsmessaginfo.SystemBytes =messagecount ;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  LinkTestRsp(uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 06 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    _hsmsmessaginfo.HeaderByte3 = 0x00;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x06;
    _hsmsmessaginfo.SystemBytes =messageid;
    QByteArray messagebytearry=   StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}

QByteArray  RejectReq(uint16_t sessionid,uint8_t hb2,uint8_t hb3, uint32_t messageid)
{
    //00 00 00 0a ff ff 00 00 00 07 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=sessionid;
    /*Ptype不支持，为被拒绝消息的Ptype
     * Stype不支持，为被拒消息的Stype
     */
    _hsmsmessaginfo.HeaderByte2 = hb2;
    /*拒绝原因
     * 1 Stype不支持
     * 2 Ptype不支持
     * 3 通讯未打开，收到没有匹配的请求消息的回复消息
     * 4 未Selected 收到data
     * 4-127 子标准定义
     * 128-255 预留
     */
    _hsmsmessaginfo.HeaderByte3 = hb3;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x07;
    _hsmsmessaginfo.SystemBytes =messageid;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;

}

QByteArray  SeparateReq( uint32_t messagecount)
{
    //00 00 00 0a ff ff 00 00 00 09 00 00 00 0?
    HSMSMessageInfo _hsmsmessaginfo={0};
    _hsmsmessaginfo.MessageLength =0x0000000A;
    _hsmsmessaginfo.SessionID=0xFFFF;
    _hsmsmessaginfo.HeaderByte2 = 0x00;
    _hsmsmessaginfo.HeaderByte3 = 0x00;
    _hsmsmessaginfo.PType = 0x00;
    _hsmsmessaginfo.SType = 0x09;
    _hsmsmessaginfo.SystemBytes =messagecount;
    QByteArray messagebytearry=  StructToBytearray(_hsmsmessaginfo);
    return messagebytearry;
}



