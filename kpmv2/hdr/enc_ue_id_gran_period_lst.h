#ifndef ENCODE_UE_ID_GRANULARITY_PERIOD_LIST_KPM_V2_H
#define ENCODE_UE_ID_GRANULARITY_PERIOD_LIST_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MatchingUEidPerGP-Item.h"
#include "ue_id_gran_period_lst.h"

MatchingUEidPerGP_Item_t * kpm_enc_ue_id_gran_period_asn(const ue_id_gran_period_lst_t * ue_id_gran_period);

#ifdef __cplusplus
}
#endif

#endif
