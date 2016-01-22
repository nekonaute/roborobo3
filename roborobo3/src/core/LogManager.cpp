/*
 * LogManager.cpp
 */

#include "Utilities/LogManager.h"
#include "RoboroboMain/roborobo.h"


LogManager::LogManager(std::ofstream __logFile)
{
    logFile = &__logFile;
	buffer = "";
}

LogManager::LogManager()
{
    logFile = &gLogFile;
}

LogManager::~LogManager()
{
}

void LogManager::write(std::string str)
{
    buffer += str;
}

void LogManager::flush()
{
    if ( ! buffer.empty() )
    {
        (*logFile) << buffer;
        buffer.clear();
    }
}

LogManager* LogManager::make_DefaultLogManager()
{
    return new LogManager();
}

void LogManager::setLogFile ( std::ofstream &__logFile )
{
    logFile = &__logFile;
}
