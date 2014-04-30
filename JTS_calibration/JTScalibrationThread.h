/**
 * @file demoThread.h
 * @brief this file contains the definition of an additional thread that does the job for the main module.
 */

#ifndef _JTSCALIBRATION_THREAD_H_
#define _JTSCALIBRATION_THREAD_H_

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <iostream>

/**
 * theoretically, here you should document your class(es) and methods. 
 */
class JTSCalibrationThread : public yarp::os::Thread {
private:

   	yarp::os::BufferedPort<yarp::os::Bottle> > inputPort_RA, inputPort_LA, inputPort_RL, inputPort_LL;
	yarp::os::BufferedPort<yarp::os::Bottle> > outputPort_RA, outputPort_LA, outputPort_RL, outputPort_LL;

public:
   /**
    * contructor.
    * @param threshold threshold for image filtering.
    */
   JTSCalibrationThread(const std::string& threadName,
                              const std::string& robotName,
                              int periodMilliseconds,
                              );

   /**
    * destructor.
    */
   ~JTSCalibrationThread();

   // and so forth... don't forget to document all your methods.
   bool threadInit();     
   void threadRelease();
   void run(); 
   void onStop();
};

#endif  //_JTSCALIBRATION_THREAD_H_

//----- end-of-file --- ( next line intentionally left blank ) ------------------
