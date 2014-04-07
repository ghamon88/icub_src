/*
 * Copyright: (C) 2014 ISIR
 * Author: Guillaume Hamon
 */


#include <yarp/os/all.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace yarp::os;



int main(int argc, char *argv[]) {

  double calibrated_JTS_RA[3] = {0.0,0.0,0.0};
  double Gain_RA[3] = {2.52,0.0,3.53}; // trouver Gain J1
  double offset_RA[3] = {0.34,-0.745,1.26}; //à déterminer en ligne normalement, prendre au démarrage

  double calibrated_JTS_LA[3] = {0.0,0.0,0.0};
  double Gain_LA[3] = {0.0,0.0,0.0}; // trouver Gain J0 J1 J3
  double offset_LA[3] = {0.0,0.0,0.0}; //à déterminer en ligne normalement, prendre au démarrage


  // Set up YARP
  Network yarp;

  // Make two ports called /hello/in and /hello/out
  // We'll send "Bottles" (a simple nested list container) between these ports
  BufferedPort<Bottle> inPort_LA, inPort_RA, outPort_RA, outPort_LA;
  bool ok = inPort_RA.open("/iCub/JTScalibration/right_arm/in_raw");
  ok = ok && inPort_LA.open("/iCub/JTScalibration/left_arm/in_raw");
  ok = ok && outPort_RA.open("/iCub/JTScalibration/right_arm/out_calibrated");
  ok = ok && outPort_LA.open("/iCub/JTScalibration/left_arm/out_calibrated");
  if (!ok) {
    fprintf(stderr, "Failed to create ports.\n");
    fprintf(stderr, "Maybe you need to start a nameserver (run 'yarpserver')\n");
    return 1;
  }

  // Make a connection between the output port and the input port
  /*yarp.connect("/icub/joint_torque_sensor/right_arm/analog:o",inPort_RA.getName());
  yarp.connect("/icub/joint_torque_sensor/left_arm/analog:o",inPort_LA.getName());*/
  
  bool first_right=1; //affecte les valeurs d'offset à la première réception, à changer par un signal de calibration??
  bool first_left=1;

  while(true) {
    // read the message
    Bottle* in_RA;
    in_RA=inPort_RA.read(true);
    if (in_RA==NULL) {
      	fprintf(stderr, "Failed to read right arm JTS values\n");
      return 1;
    }else{
	if(first_right){
		for(int i=0;i<3;i++){
			offset_RA[i]=(*in_RA).get(i).asDouble();
		}
		first_right=false;
	}else{
		for(int i=0; i<3;i++){
			calibrated_JTS_RA[i]=(*in_RA).get(i).asDouble()*Gain_RA[i]-offset_RA[i];
		}
    		cout<<"Right Arm Joint Torque Sensors calibrated values = "<< calibrated_JTS_RA[0]<< " "<<calibrated_JTS_RA[1]<< " "<<calibrated_JTS_RA[2]<<endl;
    	}
    }

    Bottle* in_LA;
    in_LA=inPort_LA.read(true);
    if (in_LA==NULL) {
      	fprintf(stderr, "Failed to read left arm JTS values\n");
      return 1;
    }else{
	if(first_left){
		for(int i=0;i<3;i++){
			offset_LA[i]=(*in_LA).get(i).asDouble();
		}
		first_left=false;
	}else{
		for(int i=0; i<3;i++){
			calibrated_JTS_LA[i]=(*in_LA).get(i).asDouble()*Gain_LA[i]-offset_LA[i];
		}
    		cout<<"Left Arm Joint Torque Sensors calibrated values = "<< calibrated_JTS_LA[0]<< " "<<calibrated_JTS_LA[1]<< " "<<calibrated_JTS_LA[2]<<endl;
    	}
    }

    // prepare a message to send
    Bottle&out_RA = outPort_RA.prepare();
    out_RA.clear();
    out_RA.addDouble(calibrated_JTS_RA[0]);
    out_RA.addDouble(calibrated_JTS_RA[1]);
    out_RA.addDouble(calibrated_JTS_RA[2]);
    printf("Sending %s\n", out_RA.toString().c_str());
    // send the message
    outPort_RA.write(true);


    Bottle&out_LA = outPort_LA.prepare();
    out_LA.clear();
    out_LA.addDouble(calibrated_JTS_LA[0]);
    out_LA.addDouble(calibrated_JTS_LA[1]);
    out_LA.addDouble(calibrated_JTS_LA[2]);
    printf("Sending %s\n", out_LA.toString().c_str());
    // send the message
    outPort_LA.write(true);

  }

  return 0;
}
