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
  char *desordenadoFileName = (char *)malloc(sizeof(char) * 128);
  char *ordenadoFileName = (char *)malloc(sizeof(char) * 128);

  int op;
  int i, j;
  boolean encontrado;
  vehicleRecord *veiculoCorrente;
  lineRecord *linhaCorrente;

  scanf(" %d", &op);
  vehicleFile *vf;
  vehicleFile *vfOrdered;
  lineFile *lf;
  lineFile *lfOrdered;

  switch (op) {
  case 15:
    scanf(" %s %s", veiculoFileName, linhaFileName);

    vf = createVehicleFileStruct(veiculoFileName, "rb");
    lf = createLineFileStruct(linhaFileName, "rb");

    if (!vf) {
      printf("Falha no processamento do arquivo.\n");
    } else if (!lf) {
      printf("Falha no processamento do arquivo.\n");
    } else {
      // Leia com readVehicleFile passando false no segundo argumento
      readVehicleFileHeader(vf);
      readLineFileHeader(lf);
      vf->nRecords = vf->header->nroRegRemovidos + vf->header->nroRegistros;
      lf->nRecords = lf->header->nroRegRemovidos + lf->header->nroRegistros;
      veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
      linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
      encontrado = false;

      for (i = 0; i < vf->nRecords; i++) {
        readVehicleReg(vf->fp, veiculoCorrente);
        // Explique porque 82
        fseek(lf->fp, 82, SEEK_SET);
        for (j = 0; j < lf->nRecords; j++) {
          readLineReg(lf->fp, linhaCorrente);
          if (comparaRegistros(linhaCorrente, veiculoCorrente)) {
            encontrado = true;
            printMerged(linhaCorrente, lf->header, veiculoCorrente, vf->header);
            break;
          }
        }
      }
      if (encontrado == false) {
        printf("Registro inexistente.\n");
      }
    }
    break;
  case 17:
    scanf(" %s %s %*s", desordenadoFileName, ordenadoFileName);
    vf = createVehicleFileStruct(desordenadoFileName, "rb");

    if (!vf) {
      printf("Falha no processamento do arquivo.\n");
    } else {
      vfOrdered = createVehicleFileStruct(ordenadoFileName, "wb");
      readVehicleFile(vf, true);
      copyOrderedVehicleRecords(vf, vfOrdered);
      destroyVehicleFile(vf);
      destroyVehicleFile(vfOrdered);
      binarioNaTela(ordenadoFileName);
    }
    break;
  case 18:
    scanf(" %s %s %*s", desordenadoFileName, ordenadoFileName);
    lf = createLineFileStruct(desordenadoFileName, "rb");

    if (!lf) {
      printf("Falha no processamento do arquivo.\n");
    } else {
      lfOrdered = createLineFileStruct(ordenadoFileName, "wb");
      readLineFile(lf, true);
      copyOrderedLineRecords(lf, lfOrdered);
      destroyLineFile(lf);
      destroyLineFile(lfOrdered);
      binarioNaTela(ordenadoFileName);
    }

    break;
  case 19:
    scanf(" %s %s %*s %*s", veiculoFileName, linhaFileName);

    vf = createVehicleFileStruct(veiculoFileName, "rb");

    if (!vf) {
      printf("Falha no processamento do arquivo.\n");
      break;
    }

    lf = createLineFileStruct(linhaFileName, "rb");

    if (!lf) {
      destroyVehicleFile(vf);
      printf("Falha no processamento do arquivo.\n");
    } else {

      vfOrdered = createVehicleFileStruct("tmpVehicle.bin", "wb");
      readVehicleFile(vf, true);
      copyOrderedVehicleRecords(vf, vfOrdered);
      destroyVehicleFile(vf);
      destroyVehicleFile(vfOrdered);

      lfOrdered = createLineFileStruct("tmpLine.bin", "wb");
      readLineFile(lf, true);
      copyOrderedLineRecords(lf, lfOrdered);
      destroyLineFile(lf);
      destroyLineFile(lfOrdered);

      vf = createVehicleFileStruct("tmpVehicle.bin", "rb");
      lf = createLineFileStruct("tmpLine.bin", "rb");

      readVehicleFile(vf, false);
      readLineFile(lf, false);
      encontrado = false;

      if (vf->nRecords > 0 && lf->nRecords > 0) {
        i = j = 0;
        veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
        linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
        readVehicleReg(vf->fp, veiculoCorrente);
        readLineReg(lf->fp, linhaCorrente);

        while (i < vf->nRecords && j < lf->nRecords) {

          while (veiculoCorrente->codLinha == linhaCorrente->codLinha) {
            encontrado = true;
            printMerged(linhaCorrente, lf->header, veiculoCorrente, vf->header);
            i++;
            if (i >= vf->nRecords) {
              break;
            }
            destroyVehicleRecord(veiculoCorrente);
            veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
            readVehicleReg(vf->fp, veiculoCorrente);
          }

          while (veiculoCorrente->codLinha > linhaCorrente->codLinha) {
            j++;
            if (j >= lf->nRecords) {
              break;
            }
            destroyLineRecord(linhaCorrente);
            linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
            readLineReg(lf->fp, linhaCorrente);
          }

          while (veiculoCorrente->codLinha < linhaCorrente->codLinha) {
            i++;
            if (i >= vf->nRecords) {
              break;
            }
            destroyVehicleRecord(veiculoCorrente);
            veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
            readVehicleReg(vf->fp, veiculoCorrente);
          }
        }

        destroyVehicleRecord(veiculoCorrente);
        destroyLineRecord(linhaCorrente);
      }

      if (encontrado == false) {
        printf("Registro inexistente.\n");
      }
      destroyLineFile(lf);
      destroyVehicleFile(vf);
    }

    break;
  default:
    break;
  }

  free(veiculoFileName);
  free(linhaFileName);
  free(desordenadoFileName);
  free(ordenadoFileName);

  return 0;
}