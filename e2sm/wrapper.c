#include <errno.h>
#include "wrapper.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"
//#include "UEID-GNB.h"


/*
#define MCC_MNC_TO_PLMNID(mCC, mNC, mNCdIGITlENGTH, oCTETsTRING)               \
do {                                                                           \
    (oCTETsTRING)->buf = calloc(3, sizeof(uint8_t));                           \
    (oCTETsTRING)->buf[0] = (MCC_MNC_DECIMAL(mCC) << 4) | MCC_HUNDREDS(mCC);   \
    (oCTETsTRING)->buf[1] = (MNC_HUNDREDS(mNC,mNCdIGITlENGTH) << 4) | MCC_MNC_DIGIT(mCC);     \
    (oCTETsTRING)->buf[2] = (MCC_MNC_DIGIT(mNC) << 4) | MCC_MNC_DECIMAL(mNC);  \
    (oCTETsTRING)->size = 3;                                                   \
} while(0)


static
BIT_STRING_t cp_amf_region_id_to_bit_string(uint8_t src)
{
  assert(src < 64);

  BIT_STRING_t dst = {.bits_unused = 0, .buf = malloc(1), .size = 1};
  assert(dst.buf != NULL);

  memcpy(dst.buf, &src, 1);

  return dst;
}

static
BIT_STRING_t cp_amf_set_id_to_bit_string(uint16_t val)
{
  assert(val < 1024);

  BIT_STRING_t dst = {0};
  dst.bits_unused = 6; // unused_bit;
  dst.size = 2;
  dst.buf = calloc(2, sizeof(uint8_t) );
  assert(dst.buf != NULL);

  dst.buf[0] = val; // 0x5555;
  dst.buf[1] = (val >> 8) << 6;

  return dst;
}

static
BIT_STRING_t cp_amf_ptr_to_bit_string(uint8_t src)
{
  assert(src < 64);

  uint8_t tmp = src << 2;

  BIT_STRING_t dst = {.bits_unused =2, .buf = malloc(1), .size = 1};
  assert(dst.buf != NULL);
  memcpy(dst.buf, &tmp, 1);

  return dst;
}


static
UEID_GNB_t* enc_gNB_UE_asn(const gnb_e2sm_t* gnb)
{
  UEID_GNB_t* gnb_asn = calloc(1, sizeof(UEID_GNB_t));
  assert(gnb_asn != NULL && "Memory exhausted");

  // 6.2.3.16
  // Mandatory
  // AMF UE NGAP ID
  // INTEGER (0..2^40 -1)
  // assert(gnb->amf_ue_ngap_id < 1UL << 40);

  gnb_asn->amf_UE_NGAP_ID.buf = calloc(5, sizeof(uint8_t));
  assert(gnb_asn->amf_UE_NGAP_ID.buf != NULL && "Memory exhausted");

  //memcpy(gnb_asn->amf_UE_NGAP_ID.buf, &gnb->amf_ue_ngap_id, 5);
  //gnb_asn->amf_UE_NGAP_ID.size = 5;
  asn_ulong2INTEGER(&gnb_asn->amf_UE_NGAP_ID, 1);

  // GUAMI
  MCC_MNC_TO_PLMNID(505 , 1, 2, &gnb_asn->guami.pLMNIdentity);

  gnb_asn->guami.aMFRegionID = cp_amf_region_id_to_bit_string(1);

  gnb_asn->guami.aMFSetID = cp_amf_set_id_to_bit_string(1);

  gnb_asn->guami.aMFPointer = cp_amf_ptr_to_bit_string(1);

  return gnb_asn;
}

static
UEID_t enc_ue_id_asn(void)
{
  UEID_t ue_id_asn = {0};
  ue_id_asn.present = UEID_PR_gNB_UEID;
  ue_id_asn.choice.gNB_UEID = enc_gNB_UE_asn(&ue_id->gnb);
  return ue_id_asn;
}
*/

ssize_t e2sm_encode_ric_control_header(void *buffer, size_t buf_size, void *ueIDbuf, size_t ueIDbuf_size, long ricControlStyleType, long ricControlActionID)
{ 
        E2SM_RC_ControlHeader_t *controlHeaderIE = (E2SM_RC_ControlHeader_t *)calloc(1, sizeof(E2SM_RC_ControlHeader_t));
        if(!controlHeaderIE)
        {
                fprintf(stderr, "alloc E2SM_RC_ControlHeader failed\n");
                   return -1;
        }

        controlHeaderIE->present = E2SM_RC_ControlHeader_PR_controlHeader_Format1;
	// 			   E2SM_RC_ControlHeader__ric_controlHeader_formats_PR_controlHeader_Format1
	//
        //E2SM_RC_ControlHeader_Format1_t  *controlHeader_Fmt1 = controlHeaderIE->choice.controlHeader_Format1;
        E2SM_RC_ControlHeader_Format1_t  *controlHeader_Fmt1 = (E2SM_RC_ControlHeader_Format1_t *)calloc(1, sizeof(E2SM_RC_ControlHeader_Format1_t));
        if(!controlHeader_Fmt1)
        {
                fprintf(stderr, "alloc E2SM_RC_ControlHeader failed\n");
                return -1;
        }
     
        // controlHeader_Fmt1->ueId =  enc_ue_id_asn();
	//controlHeader_Fmt1->ueId.present =  UEID_PR_gNB_UEID;

        controlHeader_Fmt1->ueId.buf = (uint8_t*)calloc(1, ueIDbuf_size);   
        memcpy(controlHeader_Fmt1->ueId.buf, ueIDbuf, ueIDbuf_size);        //Check how to get ueIDbuf from string
        controlHeader_Fmt1->ueId.size = ueIDbuf_size;
	

        controlHeader_Fmt1->ric_ControlStyle_Type = ricControlStyleType;
        controlHeader_Fmt1->ric_ControlAction_ID = ricControlActionID;

        controlHeaderIE->choice.controlHeader_Format1 = controlHeader_Fmt1;

        fprintf(stderr, "showing xer of asn_DEF_E2SM_RC_ControlHeader data\n");
        xer_fprint(stderr, &asn_DEF_E2SM_RC_ControlHeader, controlHeaderIE);
        fprintf(stderr, "\n");
        fprintf(stderr, "After xer of asn_DEF_E2SM_RC_ControlHeader data\n");
   
        asn_enc_rval_t encode_result;
        encode_result = aper_encode_to_buffer(&asn_DEF_E2SM_RC_ControlHeader, NULL, controlHeaderIE, buffer, buf_size);
        ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlHeader, controlHeaderIE);
        if(encode_result.encoded == -1)
        {
                fprintf(stderr, "Cannot encode %s: %s\n", encode_result.failed_type->name, strerror(errno));
                return -1;
        }
        else
        {
               return encode_result.encoded;
        }
}



ssize_t e2sm_encode_ric_control_message(void *buffer, size_t buf_size, long targetPrimaryCell, 
                        long targetCell, long nrOrEUtraCell, long nrCGIOrECGI, void* ranParameterValue, size_t  ranParameterValue_size)
{
        E2SM_RC_ControlMessage_t *e2smRcControlMsg = (E2SM_RC_ControlMessage_t*)calloc(1, sizeof(E2SM_RC_ControlMessage_t));
        if(!e2smRcControlMsg) {
            fprintf(stderr, "alloc E2SM_RC_ControlMessage_t failed\n");
        return -1;
        }

        e2smRcControlMsg->present = E2SM_RC_ControlMessage_PR_controlMessage_Format1;

       // E2SM_RC_ControlMessage_Format1_t *e2smRcControlFormat1 = e2smRcControlMsg->choice.controlMessage_Format1;

        E2SM_RC_ControlMessage_Format1_t *e2smRcControlFormat1 = (E2SM_RC_ControlMessage_Format1_t*)calloc(1, sizeof(E2SM_RC_ControlMessage_Format1_t));
        if(!e2smRcControlMsg) {
            fprintf(stderr, "alloc E2SM_RC_ControlMessage_Format1_t failed\n");
        return -1;
        }
        

        e2smRcControlFormat1->ranParameters_List =  
                        (struct E2SM_RC_ControlMessage_Format1__ranParameters_List*)calloc(1, sizeof(struct E2SM_RC_ControlMessage_Format1__ranParameters_List));
	if(!e2smRcControlFormat1->ranParameters_List)
	{
                fprintf(stderr, "alloc e2smRcControlFormat1->ranParameters_List failed\n");
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                return -1;
	}

        // Start Target Primary Cell 
        RANParameter_Item_t *ranParameterItem1 = (RANParameter_Item_t*)calloc(1,sizeof(RANParameter_Item_t));
        if(!ranParameterItem1) {
                fprintf(stderr, "alloc RANParameter_Item_t1 failed\n");
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                return -1;
        }

        
        ranParameterItem1->ranParameterItem_ID = targetPrimaryCell;     // Target Primary Cell ID value = 1
        RANParameter_ValueType_t *ranParameterValueType1 = (RANParameter_ValueType_t*)calloc(1, sizeof(RANParameter_ValueType_t));
        if(!ranParameterValueType1)
        {
                fprintf(stderr, "alloc RANParameter_ValueType_t1 failed\n");
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                return -1;
        }
        
        ranParameterValueType1->present = RANParameter_ValueType_PR_ranParameter_Structure;
        RANParameter_STRUCTURE_t *ranParameterStructure1 = (RANParameter_STRUCTURE_t*)calloc(1, sizeof(RANParameter_STRUCTURE_t));
        if(!ranParameterStructure1)
        {
                fprintf(stderr, "alloc RANParameter_STRUCTURE_t1 failed\n");
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                return -1;
        }

   
         // Start Target Cell 
        RANParameter_Item_t *ranParameterItem2 = (RANParameter_Item_t*)calloc(1,sizeof(RANParameter_Item_t));
        if(!ranParameterItem2)
        {

                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t2 failed\n");
                return -1;
        }

        ranParameterItem2->ranParameterItem_ID = targetCell;    // Target Cell ID value = 2
        RANParameter_ValueType_t *ranParameterValueType2 = (RANParameter_ValueType_t*)calloc(1, sizeof(RANParameter_ValueType_t));
        if(!ranParameterValueType2)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_STRUCTURE_t2 failed\n");
                return -1;
        }
        

        ranParameterValueType2->present = RANParameter_ValueType_PR_ranParameter_Structure;
        RANParameter_STRUCTURE_t *ranParameterStructure2 = (RANParameter_STRUCTURE_t*)calloc(1, sizeof(struct RANParameter_STRUCTURE));
        if(!ranParameterStructure2)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_STRUCTURE_t2 failed\n");
                return -1;
        }

      
        // Start NR Cell  (or E-UTRA Cell) 
        RANParameter_Item_t *ranParameterItem3 = (RANParameter_Item_t*)calloc(1,sizeof(RANParameter_Item_t));
        if(!ranParameterItem3)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t3 failed\n");
                return -1;
        }

        ranParameterItem3->ranParameterItem_ID = nrOrEUtraCell; // NR Cell ID (or E-UTRA Cell ID) value = 
        RANParameter_ValueType_t *ranParameterValueType3 = (RANParameter_ValueType_t*)calloc(1, sizeof(RANParameter_ValueType_t));
        if(!ranParameterValueType3)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t3 failed\n");
                return -1;
        }

        ranParameterValueType3->present = RANParameter_ValueType_PR_ranParameter_Structure;
        RANParameter_STRUCTURE_t *ranParameterStructure3 = (struct RANParameter_STRUCTURE*)calloc(1, sizeof(struct RANParameter_STRUCTURE));
        if(!ranParameterStructure3)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t3 failed\n");
                return -1;
        }

        // Start NR CGI (or ECGI) 
        RANParameter_Item_t *ranParameterItem4 = (RANParameter_Item_t*)calloc(1, sizeof(RANParameter_Item_t));
        if(!ranParameterItem4)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t4 failed\n");
                return -1;
        }
        ranParameterItem4->ranParameterItem_ID = nrCGIOrECGI;   // NR CGI ID (or ECGI ID) value = 
        RANParameter_ValueType_t *ranParameterValueType4 = (RANParameter_ValueType_t*)calloc(1, sizeof(RANParameter_ValueType_t));
        if(!ranParameterValueType4)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t4 failed\n");
                return -1;
        }
        ranParameterValueType4->present = RANParameter_ValueType_PR_ranParameter_Element;
        ranParameterValueType4->choice.ranParameter_Element = (RANParameter_ELEMENT_t*)calloc(1, sizeof(RANParameter_ELEMENT_t));
        if(!ranParameterValueType4->choice.ranParameter_Element)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter_Item_t4 failed\n");
                return -1;
        }
        ranParameterValueType4->choice.ranParameter_Element->keyFlag = 0;
        ranParameterValueType4->choice.ranParameter_Element->ranParameter_Value.present = RANParameter_Value_PR_valueOctS;

        ranParameterValueType4->choice.ranParameter_Element->ranParameter_Value.choice.valueOctS.buf = 
                                                (uint8_t*)calloc(1, ranParameterValue_size);
        if(!ranParameterValueType4->choice.ranParameter_Element->ranParameter_Value.choice.valueOctS.buf)
        {
                ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
                fprintf(stderr, "alloc RANParameter Value failed\n");
                return -1;
        }
	memcpy(ranParameterValueType4->choice.ranParameter_Element->ranParameter_Value.choice.valueOctS.buf, ranParameterValue, ranParameterValue_size);
        ranParameterValueType4->choice.ranParameter_Element->ranParameter_Value.choice.valueOctS.size = ranParameterValue_size;
        ranParameterItem4->ranParameterItem_valueType = ranParameterValueType4; // NR CGI (or ECGI)
        
        ASN_SEQUENCE_ADD(&ranParameterStructure3->sequence_of_ranParameters.list, ranParameterItem4);   // NR CGI (or ECGI)
        ranParameterValueType3->choice.ranParameter_Structure = ranParameterStructure3; // NR Cell  (or E-UTRA Cell)
        ranParameterItem3->ranParameterItem_valueType = ranParameterValueType3;

        ASN_SEQUENCE_ADD(&ranParameterStructure2->sequence_of_ranParameters.list, ranParameterItem3);   // NR Cell  (or E-UTRA Cell)
        ranParameterValueType2->choice.ranParameter_Structure = ranParameterStructure2; // Target Cell
        ranParameterItem2->ranParameterItem_valueType = ranParameterValueType2;

        ASN_SEQUENCE_ADD(&ranParameterStructure1->sequence_of_ranParameters.list, ranParameterItem2);   // Target Cell
        ranParameterValueType1->choice.ranParameter_Structure = ranParameterStructure1; // Target Primary Cell
        ranParameterItem1->ranParameterItem_valueType = ranParameterValueType1;

        ASN_SEQUENCE_ADD(&e2smRcControlFormat1->ranParameters_List->list, ranParameterItem1); // Target Primary Cell
        e2smRcControlMsg->choice.controlMessage_Format1 = e2smRcControlFormat1;


        fprintf(stderr, "showing xer of asn_DEF_E2SM_RC_ControlMessage data\n");
        xer_fprint(stderr, &asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
        fprintf(stderr, "\n");
        fprintf(stderr, "After xer of asn_DEF_E2SM_RC_ControlMessage data\n");

        asn_enc_rval_t encode_result;
        encode_result = aper_encode_to_buffer(&asn_DEF_E2SM_RC_ControlMessage, NULL, e2smRcControlMsg, buffer, buf_size);
        ASN_STRUCT_FREE(asn_DEF_E2SM_RC_ControlMessage, e2smRcControlMsg);
        if(encode_result.encoded == -1)
        {
                fprintf(stderr, "Cannot encode %s: %s\n", encode_result.failed_type->name, strerror(errno));
                return -1;
        }
        else
        {
                return encode_result.encoded;
        }
}

