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

  arvoreB *arvore = NULL;
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

      readVehicleFileHeader(vf);
      readLineFileHeader(lf);
      vf->nRecords = vf->header->nroRegRemovidos + vf->header->nroRegistros;
      lf->nRecords = lf->header->nroRegRemovidos + lf->header->nroRegistros;
      veiculoCorrente = (vehicleRecord *)malloc(sizeof(vehicleRecord));
      linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
      encontrado = false;

      // loop de juncao aninhado
      for (i = 0; i < vf->nRecords; i++) {
        readVehicleReg(vf->fp, veiculoCorrente);
        // ir para o byte 82 (fim do header) todo loop para ler todos
        // os registros de linha novamente
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
      destroyVehicleRecord(veiculoCorrente);
      destroyLineRecord(linhaCorrente);
      destroyVehicleFile(vf);
      destroyLineFile(lf);
    }
    break;

  case 16:
    scanf(" %s %s %*s", veiculoFileName, linhaFileName);
    vf = createVehicleFileStruct(veiculoFileName, "rb");
    lf = createLineFileStruct(linhaFileName, "rb");
    arvore = criaArvoreB("arvoreLinha.bin", "wb+");

    // prevencao de erros de leitura do arquivo
    if (!lf) {
      printf("Falha no processamento do arquivo.\n");
    } else if (!vf) {
      printf("Falha no processamento do arquivo.\n");
    } else if (!arvore) {
      printf("Falha no processamento do arquivo.\n");
    } else {

      readVehicleFileHeader(vf);
      readLineFileHeader(lf);
      vf->nRecords = vf->header->nroRegRemovidos + vf->header->nroRegistros;
      lf->nRecords = lf->header->nroRegRemovidos + lf->header->nroRegistros;

      lineRecord *linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
      int64_t offsetCorrente;

      // criando arquivo arvoreB a partir dos registros de linha
      for (i = 0; i < lf->nRecords; i++) {

        offsetCorrente = ftell(lf->fp);
        readLineReg(lf->fp, linhaCorrente);

        if (linhaCorrente->removido == '1') {

          int32_t chave = linhaCorrente->codLinha;
          inserirNaArvoreB(arvore,
                           criaChavePonteiroPreenchida(chave, offsetCorrente));
        }
      }

      vehicleRecord *veiculoCorrente =
          (vehicleRecord *)malloc(sizeof(vehicleRecord));
      // juncao de loop unico
      encontrado = false;
      for (i = 0; i < vf->nRecords; i++) {
        readVehicleReg(vf->fp, veiculoCorrente);
        // busca registros nao removidos
        if (veiculoCorrente->removido == '1') {

          int64_t offsetBuscado =
              buscaNaArvoreB(arvore, veiculoCorrente->codLinha);
          // caso encontrado, printa os registros juntos
          if (offsetBuscado != -1) {

            encontrado = true;
            fseek(lf->fp, offsetBuscado, SEEK_SET);
            readLineReg(lf->fp, linhaCorrente);
            printMerged(linhaCorrente, lf->header, veiculoCorrente, vf->header);
          }
        }
      }
      // caso nenhum registro tenha sido recuperado
      if (!encontrado) {
        printf("Registro inexistente.\n");
      }

      // limpeza da RAM
      destroyVehicleRecord(veiculoCorrente);
      destroyLineRecord(linhaCorrente);
      destroiArvoreB(arvore);
      destroyLineFile(lf);
      destroyVehicleFile(vf);
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
      // Copia registros de forma ordenada
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
      // Copia registros de forma ordenada
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

      // Ordena ambos arquivos
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

      // Lê registros ordenados
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

        // Lê registros enquanto linhas e veículos existem
        while (i < vf->nRecords && j < lf->nRecords) {

          // Imprime merged todos veículos com código da linha corrente
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

          // Pega o próximo registro de linha enquanto a linha do veículo
          // corrente seja maior que a linha corrente
          while (veiculoCorrente->codLinha > linhaCorrente->codLinha) {
            j++;
            if (j >= lf->nRecords) {
              break;
            }
            destroyLineRecord(linhaCorrente);
            linhaCorrente = (lineRecord *)malloc(sizeof(lineRecord));
            readLineReg(lf->fp, linhaCorrente);
          }

          // Pega o próximo registro de veículo enquanto a linha do veículo
          // corrente seja menor que a linha corrente
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