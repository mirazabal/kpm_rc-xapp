#ifndef DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_2_KPM_V2_H
#define DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_2_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "E2SM-KPM-ActionDefinition-Format2.h"
#include "kpm_ric_action_def_frm_2.h"



kpm_act_def_format_2_t kpm_dec_action_def_frm_2_asn(const E2SM_KPM_ActionDefinition_Format2_t * act_def_asn);


#ifdef __cplusplus
}
#endif

#endif