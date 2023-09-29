#ifndef ENCRYPTION_NG_ENB_DU_H
#define ENCRYPTION_NG_ENB_DU_H


#ifdef __cplusplus
extern "C" {
#endif

#include "UEID-NG-ENB-DU.h"
#include "ng_enb_du.h"

UEID_NG_ENB_DU_t * enc_ng_eNB_DU_UE_asn(const ng_enb_du_e2sm_t * ng_enb_du);

#ifdef __cplusplus
}
#endif

#endif
