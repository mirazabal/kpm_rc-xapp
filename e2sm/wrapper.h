#ifndef	_WRAPPER_SM_H_
#define	_WRAPPER_SM_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


ssize_t __attribute__((visibility("default"))) e2sm_encode_ric_control_header(void *buffer, size_t buf_size, void *ueIDbuf, size_t ueIDbuf_size, long ricControlStyleType, long ricControlActionID);

//extern ssize_t e2sm_encode_ric_control_header(void *buffer, size_t buf_size, long ricControlStyleType, long ricControlActionID);

ssize_t __attribute__((visibility("default"))) e2sm_encode_ric_control_message(void *buffer, size_t buf_size, long targetPrimaryCell, long targetCell, long nrOrEUtraCell, long nrCGIOrECGI, void *ranParameterValue, size_t  ranParameterValue_size);

#endif /* _WRAPPER_H_ */
