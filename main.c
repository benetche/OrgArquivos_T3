/*
Autores:
-Eduardo Amaral - NUSP 11735021
-Vitor Beneti Martins - NUSP 11877635
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#include "vehicle.h"
#include "vehicleUtils.h"

#include "line.h"
#include "lineUtils.h"

#include "bTree.h"
#include "convertePrefixo.h"

#include "lineAndVehicle.h"

int main(void) {
  char *veiculoFileName = (char *)malloc(sizeof(char) * 128);
  char *linhaFileName = (char *)malloc(sizeof(char) * 128);

  int op;

  scanf(" %d", &op);
  vehicleFile *vf;
  lineFile *lf;

  switch (op) {
  case 15:
    scanf(" %s %s", veiculoFileName, linhaFileName);

    vf = createVehicleFileStruct(veiculoFileName, "rb");
    lf = createLineFileStruct(linhaFileName, "rb");

    vehicleRecord *veiculoCorrente;
    lineRecord *linhaCorrente;

    if (!vf) {
      printf("Falha no processamento do arquivo.\n");
    } else if (!lf) {
      printf("Falha no processamento do arquivo.\n");
    } else {
      readVehicleFileHeader(vf);
      readLineFileHeader(lf);
      vf->nRecords = vf->header->nroRegRemovidos + vf->header->nroRegistros;
      lf->nRecords = lf->header->nroRegRemovidos + lf->header->nroRegistros;
      veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
      linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));

      for (int i = 0; i < vf->nRecords; i++) {
        readVehicleReg(vf->fp, veiculoCorrente);
        fseek(lf->fp, 82, SEEK_SET);
        for (int j = 0; j < lf->nRecords; j++) {
          readLineReg(lf->fp, linhaCorrente);
          if (comparaRegistros(linhaCorrente, veiculoCorrente) == 1) {
            printMerged(linhaCorrente, lf->header, veiculoCorrente, vf->header);
            break;
          }
        }
      }
    }
  }

  return 0;
}