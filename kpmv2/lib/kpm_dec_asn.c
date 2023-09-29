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

#include "dec_ric_event_trigger_frm_1.h"

#include "dec_ric_action_def_frm_1.h"
#include "dec_ric_action_def_frm_2.h"
#include "dec_ric_action_def_frm_3.h"
#include "dec_ric_action_def_frm_4.h"
#include "dec_ric_action_def_frm_5.h"

#include "dec_ric_ind_hdr_frm_1.h"

#include "dec_ric_ind_msg_frm_1.h"
#include "dec_ric_ind_msg_frm_2.h"
#include "dec_ric_ind_msg_frm_3.h"

#include "kpm_dec_asn.h"
#include "conversions.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


kpm_event_trigger_def_t kpm_dec_event_trigger_asn(size_t len, uint8_t const ev_tr[len])
{
  assert(len>0);
  assert(ev_tr != NULL);

  E2SM_KPM_EventTriggerDefinition_t *pdu = calloc(1, sizeof(E2SM_KPM_EventTriggerDefinition_t));
  assert( pdu !=NULL && "Memory exhausted" );

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  const asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_EventTriggerDefinition, (void**)&pdu, ev_tr, len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

  // xer_fprint(stderr, &asn_DEF_E2SM_KPM_EventTriggerDefinition, pdu);

  kpm_event_trigger_def_t ret = {0};

  switch (pdu->eventDefinition_formats.present)
  {
  case E2SM_KPM_EventTriggerDefinition__eventDefinition_formats_PR_eventDefinition_Format1:
    ret.type = FORMAT_1_RIC_EVENT_TRIGGER;
    ret.kpm_ric_event_trigger_format_1 = kpm_dec_event_trigger_def_frm_1_asn(pdu->eventDefinition_formats.choice.eventDefinition_Format1);
    break;
  
  default:
    assert("Non valid KPM RIC Event Trigger Format");
  }


  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_KPM_EventTriggerDefinition,pdu);
  free(pdu);

  return ret;
}


////// HACK for OSC. Do not use this. Ever.
////// HACK for OSC. Do not use this. Ever.
////// HACK for OSC. Do not use this. Ever.
////// HACK for OSC. Do not use this. Ever.
////// HACK for OSC. Do not use this. Ever.
////// HACK for OSC. Do not use this. Ever.

static
byte_array_t copy_str_to_ba(const char* str)
{
  assert(str != NULL);

  size_t const sz = strlen(str);
  byte_array_t dst = {.len = sz };
  dst.buf = calloc(sz ,sizeof(uint8_t) );
  assert(dst.buf != NULL);

  memcpy(dst.buf, str, sz);

  return dst;
}



static
meas_info_format_1_lst_t gen_meas_info_format_1_lst(const char* action)
{
  meas_info_format_1_lst_t dst = {0};

  dst.meas_type.type = NAME_MEAS_TYPE;
  // ETSI TS 128 552
  dst.meas_type.name = copy_str_to_ba(  action );

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

////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.
////// END HACK for OSC. Do not use this. Ever.


kpm_act_def_t kpm_dec_action_def_asn(size_t len, uint8_t const action_def[len]) 
{
  assert(len>0);
  assert(action_def != NULL);

  kpm_act_def_t ret = {0};

  // Hack for OSC RIC
  if(len == 1){
    const char act[] = "DRB.RlcSduDelayDl";
    ret = gen_act_def(act);
    return ret; 
  }


  E2SM_KPM_ActionDefinition_t *pdu = calloc(1, sizeof(E2SM_KPM_ActionDefinition_t));
  assert( pdu !=NULL && "Memory exhausted" );
  
  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  const asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_ActionDefinition, (void**)&pdu, action_def, len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");
  // note that constraints checking on data extracted is already performed by asn_decode()

  // xer_fprint(stderr, &asn_DEF_E2SM_KPM_ActionDefinition, pdu);

  
  switch (pdu->actionDefinition_formats.present)
  {
    case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format1:
      ret.type = FORMAT_1_ACTION_DEFINITION;
      ret.frm_1 = kpm_dec_action_def_frm_1_asn(pdu->actionDefinition_formats.choice.actionDefinition_Format1);
      break;

    case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format2:
      ret.type = FORMAT_2_ACTION_DEFINITION;
      ret.frm_2 = kpm_dec_action_def_frm_2_asn(pdu->actionDefinition_formats.choice.actionDefinition_Format2);
      break;

    case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format3:
      ret.type = FORMAT_3_ACTION_DEFINITION;
      ret.frm_3 = kpm_dec_action_def_frm_3_asn(pdu->actionDefinition_formats.choice.actionDefinition_Format3);
      break;

    case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format4:
      ret.type = FORMAT_4_ACTION_DEFINITION;
      ret.frm_4 = kpm_dec_action_def_frm_4_asn(pdu->actionDefinition_formats.choice.actionDefinition_Format4);
      break;
      
    case E2SM_KPM_ActionDefinition__actionDefinition_formats_PR_actionDefinition_Format5:
      ret.type = FORMAT_5_ACTION_DEFINITION;
      ret.frm_5 = kpm_dec_action_def_frm_5_asn(pdu->actionDefinition_formats.choice.actionDefinition_Format5);
      break;

    default:
      assert(false && "Non valid KPM RIC Action Definition Format");
  }
  
  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_KPM_ActionDefinition, pdu);
  free(pdu);
  return ret;
  
}

kpm_ind_hdr_t kpm_dec_ind_hdr_asn(size_t len, uint8_t const ind_hdr[len])
{
  assert(len>0);
  assert(ind_hdr != NULL);

  kpm_ind_hdr_t ret = {0};

  E2SM_KPM_IndicationHeader_t *pdu = calloc(1, sizeof(E2SM_KPM_IndicationHeader_t));
  assert( pdu !=NULL && "Memory exhausted" );

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  const asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_IndicationHeader, (void**)&pdu, ind_hdr, len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

  // xer_fprint(stderr, &asn_DEF_E2SM_KPM_IndicationHeader, pdu);


  switch (pdu->indicationHeader_formats.present)
  {
  case E2SM_KPM_IndicationHeader__indicationHeader_formats_PR_indicationHeader_Format1:
    ret.type = FORMAT_1_INDICATION_HEADER;
    ret.kpm_ric_ind_hdr_format_1 = kpm_dec_ind_hdr_frm_1_asn(pdu->indicationHeader_formats.choice.indicationHeader_Format1);
    break;
  
  default:
    assert(false && "Non valid KPM RIC Indication Header Format");
  }


	ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_KPM_IndicationHeader, pdu);
  free(pdu);

  return ret;
}



kpm_ind_msg_t kpm_dec_ind_msg_asn(size_t len, uint8_t const ind_msg[len])
{
  assert(ind_msg != NULL);
  assert(len>0);

  kpm_ind_msg_t ret = {0};

  E2SM_KPM_IndicationMessage_t *pdu = calloc(1, sizeof(E2SM_KPM_IndicationMessage_t));
  assert( pdu !=NULL && "Memory exhausted" );

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  const asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_IndicationMessage, (void**)&pdu, ind_msg, len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

  // xer_fprint(stderr, &asn_DEF_E2SM_KPM_IndicationMessage, pdu);


  switch (pdu->indicationMessage_formats.present)
  {
  case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format1:
    ret.type = FORMAT_1_INDICATION_MESSAGE;
    ret.frm_1 = kpm_dec_ind_msg_frm_1_asn(pdu->indicationMessage_formats.choice.indicationMessage_Format1);
    break;
  
  case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format2:
    ret.type = FORMAT_2_INDICATION_MESSAGE;
    ret.frm_2 = kpm_dec_ind_msg_frm_2_asn(pdu->indicationMessage_formats.choice.indicationMessage_Format2);
    break;

  case E2SM_KPM_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format3:
    ret.type = FORMAT_3_INDICATION_MESSAGE;
    ret.frm_3 = kpm_dec_ind_msg_frm_3_asn(pdu->indicationMessage_formats.choice.indicationMessage_Format3);
    break;

  default:
    assert(false && "Non valid KPM RIC Indication Message Format");
  }


  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_KPM_IndicationMessage, pdu);
  free(pdu); 

  return ret;
}

kpm_ran_function_def_t kpm_dec_func_def_asn(size_t len, uint8_t const func_def[len])
{
  assert(func_def != NULL);
  assert(len>0);

  kpm_ran_function_def_t ret = {0};

  E2SM_KPM_RANfunction_Description_t *pdu = calloc(1, sizeof(E2SM_KPM_RANfunction_Description_t));
  assert( pdu !=NULL && "Memory exhausted" );

  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  const asn_dec_rval_t rval = asn_decode(NULL, syntax, &asn_DEF_E2SM_KPM_RANfunction_Description, (void**)&pdu, func_def, len);
  assert(rval.code == RC_OK && "Are you sending data in ATS_ALIGNED_BASIC_PER syntax?");

  // xer_fprint(stderr, &asn_DEF_E2SM_KPM_RANfunction_Description, pdu);
  

  //  RAN Function Name
  ret.name.description.len = pdu->ranFunction_Name.ranFunction_Description.size;
  ret.name.description.buf = malloc(ret.name.description.len);
  memcpy(ret.name.description.buf, pdu->ranFunction_Name.ranFunction_Description.buf, ret.name.description.len);

  ret.name.oid.len = pdu->ranFunction_Name.ranFunction_E2SM_OID.size;
  ret.name.oid.buf = calloc(1, ret.name.oid.len);
  memcpy(ret.name.oid.buf, pdu->ranFunction_Name.ranFunction_E2SM_OID.buf, ret.name.oid.len);
  
  ret.name.name.len = pdu->ranFunction_Name.ranFunction_ShortName.size;
  ret.name.name.buf = calloc(1, ret.name.name.len);
  assert(ret.name.name.buf != NULL && "Memory exhausted");
  memcpy(ret.name.name.buf, pdu->ranFunction_Name.ranFunction_ShortName.buf, ret.name.name.len);
  
  if (pdu->ranFunction_Name.ranFunction_Instance != NULL)
  {
    ret.name.instance = calloc(1, sizeof(*ret.name.instance));
    assert(ret.name.instance != NULL && "Memory exhausted");
    ret.name.instance = pdu->ranFunction_Name.ranFunction_Instance;
  }
  else
  {
    ret.name.instance = NULL;
  }


  //  RIC Event Trigger Style Item
  if (pdu->ric_EventTriggerStyle_List != NULL)
  {
    assert(pdu->ric_EventTriggerStyle_List->list.count >=1 && pdu->ric_EventTriggerStyle_List->list.count <= maxnoofRICStyles);
    ret.sz_ric_event_trigger_style_list = pdu->ric_EventTriggerStyle_List->list.count;

    ret.ric_event_trigger_style_list = calloc(ret.sz_ric_event_trigger_style_list, sizeof(ric_event_trigger_style_item_t));
    assert(ret.ric_event_trigger_style_list != NULL && "Memory exhausted");

    for (size_t i = 0; i<ret.sz_ric_event_trigger_style_list; i++)
    {
      RIC_EventTriggerStyle_Item_t * event_item = pdu->ric_EventTriggerStyle_List->list.array[i];

      switch (event_item->ric_EventTriggerStyle_Type)
      {
      case 1:
      {
        ret.ric_event_trigger_style_list[i].style_type = STYLE_1_RIC_EVENT_TRIGGER;

        // RIC Event Trigger Style Name
        ret.ric_event_trigger_style_list[i].style_name.len = event_item->ric_EventTriggerStyle_Name.size;
        ret.ric_event_trigger_style_list[i].style_name.buf = malloc(ret.ric_event_trigger_style_list[i].style_name.len);
        memcpy(ret.ric_event_trigger_style_list[i].style_name.buf, event_item->ric_EventTriggerStyle_Name.buf, ret.ric_event_trigger_style_list[i].style_name.len);

        // RIC Event Trigger Format
        ret.ric_event_trigger_style_list[i].format_type = FORMAT_1_RIC_EVENT_TRIGGER;
        break;
      }
      
      default:
        assert(false && "Unknown RIC Event Trigger Style Type");
      }
    }
  }
  else
  {
    ret.sz_ric_event_trigger_style_list = 0;
    ret.ric_event_trigger_style_list = NULL;
  }


  // RIC Report Style Item
  if (pdu->ric_ReportStyle_List != NULL)
  {
    assert(pdu->ric_ReportStyle_List->list.count >=1 && pdu->ric_ReportStyle_List->list.count <= maxnoofRICStyles);
    ret.sz_ric_report_style_list = pdu->ric_ReportStyle_List->list.count;

    ret.ric_report_style_list = calloc(ret.sz_ric_report_style_list, sizeof(ric_report_style_item_t));
    assert(ret.ric_report_style_list != NULL && "Memory exhausted");

    for (size_t i = 0; i<ret.sz_ric_report_style_list; i++)
    {
      RIC_ReportStyle_Item_t * report_item = pdu->ric_ReportStyle_List->list.array[i];

      switch (report_item->ric_ReportStyle_Type)
      {
      case 1:
      {
        ret.ric_report_style_list[i].report_style_type = STYLE_1_RIC_SERVICE_REPORT;
        ret.ric_report_style_list[i].act_def_format_type = FORMAT_1_ACTION_DEFINITION;
        ret.ric_report_style_list[i].ind_hdr_format_type = FORMAT_1_INDICATION_HEADER;
        ret.ric_report_style_list[i].ind_msg_format_type = FORMAT_1_INDICATION_MESSAGE;

        break;
      }

      case 2:
      {
        ret.ric_report_style_list[i].report_style_type = STYLE_2_RIC_SERVICE_REPORT;
        ret.ric_report_style_list[i].act_def_format_type = FORMAT_2_ACTION_DEFINITION;
        ret.ric_report_style_list[i].ind_hdr_format_type = FORMAT_1_INDICATION_HEADER;
        ret.ric_report_style_list[i].ind_msg_format_type = FORMAT_1_INDICATION_MESSAGE;

        break;
      }

      case 3:
      {
        ret.ric_report_style_list[i].report_style_type = STYLE_3_RIC_SERVICE_REPORT;
        ret.ric_report_style_list[i].act_def_format_type = FORMAT_3_ACTION_DEFINITION;
        ret.ric_report_style_list[i].ind_hdr_format_type = FORMAT_1_INDICATION_HEADER;
        ret.ric_report_style_list[i].ind_msg_format_type = FORMAT_2_INDICATION_MESSAGE;

        break;
      }

      case 4:
      {
        ret.ric_report_style_list[i].report_style_type = STYLE_4_RIC_SERVICE_REPORT;
        ret.ric_report_style_list[i].act_def_format_type = FORMAT_4_ACTION_DEFINITION;
        ret.ric_report_style_list[i].ind_hdr_format_type = FORMAT_1_INDICATION_HEADER;
        ret.ric_report_style_list[i].ind_msg_format_type = FORMAT_3_INDICATION_MESSAGE;

        break;
      }

      case 5:
      {
        ret.ric_report_style_list[i].report_style_type = STYLE_5_RIC_SERVICE_REPORT;
        ret.ric_report_style_list[i].act_def_format_type = FORMAT_5_ACTION_DEFINITION;
        ret.ric_report_style_list[i].ind_hdr_format_type = FORMAT_1_INDICATION_HEADER;
        ret.ric_report_style_list[i].ind_msg_format_type = FORMAT_3_INDICATION_MESSAGE;

        break;
      }
      
      default:
        assert(false && "Unknown RIC REPORT Style Type");
      }

      // RIC REPORT Style Name
      ret.ric_report_style_list[i].report_style_name.len = report_item->ric_ReportStyle_Name.size;
      ret.ric_report_style_list[i].report_style_name.buf = malloc(ret.ric_report_style_list[i].report_style_name.len);
      memcpy(ret.ric_report_style_list[i].report_style_name.buf, report_item->ric_ReportStyle_Name.buf, ret.ric_report_style_list[i].report_style_name.len);


      // Measurement Information for Action
      assert(report_item->measInfo_Action_List.list.count >= 1 && report_item->measInfo_Action_List.list.count <= maxnoofMeasurementInfo);
      ret.ric_report_style_list[i].meas_info_for_action_lst_len = report_item->measInfo_Action_List.list.count;

      ret.ric_report_style_list[i].meas_info_for_action_lst = calloc(ret.ric_report_style_list[i].meas_info_for_action_lst_len, sizeof(meas_info_for_action_lst_t));
      assert(ret.ric_report_style_list[i].meas_info_for_action_lst != NULL && "Memory exhausted");

      for (size_t j = 0; j<ret.ric_report_style_list[i].meas_info_for_action_lst_len; j++)
      {
        MeasurementInfo_Action_Item_t * meas_item = report_item->measInfo_Action_List.list.array[j];

        // Measurement Type Name
        ret.ric_report_style_list[i].meas_info_for_action_lst[j].name.len = meas_item->measName.size;
        ret.ric_report_style_list[i].meas_info_for_action_lst[j].name.buf = malloc(ret.ric_report_style_list[i].meas_info_for_action_lst[j].name.len);
        memcpy(ret.ric_report_style_list[i].meas_info_for_action_lst[j].name.buf, meas_item->measName.buf, ret.ric_report_style_list[i].meas_info_for_action_lst[j].name.len);

        // Measurement Type ID
        if (meas_item->measID != NULL)
        {
          ret.ric_report_style_list[i].meas_info_for_action_lst[j].id = calloc(1, sizeof(uint16_t));
          assert(ret.ric_report_style_list[i].meas_info_for_action_lst[j].id != NULL && "Memory exhausted");
          memcpy(ret.ric_report_style_list[i].meas_info_for_action_lst[j].id, meas_item->measID, 2);
        }

        // Bin Range Definition
        // not yet implemented in ASN.1
        if (meas_item->binRangeDef != NULL)
          assert(false && "Not yet implemented");

      }
    }
  }
  else
  {
    ret.sz_ric_report_style_list = 0;
    ret.ric_report_style_list = NULL;
  }

  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_KPM_RANfunction_Description, pdu);
  free(pdu); 
  
  return ret;
}
