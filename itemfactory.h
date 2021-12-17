#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <stdint.h>
#include "itemglobal.h"
#include "itembase.h"
class ItemFactory
{
public:
    ItemFactory();
    static ItemBase* CreateNewGEMItem(ItemTypes types);
};

#endif // ITEMFACTORY_H
