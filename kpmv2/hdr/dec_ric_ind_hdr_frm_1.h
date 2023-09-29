#ifndef DECRYPTION_ASN_RIC_INDICATION_HEADER_FORMAT_1_KPM_V2_H
#define DECRYPTION_ASN_RIC_INDICATION_HEADER_FORMAT_1_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_ind_hdr_frm_1.h"
#include "E2SM-KPM-IndicationHeader-Format1.h"

kpm_ric_ind_hdr_format_1_t kpm_dec_ind_hdr_frm_1_asn(const E2SM_KPM_IndicationHeader_Format1_t * ind_hdr_asn);

#ifdef __cplusplus
}
#endif

#endif