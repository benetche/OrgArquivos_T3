#ifndef _LINE_AND_VEHICLE_
#define _LINE_AND_VEHICLE_
#include "line.h"
#include "vehicle.h"

int comparaRegistros(lineRecord *linha, vehicleRecord *veiculo);
void printMerged(lineRecord *lr, lineFileHeader* lh,vehicleRecord *vr, vehicleFileHeader* vh);
#endif