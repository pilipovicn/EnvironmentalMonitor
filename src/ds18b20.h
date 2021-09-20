#ifndef DS18B20_H
#define DS18B20_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUS "/sys/bus/w1/devices/"
#define TEMPFILE "/w1_slave"

class DS18B20 {
public:
    DS18B20(const char* address);
    virtual ~DS18B20();
    double getTemp();
private:
    char* address_;
    char path[47];

};


#endif
