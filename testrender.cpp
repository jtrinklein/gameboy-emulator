#include "irrlicht-1.8.1/include/irrlicht.h"
#include "irrlicht-1.8.1/include/driverChoice.h"

using namespace irr;

int main(int /*argc*/, const char** /*argv*/) {
    // ask user for driver
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    // create device

    IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(512, 384));

    if (device == 0)
        return 1; // could not create selected driver.

    device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");

    video::IVideoDriver* driver = device->getVideoDriver();
    while(device->run() && driver) {
        if (device->isWindowActive()) {
            u32 time = device->getTimer()->getTime();
                                                        
            driver->beginScene(true, true, video::SColor(255,120,102,136));
            driver->endScene();
        }
    }
    device->drop();

    return 0;
}
