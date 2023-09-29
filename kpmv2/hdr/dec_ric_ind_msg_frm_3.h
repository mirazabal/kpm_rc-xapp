#ifndef DECRYPTION_ASN_RIC_INDICATION_MESSAGE_FORMAT_3_KPM_V2_H
#define DECRYPTION_ASN_RIC_INDICATION_MESSAGE_FORMAT_3_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_ind_msg_frm_3.h"
#include "E2SM-KPM-IndicationMessage-Format3.h"

kpm_ind_msg_format_3_t kpm_dec_ind_msg_frm_3_asn(const E2SM_KPM_IndicationMessage_Format3_t * ind_msg_asn);

#ifdef __cplusplus
}
#endif

#endif