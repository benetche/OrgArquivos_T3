#include "lineAndVehicle.h"
#include "lineUtils.h"
#include "util.h"
#include "vehicleUtils.h"

boolean comparaRegistros(lineRecord *linha, vehicleRecord *veiculo) {
  if (linha->removido == '1' && veiculo->removido == '1') {
    return linha->codLinha == veiculo->codLinha;
  }
  return false;
}

void printMerged(lineRecord *lr, lineFileHeader *lh, vehicleRecord *vr,
                 vehicleFileHeader *vh) {
  if (vr->removido == '1') {

    printf("%s: ", vh->descrevePrefixo);
    printf("%s", vr->prefixo);
    printf("\n");

    printf("%s: ", vh->descreveModelo);
    if (isStrNull(vr->model)) {
      printf(NULO);
    } else {
      printf("%s", vr->model);
    }
    printf("\n");

    printf("%s: ", vh->descreveCategoria);
    if (isStrNull(vr->categoria)) {
      printf(NULO);
    } else {
      printf("%s", vr->categoria);
    }
    printf("\n");

    printf("%s: ", vh->descreveData);
    if (isStrNull(vr->data)) {
      printf(NULO);
    } else {
      printDate(vr->data);
    }
    printf("\n");

    printf("%s: ", vh->descreveLugares);
    if (isIntNull(vr->quantidadeLugares)) {
      printf(NULO);
    } else {
      printf("%d", vr->quantidadeLugares);
    }
    printf("\n");
  }
  if (lr->removido == '1') {

    printf("%s: ", lh->descreveCodigo);
    printf("%d", lr->codLinha);
    printf("\n");
    printf("%s: ", lh->descreveNome);
    if (isStrNull(lr->nomeLinha)) {
      printf(NULO);
    } else {
      printf("%s", lr->nomeLinha);
    }
    printf("\n");

    printf("%s: ", lh->descreveCor);
    if (isStrNull(lr->corLinha)) {
      printf(NULO);
    } else {
      printf("%s", lr->corLinha);
    }
    printf("\n");

    printf("%s: ", lh->descreveCartao);
    if (isStrNull(lr->aceitaCartao)) {
      printf(NULO);
    } else {
      printCard(lr->aceitaCartao);
    }
    printf("\n");
    printf("\n");
  }
}