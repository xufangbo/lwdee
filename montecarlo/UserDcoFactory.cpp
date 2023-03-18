#include "UserDcoFactory.h"
#include "montecarloMap.h"
#include "montecarloReduce.h"

UserDcoFactory::UserDcoFactory()
{

}

UserDcoFactory::~UserDcoFactory()
{

}

UhconnDco* UserDcoFactory::newDco(std::string& class_name) {
    if( class_name == "montecarloMap") {
        return new montecarloMap();
    }
    else if( class_name == "montecarloReduce") {
        return new montecarloReduce();
    }
    
    return 0;
}