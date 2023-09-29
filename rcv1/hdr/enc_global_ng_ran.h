#ifndef ENCRYPTION_GLOBAL_NG_RAN_H
#define ENCRYPTION_GLOBAL_NG_RAN_H


#ifdef __cplusplus
extern "C" {
#endif

#include "GlobalNGRANNodeID.h"
#include "global_ng_ran_node_id.h"

GlobalNGRANNodeID_t * enc_global_ng_ran_asn(const global_ng_ran_node_id_t * global_ng_ran);

#ifdef __cplusplus
}
#endif

#endif
