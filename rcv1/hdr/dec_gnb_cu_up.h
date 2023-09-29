#ifndef DECRYPTION_GNB_CU_UP_H
#define DECRYPTION_GNB_CU_UP_H


#ifdef __cplusplus
extern "C" {
#endif

#include "UEID-GNB-CU-UP.h"
#include "gnb_cu_up.h"

gnb_cu_up_e2sm_t dec_gNB_CU_UP_UE_asn(const UEID_GNB_CU_UP_t * gnb_cu_up_asn);

#ifdef __cplusplus
}
#endif

#endif
