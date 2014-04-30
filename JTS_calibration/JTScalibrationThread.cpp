#include "JTSCalibrationThread.h"

using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

JTSCalibrationThread::JTSCalibrationThread(const std::string& threadName,
                                                 const std::string& robotName,
                                                 int periodMilliseconds,
                                                 ):
_period(periodMilliseconds),
_threadName(threadName),
_robotName(robotName),

{   

}

JTSCalibrationThread::~JTSCalibrationThread() { threadRelease(); }

bool JTSCalibrationThread::threadInit() {
    /* initialize variables and create data-structures if needed */

    /* opening ports */
	if(!inputPort_RA.open(("/" + _moduleName + "/" + inputPortName_RA).c_str())){
		cout  << ": unable to open input port RA" <<  endl;
		return false;  // unable to open; let RFModule know so that it won't run
    	}

	if(!inputPort_LA.open(("/" + _moduleName + "/" + inputPortName_LA).c_str())){
		cout  << ": unable to open input port LA" <<  endl;
		return false;  
    	}

	if(!inputPort_RL.open(("/" + _moduleName + "/" + inputPortName_RL).c_str())){
		cout  << ": unable to open input port RL" <<  endl;
		return false;  
    	}

	if(!inputPort_LL.open(("/" + _moduleName + "/" + inputPortName_LL).c_str())){
		cout  << ": unable to open input port LL" <<  endl;
		return false;  
    	}

	if(!outputPort_RA.open(("/" + _moduleName + "/" + outputPortName_RA).c_str())){
		cout  << ": unable to open output port RA" <<  endl;
		return false;  
    	}

	if(!outputPort_LA.open(("/" + _moduleName + "/" + outputPortName_LA).c_str())){
		cout  << ": unable to open output port LA" <<  endl;
		return false;  
    	}

	if(!outputPort_RL.open(("/" + _moduleName + "/" + outputPortName_RL).c_str())){
		cout  << ": unable to open output port RL" <<  endl;
		return false;  
    	}

	if(!outputPort_LL.open(("/" + _moduleName + "/" + outputPortName_LL).c_str())){
		cout  << ": unable to open output port LL" <<  endl;
		return false;  
    	}

    	return true;
}

void demoThread::run() {
    /* 
    * do some work ....
    * for example, convert the input image to a binary image using the threshold provided 
    */ 
    unsigned char value;
    while (isStopping() != true) { // the thread continues to run until isStopping() returns true
        cout << "demoThread: threshold value is " << *thresholdValue << endl;

        image = imagePortIn.read(true);

        if(image!=0) { 
            ImageOf<PixelRgb> &binary_image = imagePortOut.prepare();
            binary_image.resize(image->width(),image->height());

            for (x=0; x<image->width(); x++) {
                for (y=0; y<image->height(); y++) {

                    rgbPixel = image->safePixel(x,y);

                    if (((rgbPixel.r + rgbPixel.g + rgbPixel.b)/3) > *thresholdValue) {
                        value = (unsigned char) 255;
                    }
                    else {
                        value = (unsigned char) 0;
                    }

                    rgbPixel.r = value;
                    rgbPixel.g = value;
                    rgbPixel.b = value;
                    binary_image(x,y) = rgbPixel;
                }
            }

            imagePortOut.write();
        }
    }  //while
}

void demoThread::threadRelease() {
   /* for example, delete dynamically created data-structures */
}

void demoThread::onStop() {
    imagePortIn.interrupt();
    imagePortOut.interrupt();
    imagePortIn.close();
    imagePortOut.close();
}
