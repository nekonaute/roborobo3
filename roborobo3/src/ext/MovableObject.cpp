#include "World/MovableObject.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"
#include "WorldModels/RobotWorldModel.h"
#include "Utilities/Graphics.h"

MovableObject::MovableObject( int __id ) : RoundObject( __id ) // should only be called by PhysicalObjectFactory
{
    setType(4);
}

void MovableObject::step()
{
    double oldX = _xReal;
    double oldY = _yReal;
    
    if ( _impulses.size() != 0 )
        move();
    
    _shifting = sqrt((oldX-_desiredX)*(oldX-_desiredX) + (oldY-_desiredY)*(oldY-_desiredY));
    
    //stepPhysicalObject();
    if ( _visible && gPhysicalObjectsRedraw == true )
    {
        registerObject();
    }
}

void MovableObject::move()
{
    // work in progress - 2017-05-16
    
    _hitWall = false;
    _didMove = false;
    _desiredX = _xReal;
    _desiredY = _yReal;
    
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
            //_efforts.insert(std::tuple<int, double>(imp.first, sqrt(impX*impX+impY*impY))); // compile on macos, not on linux
            _efforts.insert(std::pair<int, double>(imp.first, sqrt(impX*impX+impY*impY))); //
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
            
            //if (_hitWall )  // trigger relocation if object hits wall -- deprecated as of 31/07/2017
            //{
            //    registered = false;
            //    _visible = false;
            //}
            //else
            registerObject();
        }
        else // silently move offscreen by less than a pixel
        {
            _xReal = _desiredX;
            _yReal = _desiredY;
        }
        
        _impulses.clear();
    }
}

bool MovableObject::canRegister()
{
    return canRegister(getXCenterPixel(), getYCenterPixel());
}

bool MovableObject::canRegister( Sint16 __x, Sint16 __y )
{
    // test shape
    for (Sint16 xColor = __x - _radius ; xColor < __x + _radius ; xColor++)
    {
        for (Sint16 yColor = __y - _radius ; yColor < __y + _radius; yColor ++)
        {
            if ( pow (xColor-__x,2) + pow (yColor - __y,2) < _radius*_radius )
            {
                Uint32 pixel = getPixel32_secured( gEnvironmentImage, xColor, yColor);
                if ( pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) {
                    // if we touched an object, tell it
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b);
                    
                    int targetIndex = (r<<16)+(g<<8)+b;
                    
                    if ( targetIndex >= gPhysicalObjectIndexStartOffset && targetIndex < gRobotIndexStartOffset && gMovableObjects)   // physical object
                    {
                        targetIndex = targetIndex - gPhysicalObjectIndexStartOffset;
                        gPhysicalObjects[targetIndex]->isPushed(_id, std::tie(_desiredLinearSpeed, _desiredSpeedOrientation));
                    } else if (targetIndex < gRobotIndexStartOffset) {
                        _hitWall = true;
                    }
                    return false; // collision!
                }
            }
        }
    }
    return true;
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
    //if ( gVerbose && gDisplayMode <= 1)
        //std::cout << "[DEBUG] Physical object #" << this->getId() << " (movable object) pushed by robot/agent #" << __id << std::endl;

    if (_impulses.count(__id) == 0) {
        //_impulses.insert(std::tie(__id, __speed));  // compiles on macos, not on linux
        _impulses.insert(std::pair<int, std::tuple<double, double>>(__id, __speed));
        //std::cout << "[DEBUG] Object registers impulse from robot #" << __id << ".\n";
    }


}

