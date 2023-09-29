#ifndef DECRYPTION_ASN_EVENT_TRIGGER_DEFINITION_FORMAT_1_KPM_V2_H
#define DECRYPTION_ASN_EVENT_TRIGGER_DEFINITION_FORMAT_1_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_event_trigger_def_frm_1.h"
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"

kpm_ric_event_trigger_format_1_t kpm_dec_event_trigger_def_frm_1_asn(const E2SM_KPM_EventTriggerDefinition_Format1_t * event_trigger_asn);

#ifdef __cplusplus
}
#endif

#endif