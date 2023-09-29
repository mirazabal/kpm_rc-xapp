#ifndef ACTION_DEFINITION_FORMAT_5_KPM_V2_H
#define ACTION_DEFINITION_FORMAT_5_KPM_V2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ue_id.h"
#include "kpm_ric_action_def_frm_1.h"

//  8.2.1.2.5  E2SM-KPM Action Definition Format 5

typedef struct {
    size_t ue_id_lst_len;  // [2, 65535]
    ue_id_e2sm_t *ue_id_lst;  // 8.3.24

    kpm_act_def_format_1_t action_def_format_1;  // 8.2.1.2.1

} kpm_act_def_format_5_t;

void free_kpm_action_def_frm_5(kpm_act_def_format_5_t * src);

bool eq_kpm_action_def_frm_5(kpm_act_def_format_5_t const * m0, kpm_act_def_format_5_t const * m1);

kpm_act_def_format_5_t cp_kpm_action_def_frm_5(const kpm_act_def_format_5_t * src);

#ifdef __cplusplus
}
#endif

#endif

// done
