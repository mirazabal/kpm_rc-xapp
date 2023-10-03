
#ifndef KPM_V2_HDR_
#define KPM_V2_HDR_ 

#include <stdint.h>
#include <stddef.h>

__attribute__((visibility("default"))) 
int64_t kpm_dec_ind_asn(void* data);

__attribute__((visibility("default"))) 
size_t kpm_action_def(void* buf, size_t len);

__attribute__((visibility("default"))) 
size_t kpm_event_trigger_def(void* buf, size_t len);



#endif



