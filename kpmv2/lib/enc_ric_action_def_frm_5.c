#include <assert.h>
#include "asn_constant.h"


#include "enc_ric_action_def_frm_5.h"
#include "enc_ric_action_def_frm_1.h"
#include "enc_ue_id.h"
#include "MatchingUEidPerSubItem.h"

E2SM_KPM_ActionDefinition_Format5_t * kpm_enc_action_def_frm_5_asn(const kpm_act_def_format_5_t * act_def_frm_5)
{
    E2SM_KPM_ActionDefinition_Format5_t * act_def_frm_5_asn = calloc(1, sizeof(E2SM_KPM_ActionDefinition_Format5_t));
    assert (act_def_frm_5_asn != NULL && "Memory exhausted" );

    assert(act_def_frm_5->ue_id_lst_len >= 2 && act_def_frm_5->ue_id_lst_len <= maxnoofUEIDPerSub);

    for (size_t i = 0; i < act_def_frm_5->ue_id_lst_len; i++)
    {
        MatchingUEidPerSubItem_t * ue_item = calloc(1, sizeof(MatchingUEidPerSubItem_t));
        assert(ue_item != NULL && "Memory exhausted");

        ue_item->ueID = enc_ue_id_asn(&act_def_frm_5->ue_id_lst[i]);
        int rc1 = ASN_SEQUENCE_ADD(&act_def_frm_5_asn->matchingUEidList.list, ue_item);
        assert(rc1 == 0);
    }


    act_def_frm_5_asn->subscriptionInfo = kpm_enc_action_def_frm_1_asn(&act_def_frm_5->action_def_format_1);

    return act_def_frm_5_asn;

}
