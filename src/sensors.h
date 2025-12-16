#ifndef SENSORS_H
#define SENSORS_H

#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#endif