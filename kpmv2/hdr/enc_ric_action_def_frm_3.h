#ifndef ENCRYPTION_ASN_ACTION_DEFINITION_FORMAT_3_KPM_V2_H
#define ENCRYPTION_ASN_ACTION_DEFINITION_FORMAT_3_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_action_def_frm_3.h"
#include "E2SM-KPM-ActionDefinition-Format3.h"

E2SM_KPM_ActionDefinition_Format3_t * kpm_enc_action_def_frm_3_asn(const kpm_act_def_format_3_t * act_def_frm_3);

#ifdef __cplusplus
}
#endif

#endif