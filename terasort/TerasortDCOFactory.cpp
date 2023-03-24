#include "TerasortDCOFactory.h"
#include "map/MapDCO.h"
#include "reduce/ReduceDCO.h"

TerasortDCOFactory::TerasortDCOFactory()
{

}

TerasortDCOFactory::~TerasortDCOFactory()
{

}

UhconnDco* TerasortDCOFactory::newDco(std::string& class_name) {
    if( class_name == "MapDCO") {
        return new MapDCO();
    }
    else if( class_name == "ReduceDCO") {
        return new ReduceDCO();
    }
    return 0;
}