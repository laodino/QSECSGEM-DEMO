#ifndef ITEMGLOBAL_H
#define ITEMGLOBAL_H
#include "itembase.h"

enum  ItemTypes
{
    List   = 0b000000,
    Binary = 0b001000,
    Boolen = 0b001001,
    ASCII  = 0b010000,
    JIS8   = 0b010001,
    C2     = 0b010010,
    I8     = 0b011000,
    I1     = 0b011000,
    I2     = 0b011010,
    I4     = 0b011100,
    F8     = 0b100000,
    F4     = 0b100100,
    UI8    = 0b101000,
    UI1    = 0b101001,
    UI2    = 0b101010,
    UI4    = 0b101100
};
namespace ItemTypeSpace
{

/**itemstruct
 *  header   1  000000  00
 *             itemtype 长度的字节数 比如1 代表下面1行为item的长度，长度代表itembody的长度
 *  length   2  00000000   长度
 *  length   3  00000000  如果为2
 *  length   4  00000000  如果为3
 *           5 itembody
 *           6 body
 * 实际item长度为 数据长度加上2，3，4
 */


class List:ItemBase
{
public:
    List() {}
     void ShowItemname();
};

class  Binary:ItemBase
{
public:
     Binary() {}
     void ShowItemname();
};

class Boolen:ItemBase
{
public:
    Boolen() {}
    void ShowItemname();
};

class ASCII:ItemBase
{
public:
    ASCII() {}
    void ShowItemname();
};

class JIS8:ItemBase
{
public:
    JIS8() {}
    void ShowItemname();
};

class C2:ItemBase
{
public:
    C2() {}
    void ShowItemname();
};

class I8:ItemBase
{
public:
    I8() {}
    void ShowItemname();
};

class I1:ItemBase
{
public:
    I1() {}
    void ShowItemname();
};
class I2:ItemBase
{
public:
    I2() {}
    void ShowItemname();
};

class I4:ItemBase
{
public:
    I4() {}
    void ShowItemname();
};

class F8:ItemBase
{
public:
    F8() {}
    void ShowItemname();
};

class F4:ItemBase
{
public:
    F4() {}
    void ShowItemname();
};

class UI8:ItemBase
{
public:
    UI8() {}
    void ShowItemname();
};

class UI1:ItemBase
{
public:
    UI1() {}
    void ShowItemname();
};

class UI2:ItemBase
{
public:
    UI2() {}
    void ShowItemname();
};

class UI4:ItemBase
{
public:
    UI4() {}
    void ShowItemname();
};


}



#endif // ITEMGLOBAL_H
