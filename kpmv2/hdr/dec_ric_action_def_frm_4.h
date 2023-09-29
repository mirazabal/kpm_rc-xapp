#ifndef DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_4_KPM_V2_H
#define DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_4_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_action_def_frm_4.h"
#include "E2SM-KPM-ActionDefinition-Format4.h"

kpm_act_def_format_4_t kpm_dec_action_def_frm_4_asn(const E2SM_KPM_ActionDefinition_Format4_t * act_def_asn);

#ifdef __cplusplus
}
#endif

#endif