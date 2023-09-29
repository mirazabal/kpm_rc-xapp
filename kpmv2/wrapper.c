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
#include <stdio.h>
#include "kpm_dec_asn.h"

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



