#ifndef HSMSGEMGLOBAL_H
#define HSMSGEMGLOBAL_H

#include <stdint.h>
#include <QByteArray>

struct HSMSHeadr
{
   /**
   * @brief SessionID
   * 占10bytes
   * 0000 0000 0000 0000
   * 最左边第16位代表消息发送方向，1为To host，0为To equip
   * 剩余15位唯一标识一台设备 0-32767 000 0000 0000 0000 - 111 1111 1111 1111
   */
  uint16_t SessionID;

  /**
   * @brief HeaderByte2
   * 如果PType为0，headerbyte2代表W-Bit和Stream
   * W-Bit指明发送SnFn消息时是否需要回复，0为不需要，1为需要。
   * Stream指明消息所在大类，参考SnFn
   */
  uint8_t  HeaderByte2;

  /**
   * @brief HeaderByte3
   * 指明Function号，参考SnFn
   */
  uint8_t  HeaderByte3;

  /**
   * @brief PType
   * 表示类型，0为SECSⅡ Encode
   * 其余为子标准定义或预留
   */
  uint8_t  PType;

  /**
   * @brief SType
   * Session Type
   * 0       DataMessage    指发送SnFn命令
   * 1       Select.req     选择请求
   * 2       Select.rsp     选择回复
   * 3       Deselect.req   取消选择请求
   * 4       Deselect.rsp   取消选择回复
   * 5       Linktest.req   连接测试请求，类似心跳帧
   * 6       Linktest.rsp   连接测试回复
   * 7       Reject.req     拒绝请求
   * 8       未使用
   * 9       Seperate.req   断开连接请求
   * 10      未使用
   * 11-127  子标准定义
   * 128-255 预留
   */
  uint8_t  SType;

  /**
   * @brief SystemBytes
   * 唯一标识一次信息交互
   */
  uint32_t SystemBytes;
};


/**
 * @brief The MessageInfo struct
 * 消息结构体
 */
struct HSMSMessageInfo
{
    /**
   * @brief MessageLength
   * 消息长度，4 bytes
   * 长度为消息头长度加上消息文本，最小为10，仅有消息头
   */
  uint32_t MessageLength;

  /**
   * @brief Header
   * 消息头  10bytes
   */
  HSMSHeadr Header;

  /**
   * @brief MessageText
   * 消息文本 0-n bytes
   */
  QByteArray MessageText;

};

/**
 * @brief The HSMSHeadr struct
 * 共占10bytes
 * SessionID    0-1 0为MSB,1为LSB
 * HeaderByte2  2
 * HeaderByte3  3
 * PType        4
 * SType        5
 * System Bytes 6-9
 * 详细说明见下面定义
 */







#endif // HSMSGEMGLOBAL_H
