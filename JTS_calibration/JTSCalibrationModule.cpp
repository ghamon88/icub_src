#include "JTSCalibrationModule.h"
#include "JTSCalibrationThread.h"
//#include <paramHelp/paramHelperServer.h>
//#include <paramHelp/paramHelperClient.h>

#include <cstdio>
#include <string>

using namespace yarp::os;
using namespace yarp::sig;
using namespace std;
using namespace paramHelp;

/* 
 * Configure method. Receive a previously initialized
 * resource finder object. Use it to configure your module.
 * If you are migrating from the old Module, this is the 
 * equivalent of the "open" method.
 */

namespace JTSCalibration {
    JTSCalibrationModule::JTSCalibrationModule()
    {
        
    }

bool JTSCalibrationModule::configure(yarp::os::ResourceFinder &rf) {    


   /*_parameterServer = new ParamHelperServer(JTSCalibrationParamDescriptors, JTSCalibrationParamDescriptorsSize,
                                                 JTSCalibrationCommandDescriptors, JTSCalibrationCommandDescriptorsSize);
        if (!_parameterServer) {
            error_out("Could not initialize parameter server. Closing module");
            return false;
        }
        _parameterServer->linkParam(JTSCalibrationParamIDModuleName, &_moduleName);
        _parameterServer->linkParam(JTSCalibrationParamIDPeriod, &_period);
        _parameterServer->linkParam(JTSCalibrationParamIDRobotName, &_robotName);*/
	/*_parameterServer->linkParam(JTSCalibrationParamInputPortRightArmName, &_moduleName);
        _parameterServer->linkParam(JTSCalibrationParamInputPortLeftArmName, &_period);
        _parameterServer->linkParam(JTSCalibrationParamInputPortRightLegName, &_robotName);
	_parameterServer->linkParam(JTSCalibrationParamInputPortLeftLegName, &_robotName);*/
	
        /*
        _parameterServer->registerCommandCallback(JTSCalibrationCommandIDHelp, this);
        _parameterServer->registerCommandCallback(JTSCalibrationCommandIDQuit, this);
        _parameterServer->registerCommandCallback(JTSCalibrationCommandIDReset, this);
        */

        // Read parameters from configuration file (or command line)
        Bottle initMsg;
        /*_parameterServer->initializeParams(rf, initMsg);
        info_out("*** Parsing configuration file...\n%s\n", initMsg.toString().c_str());*/
        
        // Open ports for communicating with other modules
        /*if(!_parameterServer->init(_moduleName)) {
            error_out("Error while initializing parameter server. Closing module.\n");
            return false;
        }*/

	inputPortName_RA      = "/";
    	inputPortName_RA      += getName(
                           rf.check("InputPortRightArm", 
                           Value("right_arm/raw:i"),
                           "Input raw value right arm JTS port (string)").asString()
                           );
	inputPortName_LA      = "/";
    	inputPortName_LA      += getName(
                           rf.check("InputPortLeftArm", 
                           Value("left_arm/raw:i"),
                           "Input raw value left arm JTS port (string)").asString()
                           );

	inputPortName_RL      = "/";
    	inputPortName_RL      += getName(
                           rf.check("InputPortRightLeg", 
                           Value("right_leg/raw:i"),
                           "Input raw value right leg JTS port (string)").asString()
                           );
	inputPortName_LL      = "/";
    	inputPortName_Ll      += getName(
                           rf.check("InputPortLeftLeg", 
                           Value("left_leg/raw:i"),
                           "Input raw value left leg JTS port (string)").asString()
                           );

    	outputPortName_RA        = "/";
    	outputPortName_RA       += getName(
                           rf.check("OutputPortRightArm", 
                           Value("right_arm/calibrated:o"),
                           "Output calibrated value right arm JTS port (string)").asString()
                           );

	outputPortName_LA        = "/";
    	outputPortName_LA       += getName(
                           rf.check("OutputPortLeftArm", 
                           Value("left_arm/calibrated:o"),
                           "Output calibrated value left arm JTS port (string)").asString()
                           );

	outputPortName_RL        = "/";
    	outputPortName_RL       += getName(
                           rf.check("OutputPortRightLeg", 
                           Value("right_leg/calibrated:o"),
                           "Output calibrated value right leg JTS port (string)").asString()
                           );

	outputPortName_LL        = "/";
    	outputPortName_LL       += getName(
                           rf.check("OutputPortLeftLeg", 
                           Value("left_leg/calibrated:o"),
                           "Output calibrated value left leg JTS port (string)").asString()
                           );


	

    	setName(_moduleName.c_str());

    	/*attach(inputPort_RA);
	attach(inputPort_LA);
	attach(inputPort_RL);
	attach(inputPort_LL);

	attach(outputPort_RA);
	attach(outputPort_LA);
	attach(outputPort_RL);
	attach(outputPort_LL);*/

    	initMsg.clear();
	
	/*
    	* attach a port of the same name as the module (prefixed with a /) to the module
    	* so that messages received from the port are redirected to the respond method
    	*/
    	handlerPortName =  "/";
    	handlerPortName += getName();         // use getName() rather than a literal 

    	if (!handlerPort.open(handlerPortName.c_str())) {           
        	cout << getName() << ": Unable to open port " << handlerPortName << endl;  
        return false;
    	}

    	attach(handlerPort);                  // attach to port


	//--------------------------CONTROL THREAD--------------------------
        _jtscalibrationThread = new JTSCalibrationThread(_moduleName,
                                                   _robotName,
                                                   _period,
                                                   *_parameterServer,
                                                   *_parameterClient);
        if (!_jtscalibrationThread || !_jtscalibrationThread->start()) {
            error_out("Error while initializing control thread. Closing module.\n");
            return false;
        }
        //_controlThread->setInitialConditions(_initialPiHat, _initialXi1);
        
        info_out("JTS Calibration module correctly initialized\n");
        return true;
}
}
    

bool JTSCalibrationModule::interruptModule() {
	if(_jtscalibration) _jtscalibrationThread->stop();
    	handlerPort.interrupt();
    	return true;
}

bool JTSCalibrationModule::close() {

    /* stop the thread */
	if(_jtscalibration){
        	_jtscalibrationThread->stop();
		delete _jtscalibrationThread;
        	_jtscalibrationThread = NULL;
	}
	if(_parameterServer) {
            	_parameterServer->close();
            	delete _parameterServer;
            	_parameterServer = NULL;
        }
	//closing ports de communication avec le module (commandes), pour le thread le faire dans thread-->release
	handlerPort.close();
	info_out("about to close\n");
    	return true;
}

bool JTSCalibrationModule::respond(const Bottle& command, Bottle& reply) {
  string helpMessage =  string(getName().c_str()) + 
                        " commands are: \n" +  
                        "help \n" + 
                        "quit \n" + 
/********************** documenter ici les commandes de changement de gain et d'offset ******************/
                        "set thr <n> ... set the threshold \n" + 
                        "(where <n> is an integer number) \n";
/********************************************************************************************************/
  reply.clear(); 

  if (command.get(0).asString()=="quit") {
       reply.addString("quitting");rf.check("InputPortRightArm", 
                           Value("right_arm/raw:i"),
                           "Input raw value right arm JTS port (string)").asString()
                           );
       return false;     
   }
   else if (command.get(0).asString()=="help") {
      cout << helpMessage;
      reply.addString("ok");
   }
/************** Ici prendre en compte les commandes de changement de gain et d'offset *******************/
   else if (command.get(0).asString()=="set") {
      if (command.get(1).asString()=="thr") {
         thresholdValue = command.get(2).asInt(); // set parameter value
         reply.addString("ok");
      }
   }
/********************************************************************************************************/
   return true;
}

/* Called periodically every getPeriod() seconds */
bool JTSCalibrationModule::updateModule() {
	if (!_jtscalibrationThread) {
            error_out("%s: Error. Control thread pointer is zero.\n", _moduleName.c_str());
            return false;
        }
// tester le tps d'exécution, période ??
    	return true;
}

double JTSCalibrationModule::getPeriod() {
    /* module periodicity (seconds), called implicitly by myModule */    
	return rf.check("period", Value(10),"Period in ms").asInt());
}

