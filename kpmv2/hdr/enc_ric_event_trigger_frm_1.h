#ifndef ENCRYPTION_ASN_EVENT_TRIGGER_DEFINITION_FORMAT_1_KPM_V2_H
#define ENCRYPTION_ASN_EVENT_TRIGGER_DEFINITION_FORMAT_1_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "kpm_ric_event_trigger_def_frm_1.h"
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"

E2SM_KPM_EventTriggerDefinition_Format1_t * kpm_enc_event_trigger_def_frm_1_asn(const kpm_ric_event_trigger_format_1_t * event_trigger);

#ifdef __cplusplus
}
#endif

#endif