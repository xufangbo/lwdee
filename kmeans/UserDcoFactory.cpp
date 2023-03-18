#include "UserDcoFactory.h"
#include "UserDco.h"
#include "KmeansMapDco.h"
#include "KmeansReduceDco.h"
#include "KmeansMain.h"
#include "MpiProcess.h"

UserDcoFactory::UserDcoFactory()
{

}

UserDcoFactory::~UserDcoFactory()
{

}

UhconnDco* UserDcoFactory::newDco(std::string& class_name) {
    std::cout << "new dco: " << class_name << std::endl;
    if( class_name == "UserDco") {
        return new UserDco();
    }
    else if( class_name == "KmeansMapDco") {
        return new KmeansMapDco();
    }
    else if( class_name == "KmeansReduceDco") {
         return new KmeansReduceDco();
    }
    else if( class_name == "KmeansMain" ) {
        return new KmeansMain();
    }    
    else if( class_name == "MpiProcess" ) {
        return new MpiProcess();
    }  
    return 0;
}