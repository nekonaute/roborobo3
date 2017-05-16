#include "World/MovableObject.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"
#include "WorldModels/RobotWorldModel.h"


MovableObject::MovableObject( int __id ) : CircleObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(4);
}

void MovableObject::step()
{
    if ( _impulses.size() != 0 )
        move();
    
    //stepPhysicalObject();
    if ( _visible && gPhysicalObjectsRedraw == true )
    {
        registerObject();
    }
}

void MovableObject::move()
{
    // work in progress - 2017-05-16
    
    bool _hitWall = false; // SHOULD BE GLOBAL
    bool _didMove = false;
    double _desiredX = _xReal;
    double _desiredY = _yReal;
    
    if (_impulses.size() > 0)
    {
        //       printf("[DEBUG] Moving object %d\n", _id);
        double impXtot = 0, impYtot = 0, vr, vtheta, vx, vy, ux, uy;
        
        for (auto& imp : _impulses) {
            // We only want the component of the speed normal to the centers of mass
            // v: agent speed vector, u: agent-object vector
            // impulses are in polar form
            std::tie(vr, vtheta) = imp.second;
            vx = vr*cos(vtheta * M_PI / 180.0);
            vy = vr*sin(vtheta * M_PI / 180.0);
            
            if (imp.first >= gRobotIndexStartOffset)
            {
                // pushed by a robot
                Robot *robot = gWorld->getRobot(imp.first-gRobotIndexStartOffset);
                ux = _xReal - robot->getWorldModel()->getXReal();
                uy = _yReal - robot->getWorldModel()->getYReal();
            }
            else
            {
                // pushed by other object
                PhysicalObject *object = gPhysicalObjects[imp.first];
                ux = _xReal - object->getXReal();
                uy = _yReal - object->getYReal();
            }
            double sqnorm = ux*ux + uy*uy;
            double impX =(vx*ux+vy*uy)*ux/sqnorm;
            double impY = (vx*ux+vy*uy)*uy/sqnorm;
            impXtot += impX;
            impYtot += impY;
            _efforts.insert(std::tuple<int, double>(imp.first, sqrt(impX*impX+impY*impY)));
        }
        
        _desiredLinearSpeed = sqrt(impXtot*impXtot + impYtot*impYtot);
        _desiredSpeedOrientation = atan2(impYtot, impXtot) * 180 / M_PI;
        _desiredX = _xReal+impXtot;
        _desiredY = _yReal+impYtot;
        
        Sint16 newX = _desiredX; //rounded
        Sint16 newY = _desiredY;
        
        if (newX != getXCenterPixel() || newY != getYCenterPixel()) // we're going to try to move onscreen
        {
            unregisterObject();
            hide();
            
            if (canRegister(newX, newY))
            {
                _xReal = _desiredX;
                _yReal = _desiredY;
                _didMove = true;
            }
            
            if (_hitWall) { // reappear somewhere else
                registered = false;
                _visible = false;
            }
            else {
                registerObject();
            }
        }
        else // silently move offscreen by less than a pixel
        {
            _xReal = _desiredX;
            _yReal = _desiredY;
        }
        
        _impulses.clear();
    }
    
    // update integer coordinates (used for display)
    // BUG: integer-precision implies approximation in collision
    this->_xCenterPixel= (Sint16)(_xReal+0.5);
    this->_yCenterPixel = (Sint16)(_yReal+0.5);
}

void MovableObject::isTouched( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) touched by robot #" << __idAgent << std::endl;
}

void MovableObject::isWalked( int __idAgent )
{
//    if ( gVerbose && gDisplayMode <= 1)
//        std::cout << "[DEBUG] Physical object #" << this->getId() << " (round object) walked upon by robot #" << __idAgent << std::endl;
}

void MovableObject::isPushed( int __id, std::tuple<double, double> __speed )
{
        if ( gVerbose && gDisplayMode <= 1)
           std::cout << "[DEBUG] Physical object #" << this->getId() << " (movable object) pushed by robot/agent #" << __id << std::endl;
    
    if (_impulses.count(__id) == 0) {
        _impulses.insert(std::tie(__id, __speed));
    }

}

