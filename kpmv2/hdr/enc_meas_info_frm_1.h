#ifndef ENCODE_MEASUREMENT_INFO_FORMAT_1_KPM_V2_H
#define ENCODE_MEASUREMENT_INFO_FORMAT_1_KPM_V2_H

#include "MeasurementInfoList.h"
#include "meas_info_frm_1_lst.h"

MeasurementInfoList_t kpm_enc_meas_info_asn(const meas_info_format_1_lst_t * meas_info, const size_t meas_info_len);



#endif