#include "World/PhysicalObjectFactory.h"

int PhysicalObjectFactory::_nextId = 0;

void PhysicalObjectFactory::makeObject( int type )
{
    int id = PhysicalObjectFactory::getNextId();
    
	std::string s = "";
	std::stringstream out;
	out << id;
    
    s = "physicalObject[";
	s += out.str();
	s += "].type";
	if ( gProperties.hasProperty( s ) )
	{
		convertFromString<int>(type, gProperties.getProperty( s ), std::dec);
	}
	else
	{
        if ( gVerbose )
            std::cerr << "[MISSING] PhysicalObjectFactory: object #" << id << ", type is missing. Assume type "<< gPhysicalObjectDefaultType << "." << std::endl;
        type = gPhysicalObjectDefaultType;
	}
    
    switch ( type )
    {
        case 0:
            if ( gVerbose )
                std::cout << "[INFO] Round Object created (type = " << type << ").\n";
            gPhysicalObjects.push_back( new RoundObject(id) );
            break;
        case 1:
            if ( gVerbose )
                std::cout << "[INFO] Energy Item created (type = " << type << ").\n";
            gPhysicalObjects.push_back( new EnergyItem(id) );
            break;
        case 2:
            if ( gVerbose )
                std::cout << "[INFO] Gate Object created (type = " << type << ").\n";
            gPhysicalObjects.push_back( new GateObject(id) );
            break;
        case 3:
            if ( gVerbose )
                std::cout << "[INFO] Switch Object created (type = " << type << ").\n";
            gPhysicalObjects.push_back( new SwitchObject(id) );
            break;
        // case ...: DO NOT FORGET TO UPDATE getNbOfTypes() method.
        default:
            std::cerr << "[CRITICAL] PhysicalObjectFactory: object #" << id << ", type unknown (" << type << ")" << std::endl;
            exit(-1);
    }
}

int PhysicalObjectFactory::getNbOfTypes()
{
    return 4;
}


int PhysicalObjectFactory::getNextId()
{
    int retValue = _nextId;
    _nextId++;
    return retValue;
}
