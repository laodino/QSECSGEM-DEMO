#ifndef HSMSGEM_H
#define HSMSGEM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include "hsmsgemglobal.h"

namespace Ui {
class HSMSGEM;
}

class HSMSGEM : public QWidget
{
    Q_OBJECT

public:
    explicit HSMSGEM(QWidget *parent = 0);
    ~HSMSGEM();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HSMSGEM *ui;

    //服务器端
    QTcpServer *_tcpserver ;

    QTcpSocket *_socket ;


    /**
      * @brief connection
      */
    bool listen(quint16 port);

    bool Connect2Server(QHostAddress ipaddress, quint16 port);

    void OneNewConnection();

    void ReadData();

    void ClientDisconnect();

    bool WriteSocket(QByteArray message);

    /**
           * @brief DataProcess
           */
    void DataProcess(QByteArray messagedata);

    QByteArray structToBytearry(HSMSMessageInfo _hsmsmessageinfo);
    QByteArray structToBytearry(HSMSHeadr _hsmsheader);


    /**
      * @brief SelectReq
      */
    void SelectReq();

    /**
     * @brief SelectRsp
     * @param sessionid
     * @param messageid
     */
    void SelectRsp(uint16_t sessionid, uint32_t messageid);

    /**
     * @brief UnselectReq
     */
    void UnselectReq();
    /**
     * @brief UnselectRsp
     * @param sessionid
     * @param messageid
     */
    void UnselectRsp(uint16_t sessionid, uint32_t messageid);


    void LinkTestReq();

    void LinkTestRsp(uint32_t messageid);

    void RejectReq(uint16_t sessionid, uint32_t messageid);

    void SeparateReq();

    /**
       * @brief messagecount
       * 消息计数器
       */

    uint32_t messagecount =1;





};

#endif // HSMSGEM_H
