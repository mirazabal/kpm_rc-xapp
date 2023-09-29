#ifndef ENCODE_MEASUREMENT_INFO_FORMAT_3_KPM_V2_H
#define ENCODE_MEASUREMENT_INFO_FORMAT_3_KPM_V2_H

#include "MeasurementCondList.h"
#include "meas_info_frm_3_lst.h"

MeasurementCondList_t kpm_enc_meas_info_cond_asn(const meas_info_format_3_lst_t * meas_info_cond, const size_t meas_info_cond_len);



#endif