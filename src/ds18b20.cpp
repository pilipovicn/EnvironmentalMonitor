#include "ds18b20.h"

DS18B20::DS18B20(const char* address) {
    address_ = strdup(address);
    snprintf(path, 46, "%s%s%s", BUS, address_, TEMPFILE);
}

DS18B20::~DS18B20() {
}

double DS18B20::getTemp() {
    FILE *devFile = fopen(path, "r");
    if (devFile == nullptr) {
        printf("Count not open %s\n", path);
        perror("\n");
    }
    double temp = -1;
    if (devFile != nullptr) {
        if (!ferror(devFile)) {
            double tempInt = -1;
            char crcConf[5];
            fscanf(devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x : crc=%*x %s", crcConf);
            if (strncmp(crcConf, "YES", 3) == 0) {
                fscanf(devFile, "%*x %*x %*x %*x %*x %*x %*x %*x %*x t=%lf", &tempInt);
                temp = tempInt / 1000.0;
            }
        }
    }
    fclose(devFile);
    return temp;
}
