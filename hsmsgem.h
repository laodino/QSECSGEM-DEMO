#ifndef HSMSGEM_H
#define HSMSGEM_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include "hsmsgemglobal.h"

/**
* @projectName   HSMSTEST
* @brief         摘要
* @author        Laodino
* @date          2021-12-16
*/
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

    //bool (*WriteSocketptr)(QByteArray message);

    /**
           * @brief DataProcess
           */
    void DataProcess(QByteArray messagedata);





    /**
       * @brief messagecount
       * 消息计数器
       */

    uint32_t messagecount =1;





};

#endif // HSMSGEM_H
