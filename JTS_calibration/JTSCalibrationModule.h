#ifndef _JTSCALIBRATIONMODULE_H_
#define _JTSCALIBRATIONMODULE_H_

#include <iostream>
#include <string>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
 
// within project includes  
#include <iCub/demoThread.h>

/**
 * here you need to document your classes.
 */
class JTSCalibrationModule : public yarp::os::RFModule {
    std::string _moduleName;
    std::string _robotName; 
    /*std::string inputPortName;
    std::string outputPortName;  
    std::string handlerPortName;*/
    

    yarp::os::Port handlerPort;      //a port to handle messages 

    /* pointer to a new thread to be created and started in configure() and stopped in close() */
    JTSCalibrationThread *_jtscalibrationThread;

public:
    /** 
     * document your methods too.
     */
    bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
    bool interruptModule();                       // interrupt, e.g., the ports 
    bool close();                                 // close and shut down the module
    bool respond(const yarp::os::Bottle& command, yarp::os::Bottle& reply);
    double getPeriod(); 
    bool updateModule();
};


#endif // __JTSCALIBRATIONMODULE_H__

//----- end-of-file --- ( next line intentionally left blank ) ------------------

