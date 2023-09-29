#ifndef DECRYPTION_GLOBAL_NG_RAN_H
#define DECRYPTION_GLOBAL_NG_RAN_H


#ifdef __cplusplus
extern "C" {
#endif

#include "GlobalNGRANNodeID.h"
#include "global_ng_ran_node_id.h"

global_ng_ran_node_id_t * dec_global_ng_ran_asn(const GlobalNGRANNodeID_t * global_ng_ran_asn);

#ifdef __cplusplus
}
#endif

#endif
