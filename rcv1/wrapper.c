#include "wrapper.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "rc_data_ie.h"
#include "e2sm_rc_ctrl_hdr_frmt_1.h"
#include "ran_param_list.h"
#include "rc_enc_asn.h"
#include "fill_rnd_data_rc.h"


typedef enum{
  DRB_QoS_Configuration_7_6_2_1 = 1,
  QoS_flow_mapping_configuration_7_6_2_1 = 2,
  Logical_channel_configuration_7_6_2_1 = 3,
  Radio_admission_control_7_6_2_1 = 4,
  DRB_termination_control_7_6_2_1 = 5,
  DRB_split_ratio_control_7_6_2_1 = 6,
  PDCP_Duplication_control_7_6_2_1 = 7,
} rc_ctrl_service_style_1_e;

static
ue_id_e2sm_t ue_id ;



static
e2sm_rc_ctrl_hdr_frmt_1_t gen_rc_ctrl_hdr_frmt_1(void)
{
  e2sm_rc_ctrl_hdr_frmt_1_t dst = {0};

  // 6.2.2.6
  {
  //lock_guard(&mtx);
  dst.ue_id =  fill_rnd_ue_id(); // cp_ue_id_e2sm(&ue_id);
  }
  // CONTROL Service Style 1: Radio Bearer Control
  dst.ric_style_type = 1;

  // QoS flow mapping conf
  dst.ctrl_act_id = QoS_flow_mapping_configuration_7_6_2_1 ;

  return dst;
}

static
e2sm_rc_ctrl_hdr_t gen_rc_ctrl_hdr(void)
{
  e2sm_rc_ctrl_hdr_t dst = {0};
  // Radio Bearer Control
  dst.format = FORMAT_1_E2SM_RC_CTRL_HDR;
  dst.frmt_1 = gen_rc_ctrl_hdr_frmt_1();
  return dst;
}

typedef enum {
  DRB_ID_8_4_2_2 = 1,
  LIST_OF_QOS_FLOWS_MOD_IN_DRB_8_4_2_2 = 2,
  QOS_FLOW_ITEM_8_4_2_2 = 3,
  QOS_FLOW_ID_8_4_2_2 = 4,
  QOS_FLOW_MAPPING_IND_8_4_2_2 = 5,
} qos_flow_mapping_conf_e;

static
e2sm_rc_ctrl_msg_frmt_1_t gen_rc_ctrl_msg_frmt_1_qos_flow_map(int drb)
{
  e2sm_rc_ctrl_msg_frmt_1_t dst = {0};

  // 8.4.2.2 QoS flow mapping configuration
  dst.sz_ran_param = 2;
  dst.ran_param = calloc(2, sizeof(seq_ran_param_t));
  assert(dst.ran_param != NULL && "Memory exhausted");

  dst.ran_param[0].ran_param_id = DRB_ID_8_4_2_2;
  dst.ran_param[0].ran_param_val.type = ELEMENT_KEY_FLAG_TRUE_RAN_PARAMETER_VAL_TYPE;
  dst.ran_param[0].ran_param_val.flag_true = calloc(1, sizeof(ran_parameter_value_t)) ;
  assert(dst.ran_param[0].ran_param_val.flag_true != NULL && "Memory exhausted");

  // Let's suppose that it is the DRB 4 or 5
  assert(drb == 4 || drb == 5);
  dst.ran_param[0].ran_param_val.flag_true->type = INTEGER_RAN_PARAMETER_VALUE;
  dst.ran_param[0].ran_param_val.flag_true->int_ran = drb;

  // List of QoS Flows to be modified in DRB
  dst.ran_param[1].ran_param_id = LIST_OF_QOS_FLOWS_MOD_IN_DRB_8_4_2_2;
  dst.ran_param[1].ran_param_val.type = LIST_RAN_PARAMETER_VAL_TYPE;
  dst.ran_param[1].ran_param_val.lst = calloc(1, sizeof(ran_param_list_t));
  assert(dst.ran_param[1].ran_param_val.lst != NULL && "Memory exhausted");
  ran_param_list_t* rpl = dst.ran_param[1].ran_param_val.lst;

  rpl->sz_lst_ran_param = 1;
  rpl->lst_ran_param = calloc(1, sizeof(lst_ran_param_t));
  assert(rpl->lst_ran_param != NULL && "Memory exhausted");

  // QoS Flow Item
  rpl->lst_ran_param[0].ran_param_id = QOS_FLOW_ITEM_8_4_2_2;
  rpl->lst_ran_param[0].ran_param_struct.sz_ran_param_struct = 2;
  rpl->lst_ran_param[0].ran_param_struct.ran_param_struct = calloc(2, sizeof(seq_ran_param_t));
  assert(rpl->lst_ran_param[0].ran_param_struct.ran_param_struct != NULL && "Memory exhausted");
  seq_ran_param_t* rps = rpl->lst_ran_param[0].ran_param_struct.ran_param_struct ;

  // QoS Flow Identifier
  rps[0].ran_param_id = QOS_FLOW_ID_8_4_2_2;
  rps[0].ran_param_val.type = ELEMENT_KEY_FLAG_TRUE_RAN_PARAMETER_VAL_TYPE;
  rps[0].ran_param_val.flag_true = calloc(1, sizeof(ran_parameter_value_t));
  assert(rps[0].ran_param_val.flag_true != NULL && "Memory exhausted");
  rps[0].ran_param_val.flag_true->type = INTEGER_RAN_PARAMETER_VALUE;
  // Let's suppose that we have QFI 10
  rps[0].ran_param_val.flag_true->int_ran = 10;

  // QoS Flow Mapping Indication
  rps[1].ran_param_id = QOS_FLOW_MAPPING_IND_8_4_2_2;
  rps[1].ran_param_val.type = ELEMENT_KEY_FLAG_FALSE_RAN_PARAMETER_VAL_TYPE;
  rps[1].ran_param_val.flag_false = calloc(1, sizeof(ran_parameter_value_t));
  assert(rps[1].ran_param_val.flag_false != NULL && "Memory exhausted");

  // ENUMERATED (ul, dl, ...)
  rps[1].ran_param_val.flag_false->type = INTEGER_RAN_PARAMETER_VALUE;
  rps[1].ran_param_val.flag_false->int_ran = 1;

  return dst;
}

static
e2sm_rc_ctrl_msg_t gen_rc_ctrl_msg(void)
{
  e2sm_rc_ctrl_msg_t dst = {0};

  // Radio Bearer Control
  dst.format = FORMAT_1_E2SM_RC_CTRL_MSG;
  const int drb = 5;
  dst.frmt_1 = gen_rc_ctrl_msg_frmt_1_qos_flow_map(drb);

  return dst;
}

static
e2sm_rc_ctrl_msg_t gen_rc_ctrl_msg_release_bearer(void)
{
  e2sm_rc_ctrl_msg_t dst = {0};

  // Radio Bearer Control
  dst.format = FORMAT_1_E2SM_RC_CTRL_MSG;
  const int drb = 4;
  dst.frmt_1 = gen_rc_ctrl_msg_frmt_1_qos_flow_map(drb);

  return dst;
}

__attribute__((visibility("default")))
size_t rc_encode_rc_ric_ctl_hdr(void* out, size_t len)
{
  e2sm_rc_ctrl_hdr_t hdr = gen_rc_ctrl_hdr();
  byte_array_t ba = rc_enc_ctrl_hdr_asn(&hdr);

  for(size_t i = 0; i < ba.len; ++i){
  	fprintf(stderr, "0x%02hhx ", ba.buf[i]);
  }

  assert(len > ba.len);
  memcpy(out, ba.buf, ba.len); 
  free_e2sm_rc_ctrl_hdr(&hdr); 
  free_byte_array(ba);
  return ba.len;
}


__attribute__((visibility("default")))
size_t create_drb_rc_msg(void* out, size_t len)
{
  e2sm_rc_ctrl_msg_t msg = gen_rc_ctrl_msg();
  byte_array_t ba = rc_enc_ctrl_msg_asn(&msg);

  assert(len > ba.len);
  memcpy(out, ba.buf, ba.len); 
  free_e2sm_rc_ctrl_msg(&msg); 
  free_byte_array(ba);
  return ba.len;
}


__attribute__((visibility("default")))
size_t release_drb_rc_msg(void* out, size_t len)
{
  e2sm_rc_ctrl_msg_t msg = gen_rc_ctrl_msg_release_bearer();
  byte_array_t ba = rc_enc_ctrl_msg_asn(&msg);

  assert(len > ba.len);
  memcpy(out, ba.buf, ba.len); 
  free_e2sm_rc_ctrl_msg(&msg); 
  free_byte_array(ba);
  return ba.len;
}





