#include <errno.h>
#include "wrapper.h"

size_t encode_E2AP_PDU(E2AP_PDU_t* pdu, void* buffer, size_t buf_size)
{
    asn_enc_rval_t encode_result;
    encode_result = aper_encode_to_buffer(&asn_DEF_E2AP_PDU, NULL, pdu, buffer, buf_size);
    ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, pdu);
    if(encode_result.encoded == -1) {
        fprintf(stderr, "Cannot encode %s: %s\n", encode_result.failed_type->name, strerror(errno));
        return -1;
    } else {
          return encode_result.encoded;
    }
}

E2AP_PDU_t* decode_E2AP_PDU(const void* buffer, size_t buf_size)
{
    asn_dec_rval_t decode_result;
    E2AP_PDU_t *pdu = 0;
    decode_result = aper_decode_complete(NULL, &asn_DEF_E2AP_PDU, (void **)&pdu, buffer, buf_size);
    if(decode_result.code == RC_OK) {
        return pdu;
    } else {
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, pdu);
        return 0;
    }
}

ssize_t e2ap_encode_ric_control_request_message(void *buffer, size_t buf_size, long ricRequestorID, long ricRequestSequenceNumber, 
                  long ranFunctionID, void *ricControlHdr, size_t ricControlHdrSize, void *ricControlMsg, size_t ricControlMsgSize)
{
  
    E2AP_PDU_t *init = (E2AP_PDU_t *)calloc(1, sizeof(E2AP_PDU_t));
    if(!init) {
        fprintf(stderr, "alloc E2AP_PDU failed\n");
        return -1;
    }

    
    InitiatingMessage_t *initiatingMsg = (InitiatingMessage_t *)calloc(1, sizeof(InitiatingMessage_t));
    if(!initiatingMsg) {
        fprintf(stderr, "alloc InitiatingMessage failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }

        init->choice.initiatingMessage = initiatingMsg;
    init->present = E2AP_PDU_PR_initiatingMessage;

    initiatingMsg->procedureCode = ProcedureCode_id_RICcontrol;
    initiatingMsg->criticality = Criticality_reject;
    initiatingMsg->value.present = InitiatingMessage__value_PR_RICcontrolRequest;

    RICcontrolRequest_t *control_request = &initiatingMsg->value.choice.RICcontrolRequest;


    //RICrequestID
    RICcontrolRequest_IEs_t *controlReqID = (RICcontrolRequest_IEs_t *)calloc(1, sizeof(RICcontrolRequest_IEs_t));
    if(!controlReqID) {
        fprintf(stderr, "alloc RICrequestID failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }

    controlReqID->criticality = Criticality_reject;
    controlReqID->id = ProtocolIE_ID_id_RICrequestID;

    controlReqID->value.present = RICcontrolRequest_IEs__value_PR_RICrequestID;
    RICrequestID_t *ricrequest_ie = &controlReqID->value.choice.RICrequestID;
    ricrequest_ie->ricRequestorID = ricRequestorID;
    ricrequest_ie->ricInstanceID = ricRequestSequenceNumber;
    ASN_SEQUENCE_ADD(&control_request->protocolIEs.list, controlReqID);

    //RICfunctionID
    RICcontrolRequest_IEs_t *controlReqFunID = (RICcontrolRequest_IEs_t *)calloc(1, sizeof(RICcontrolRequest_IEs_t));
    if(!controlReqFunID) {
        fprintf(stderr, "alloc RICrequestID failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }

    controlReqFunID->criticality = Criticality_reject;
    controlReqFunID->id = ProtocolIE_ID_id_RANfunctionID;
    controlReqFunID->value.present = RICcontrolRequest_IEs__value_PR_RANfunctionID;
    RANfunctionID_t *ranfunction_ie = &controlReqFunID->value.choice.RANfunctionID;
    *ranfunction_ie = ranFunctionID;
    ASN_SEQUENCE_ADD(&control_request->protocolIEs.list, controlReqFunID);

    // RICControlHdr
    RICcontrolRequest_IEs_t *controlReqHdr = (RICcontrolRequest_IEs_t *)calloc(1, sizeof(RICcontrolRequest_IEs_t));
    if(!controlReqHdr) {
        fprintf(stderr, "alloc RICcontrolRequest_IEs_t failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }
    controlReqHdr->criticality = Criticality_reject;
    controlReqHdr->id = ProtocolIE_ID_id_RICcontrolHeader;
    controlReqHdr->value.present = RICcontrolRequest_IEs__value_PR_RICcontrolHeader;
    RICcontrolHeader_t *controlHdr = &controlReqHdr->value.choice.RICcontrolHeader;
    controlHdr->buf = (uint8_t *)calloc(1, ricControlHdrSize);
    if(!controlHdr->buf) {
        fprintf(stderr, "alloc RICcontrolHeader_t buf failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }

    memcpy(controlHdr->buf, ricControlHdr, ricControlHdrSize);
    controlHdr->size = ricControlHdrSize;
    ASN_SEQUENCE_ADD(&control_request->protocolIEs.list, controlReqHdr);

    //Ric Control Message
    RICcontrolRequest_IEs_t *controlReqMsg = (RICcontrolRequest_IEs_t *)calloc(1, sizeof(RICcontrolRequest_IEs_t));
    if(!controlReqMsg) {
        fprintf(stderr, "alloc RICcontrolRequest_IEs_t failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }
    controlReqMsg->criticality = Criticality_reject;
    controlReqMsg->id = ProtocolIE_ID_id_RICcontrolMessage;
    controlReqMsg->value.present = RICcontrolRequest_IEs__value_PR_RICcontrolMessage;
    RICcontrolMessage_t *controlMsg = &controlReqMsg->value.choice.RICcontrolMessage;
    controlMsg->buf = (uint8_t *)calloc(1, ricControlMsgSize);
    if(!controlMsg->buf) {
        fprintf(stderr, "alloc RICcontrolMessage_t buf failed\n");
        ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, init);
        return -1;
    }

    memcpy(controlMsg->buf, ricControlMsg, ricControlMsgSize);
    controlMsg->size = ricControlMsgSize;
    ASN_SEQUENCE_ADD(&control_request->protocolIEs.list, controlReqMsg);

    fprintf(stderr, "showing xer of asn_DEF_E2AP_PDU data\n");
    xer_fprint(stderr, &asn_DEF_E2AP_PDU, init);
    fprintf(stderr, "\n");
    fprintf(stderr, "After xer of asn_DEF_E2AP_PDU data\n");
   
    return encode_E2AP_PDU(init, buffer, buf_size);
}

                                            
