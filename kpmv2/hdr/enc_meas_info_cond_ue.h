#ifndef ENCODE_MEASUREMENT_INFO_COND_UE_KPM_V2_H
#define ENCODE_MEASUREMENT_INFO_COND_UE_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MeasurementCondUEidList.h"
#include "meas_info_cond_ue_lst.h"

MeasurementCondUEidList_t kpm_enc_meas_info_cond_ue_asn(const meas_info_cond_ue_lst_t * meas_cond_ue, const size_t meas_cond_ue_len);

#ifdef __cplusplus
}
#endif

#endif