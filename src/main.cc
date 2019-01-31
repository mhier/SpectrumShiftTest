#include <iostream>
#include <chrono>
#include <ChimeraTK/Device.h>


int main() {

    ChimeraTK::setDMapFilePath("devices.dmap");

    ChimeraTK::Device dev;
    dev.open("VS");

    auto vsAmplSample = dev.getScalarRegisterAccessor<double>("AMPL.SAMPLE");
    auto vsAmplTD = dev.getOneDRegisterAccessor<double>("AMPL.TD");
    auto vsITD = dev.getOneDRegisterAccessor<double>("I.TD");

    double sampleFreq = 81.25/9;
    double T0 = 800.;
    double iT0 = T0 * sampleFreq;
    size_t imax = std::floor(iT0/2.);
    std::cout << "imax = " << imax << std::endl;

    vsAmplSample.read();
    vsAmplTD.read();
    std::cout << "vsAmplSample = " << vsAmplSample << std::endl;

    auto start = std::chrono::system_clock::now();

    bool isOff = false;
    while(true) {

      std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
      std::string sElapsed = "["+std::to_string(elapsed.count())+"] ";

      vsAmplSample.read();
      if(vsAmplSample < 1.0) {
        if(!isOff) std::cout << sElapsed << "off" << std::endl;
        isOff = true;
        continue;
      }
      if(isOff) {
        isOff = false;
        std::cout << sElapsed << "on" << std::endl;
      }
      vsAmplTD.read();
      vsITD.read();
      double threshold = 0.01;
      bool isOKampl = true;
      bool isOKi = true;
      for(size_t i=0; i<imax; ++i) {
        if(vsAmplTD[i] > threshold) isOKampl = false;
        if(vsITD[i] > threshold) isOKi = false;
      }
      if(!isOKampl || !isOKi) std::cout << sElapsed << "FAULT";
      if(!isOKampl) {
        std::cout << " ampl";
      }
      if(!isOKi) {
        std::cout <<  " I";
      }
      if(!isOKampl || !isOKi) {
        std::cout << std::endl;
        exit(1);
      }

    }

}
