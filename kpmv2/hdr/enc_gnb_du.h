#ifndef ENCRYPTION_GNB_DU_H
#define ENCRYPTION_GNB_DU_H


#ifdef __cplusplus
extern "C" {
#endif

#include "UEID-GNB-DU.h"
#include "gnb_du.h"

UEID_GNB_DU_t * enc_gNB_DU_UE_asn(const gnb_du_e2sm_t * gnb_du);

#ifdef __cplusplus
}
#endif

#endif
