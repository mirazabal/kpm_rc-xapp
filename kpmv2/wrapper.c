#include "wrapper.h"

/*
#include "E2SM-KPM-EventTriggerDefinition-Format1.h"
#include "E2SM-KPM-EventTriggerDefinition.h"
#include "E2SM-KPM-IndicationHeader.h"
#include "E2SM-KPM-IndicationHeader-Format1.h"
#include "E2SM-KPM-IndicationMessage.h"
#include "E2SM-KPM-IndicationMessage-Format1.h"
#include "MeasurementDataItem.h"
#include "MeasurementData.h"
#include "MeasurementRecord.h"
#include "MeasurementRecordItem.h"
#include "E2SM-KPM-RANfunction-Description.h"
#include "RANfunction-Name.h"
#include "E2SM-KPM-ActionDefinition.h"
#include "E2SM-KPM-ActionDefinition-Format1.h"
#include "MeasurementInfoItem.h"
#include "MeasurementType.h"
#include "LabelInfoItem.h"
#include "asn_constant.h"
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-ReportStyle-Item.h"
#include "MeasurementInfo-Action-Item.h"
*/

#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include "kpm_dec_asn.h"
#include "kpm_enc_asn.h"


#include "byte_array.h"

typedef struct{
 byte_array_t hdr;
 byte_array_t msg;
} hdr_msg_t;

static
int cnt = 0;

int64_t kpm_dec_ind_asn(void* data)
{

  hdr_msg_t* in = (hdr_msg_t*)data;

  kpm_ind_hdr_t hdr = kpm_dec_ind_hdr_asn(in->hdr.len, in->hdr.buf);
  kpm_ind_msg_t msg = kpm_dec_ind_msg_asn(in->msg.len, in->msg.buf);

  int64_t const sojourn_time = msg.frm_3.meas_report_per_ue[0].ind_msg_format_1.meas_data_lst[0].meas_record_lst[0].real_val;  

  free_kpm_ind_hdr(&hdr);
  free_kpm_ind_msg(&msg);


  fprintf(stderr, "Sojourn time KPM C %ld \n", sojourn_time);

  return sojourn_time; 

/*
  E2SM_KPM_IndicationHeader_t *pdu_hdr = calloc(1, sizeof(E2SM_KPM_IndicationHeader_t));
  assert( pdu_hdr !=NULL && "Memory exhausted" );

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_IndicationHeader, (void**)&pdu_hdr, in->hdr.buf, in->hdr.len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

  xer_fprint(stderr, &asn_DEF_E2SM_KPM_IndicationHeader, pdu_hdr);
  
  E2SM_KPM_IndicationMessage_t *pdu_msg = calloc(1, sizeof(E2SM_KPM_IndicationMessage_t));
  assert( pdu_msg !=NULL && "Memory exhausted" );

  rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_IndicationMessage, (void**)&pdu_msg, in->msg.buf, in->msg.len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

 // xer_fprint(stderr, &asn_DEF_E2SM_KPM_IndicationMessage, pdu_msg);


  free(pdu_hdr);
  free(pdu_msg);

  // Never do this!!!
  free(in->hdr.buf);
  free(in->msg.buf);
  free(in);

  ++cnt;
  if(cnt%5 == 0){
    fprintf(stderr, "Sending value for RC \n");
    return 25000000;
  }

  return 100;
  */

}


static
kpm_event_trigger_def_t gen_ev_trig(uint64_t period)
{
  kpm_event_trigger_def_t dst = {0};

  dst.type = FORMAT_1_RIC_EVENT_TRIGGER;
  dst.kpm_ric_event_trigger_format_1.report_period_ms = period;

  return dst;
}

static
meas_info_format_1_lst_t gen_meas_info_format_1_lst(const char* action)
{
  meas_info_format_1_lst_t dst = {0};

  dst.meas_type.type = NAME_MEAS_TYPE;
  // ETSI TS 128 552
  dst.meas_type.name = cp_str_to_ba(  action );

  dst.label_info_lst_len = 1;
  dst.label_info_lst = calloc(1, sizeof(label_info_lst_t));
  assert(dst.label_info_lst != NULL && "Memory exhausted");
  dst.label_info_lst[0].noLabel = calloc(1, sizeof(enum_value_e));
  assert(dst.label_info_lst[0].noLabel != NULL && "Memory exhausted");
  *dst.label_info_lst[0].noLabel = TRUE_ENUM_VALUE;

  return dst;
}

static
kpm_act_def_format_1_t gen_act_def_frmt_1(const char* action)
{
  kpm_act_def_format_1_t dst = {0};

  dst.gran_period_ms = 100;

  // [1, 65535]
  dst.meas_info_lst_len = 1;
  dst.meas_info_lst = calloc(1, sizeof(meas_info_format_1_lst_t));
  assert(dst.meas_info_lst != NULL && "Memory exhausted");

  *dst.meas_info_lst = gen_meas_info_format_1_lst(action);

  return dst;
}

static
kpm_act_def_format_4_t gen_act_def_frmt_4(const char* action)
{
  kpm_act_def_format_4_t dst = {0};

  // [1, 32768]
  dst.matching_cond_lst_len = 1;

  dst.matching_cond_lst = calloc(dst.matching_cond_lst_len, sizeof(matching_condition_format_4_lst_t));
  assert(dst.matching_cond_lst != NULL && "Memory exhausted");

  // Hack. Subscribe to all UEs with CQI greater than 0 to get a list of all available UEs in the RAN
  dst.matching_cond_lst[0].test_info_lst.test_cond_type = CQI_TEST_COND_TYPE;
  dst.matching_cond_lst[0].test_info_lst.CQI = TRUE_TEST_COND_TYPE;

  dst.matching_cond_lst[0].test_info_lst.test_cond = calloc(1, sizeof(test_cond_e));
  assert(dst.matching_cond_lst[0].test_info_lst.test_cond != NULL && "Memory exhausted");
  *dst.matching_cond_lst[0].test_info_lst.test_cond = GREATERTHAN_TEST_COND;

  dst.matching_cond_lst[0].test_info_lst.test_cond_value = calloc(1, sizeof(test_cond_value_e));
  assert(dst.matching_cond_lst[0].test_info_lst.test_cond_value != NULL && "Memory exhausted");
  *dst.matching_cond_lst[0].test_info_lst.test_cond_value =  INTEGER_TEST_COND_VALUE;
  dst.matching_cond_lst[0].test_info_lst.int_value = malloc(sizeof(int64_t));
  assert(dst.matching_cond_lst[0].test_info_lst.int_value != NULL && "Memory exhausted");
  *dst.matching_cond_lst[0].test_info_lst.int_value = 0;

  // Action definition Format 1
  dst.action_def_format_1 = gen_act_def_frmt_1(action);  // 8.2.1.2.1

  return dst;
}

static
kpm_act_def_t gen_act_def(const char* act)
{
  kpm_act_def_t dst = {0};

  dst.type = FORMAT_4_ACTION_DEFINITION;
  dst.frm_4 = gen_act_def_frmt_4(act);
  return dst;
}

__attribute__((visibility("default")))
size_t kpm_event_trigger_def(void* buf, size_t len)
{
  // KPM Event Trigger
  uint64_t period_ms = 100;
  kpm_event_trigger_def_t ev = gen_ev_trig(period_ms);

  byte_array_t ba = kpm_enc_event_trigger_asn(&ev); 
  assert(ba.len < len);

  memcpy(buf, ba.buf, ba.len);

//byte_array_t ba2 = {.len = ba.len}; 
//ba2.buf = calloc(ba.len, sizeof(uint8_t));

//  for(int i =0; i < ba.len; i+=4){
//    uint32_t b = htonl(*((uint32_t*)(&ba.buf[i])));
//    memcpy(&((uint8_t*)buf)[i], &b, sizeof(uint32_t));
//  }

  fprintf(stderr, "Event Trigger Definition ba.len %ld\n", ba.len);

  free_kpm_event_trigger_def(&ev);
  free_byte_array(ba);

  return ba.len;
}

__attribute__((visibility("default"))) 
size_t kpm_action_def(void* buf, size_t len)
{
  // KPM Action Definition
  //kpm_sub.sz_ad = 1;
  //kpm_sub.ad = calloc(1, sizeof(kpm_act_def_t));
  //assert(kpm_sub.ad != NULL && "Memory exhausted");
  const char act[] = "DRB.RlcSduDelayDl";
  kpm_act_def_t ad = gen_act_def(act);

  byte_array_t ba = kpm_enc_action_def_asn(&ad);
  assert(ba.len < len);
  memcpy(buf, ba.buf, ba.len);

  fprintf(stderr, "Action Definition ba.len %ld\n", ba.len);

  free_kpm_action_def(&ad);
  free_byte_array(ba);

  return ba.len;
}

