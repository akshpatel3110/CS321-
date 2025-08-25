#ifndef BMP390_H
#define BMP390_H

bool bmp390_init();
void bmp390_read(float &pressure_hpa, float &altitude_m);

#endif
