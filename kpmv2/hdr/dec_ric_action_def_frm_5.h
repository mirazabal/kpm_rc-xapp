#ifndef DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_5_KPM_V2_H
#define DECRYPTION_ASN_ACTION_DEFINITION_FORMAT_5_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "E2SM-KPM-ActionDefinition-Format5.h"
#include "kpm_ric_action_def_frm_5.h"



kpm_act_def_format_5_t kpm_dec_action_def_frm_5_asn(const E2SM_KPM_ActionDefinition_Format5_t * act_def_asn);


#ifdef __cplusplus
}
#endif

#endif