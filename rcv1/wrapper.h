#ifndef WRAPPER_RCV1_HDR
#define WRAPPER_RCV1_HDR 

#include <stddef.h>

__attribute__((visibility("default")))
size_t rc_encode_rc_ric_ctl_hdr(void* out, size_t len);


__attribute__((visibility("default")))
size_t create_drb_rc_msg(void* out, size_t len);

__attribute__((visibility("default")))
size_t release_drb_rc_msg(void* out, size_t len);




#endif
