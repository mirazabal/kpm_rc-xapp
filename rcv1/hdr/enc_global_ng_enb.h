#ifndef ENCRYPTION_GLOBAL_NG_ENB_H
#define ENCRYPTION_GLOBAL_NG_ENB_H


#ifdef __cplusplus
extern "C" {
#endif

#include "GlobalNgENB-ID.h"
#include "global_ng_enb_id.h"

GlobalNgENB_ID_t * enc_global_ng_enb_asn(const global_ng_enb_id_t * global_ng_enb_id);

#ifdef __cplusplus
}
#endif

#endif
