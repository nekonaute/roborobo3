/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#ifndef TEMPLATEBOIDSCONTROLLER_H
#define TEMPLATEBOIDSCONTROLLER_H

#include "Controllers/Controller.h"

class RobotWorldModel;

class TemplateBoidsController : public Controller
{
private:
    int _iteration;
    void resetRobot();
    
public:
    
    TemplateBoidsController(RobotWorldModel *wm);
    ~TemplateBoidsController();
    
    void reset();
    void step();
};


#endif

