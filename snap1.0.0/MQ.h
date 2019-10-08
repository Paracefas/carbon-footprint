#ifndef __MQ_H__
#define __MQ_H__


struct MQ {
  typedef unsigned int port;
  MQ(void) = delete;
  MQ(port analog) : analog_{analog}, haveDigital_{false} {  }
  MQ(port analog, port digital) : analog_{analog}, digital_{digital}, haveDigital_{true} {  }
  MQ(port analog, double m, double b, double r0, double RL)
    : m_{m}, b_{b}, r0_{r0}, RL_{RL}, haveDigital_{false}, analog_{analog} {  }
  void start() {
    if(haveDigital_) 
      pinMode(digital_, INPUT);
  }
  double digitalRead() {
    if(haveDigital_) return ::digitalRead(digital_);
  }
  double read() {
    float sensor_volt; 
    float RS_gas; 
    float ratio;
    float sensorValue = analogRead(analog_);
    sensor_volt = sensorValue*(5.0/1023.0);
    RS_gas = ((5.0*RL_)/sensor_volt)-(RL_ == 1 ? 0 : RL_); 
    ratio = RS_gas/r0_;
    double ppm_log = (log10(ratio)-b_)/m_;
    double ppm = pow(10, ppm_log); 

    return ppm;
  }
private:
  double m_, b_, r0_, RL_;
  bool haveDigital_;
  port digital_, analog_;
};

#endif
