#include <errno.h>
#include "wrapper.h"
#include "OCTET_STRING.h"


ssize_t e2sm_encode_ric_control_header(void *buffer, size_t buf_size, void *ueIDbuf, size_t ueIDbuf_size, long ricControlStyleType, long ricControlActionID)
{ 
        E2SM_RC_ControlHeader_t *controlHeaderIE = (E2SM_RC_ControlHeader_t *)calloc(1, sizeof(E2SM_RC_ControlHeader_t));
        if(!controlHeaderIE)
        {
                fprintf(stderr, "alloc E2SM_RC_ControlHeader failed\n");
                   return -1;
        }

        controlHeaderIE->present = E2SM_RC_ControlHeader_PR_controlHeader_Format1;
        //E2SM_RC_ControlHeader_Format1_t  *controlHeader_Fmt1 = controlHeaderIE->choice.controlHeader_Format1;
        E2SM_RC_ControlHeader_Format1_t  *controlHeader_Fmt1 = (E2SM_RC_ControlHeader_Format1_t *)calloc(1, sizeof(E2SM_RC_ControlHeader_Format1_t));
        if(!controlHeader_Fmt1)
        {
                fprintf(stderr, "alloc E2SM_RC_ControlHeader failed\n");
                return -1;
        }
        
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

