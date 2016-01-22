
#ifndef EXTENDED_PROPERTIES_H
#define EXTENDED_PROPERTIES_H

//#include <iostream>
//#include <map>
//#include <vector>
//#include <string>

#include <zsu/Properties.h>

using namespace zsu;

class  ExtendedProperties : public Properties {

	public:
		ExtendedProperties();
		
		// check and get property. Critical error OR warning if not present. return success status (boolean).
		bool checkAndGetPropertyValue( std::string , int* , bool );
		bool checkAndGetPropertyValue( std::string , double* , bool );
        bool checkAndGetPropertyValue( std::string , float* , bool );
		bool checkAndGetPropertyValue( std::string , bool* , bool );
		bool checkAndGetPropertyValue( std::string , std::string* , bool );

};

#endif
