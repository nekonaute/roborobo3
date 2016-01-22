/*
 * LogManager.h
 *
 */

#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include <fstream>
#include <iostream>

/*
 
 How to use LogManager?
 
 OPTION 1:
    with any new instance of roborobo, a gLogManager is created and available. You just have to use the two following commands: 
    (1) gLogManager.write()
    (2) optionally: gLogManager.flush() -- note that this command is called automatically after each evaluation.
 
 OPTION 2:
    You can create as many new LogManager as you want. By default they will write in the same file as gLogManager. Here is how to change this:
 
        std::string filename = "logs/test.txt";
        std::ofstream file;
 
        file.open(filename.c_str());
 
        if(!file) {
            std::cout << "[error] Cannot open log file " << std::endl;
            exit (-1);
        }
 
        LogManager *lm = new LogManager();

        lm->setLogFile(file);
 
        lm->write("all work and no play makes Jack a dull boy ");
 
        lm->flush();
 
        file.close();
 
    Some remarks on option 2: 
        (1) dont forget to reassign file target (cf. lm->setLogFile(...) )
        (2) dont forget to flush once in a while (no flush means information is not written on disk)
 */


class LogManager {

private:

	std::string buffer;
    std::ofstream *logFile;  // LogManager does not open/close. Assume it is handled elsewhere.

public:

    LogManager();
	LogManager(std::ofstream __logFile);
	virtual ~LogManager();

    static LogManager* make_DefaultLogManager(); //(std::ofstream __logFile);

    void setLogFile ( std::ofstream &__logFile );
    
	void write(std::string str);
    void flush();
};

#endif
