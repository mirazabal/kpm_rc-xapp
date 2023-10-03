#include <errno.h>
#include "wrapper.h"
#include <e2sm/wrapper.h>

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

/*
size_t encode_E2AP_PDU(E2AP_PDU_t* pdu, void* buf, size_t len)
{
    assert(pdu != NULL);
    assert(buf != NULL);
   // xer_fprint(stderr, &asn_DEF_E2AP_PDU, pdu);
    const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
    asn_enc_rval_t er = asn_encode_to_buffer(NULL, syntax, &asn_DEF_E2AP_PDU, pdu, buf, len);
    assert(er.encoded < (ssize_t) len);
    if(er.encoded == -1) {
      printf("Failed the encoding in type %s and xml_type = %s\n", er.failed_type->name, er.failed_type->xml_tag);
      fflush(stdout);
      return 0;
    }
    fprintf(stderr, "Len encoded e2ap_wrapper %d\n", er.encoded);
    //assert(er.encoded > -1); 
    //b->len = er.encoded;
    return er.encoded;
}
*/

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


typedef struct {
  size_t len;
  uint8_t *buf;
} byte_array_t;


typedef struct{
 byte_array_t hdr;
 byte_array_t msg;
} hdr_msg_t;


static inline
byte_array_t copy_ostring_to_ba(OCTET_STRING_t src)
{
  assert(src.size > 0 && src.buf != NULL);

  byte_array_t dst = { .len = src.size};
  dst.buf = malloc(src.size);
  memcpy(dst.buf, src.buf, src.size);
  return dst;
}

// E2 -> RIC
static
hdr_msg_t e2ap_dec_indication(const E2AP_PDU_t* pdu)
{
  assert(pdu != NULL);

  hdr_msg_t ret = {0}; 

//  e2ap_msg_t ret = {.type = RIC_INDICATION};
//  ric_indication_t* ind = &ret.u_msgs.ric_ind;

  assert(pdu->present == E2AP_PDU_PR_initiatingMessage);
  assert(pdu->choice.initiatingMessage->procedureCode == ProcedureCode_id_RICindication);
  //assert(pdu->choice.initiatingMessage->criticality == Criticality_reject);
  assert(pdu->choice.initiatingMessage->value.present == InitiatingMessage__value_PR_RICindication);

  const RICindication_t* out = &pdu->choice.initiatingMessage->value.choice.RICindication;

  // RIC Request ID. Mandatory
  const RICindication_IEs_t* sub_req = out->protocolIEs.list.array[0];
  assert(sub_req->id == ProtocolIE_ID_id_RICrequestID);
  assert(sub_req->criticality == Criticality_reject);
  assert(sub_req->value.present == RICindication_IEs__value_PR_RICrequestID);

  //assert(sub_req->value.choice.RICrequestID.ricInstanceID < MAX_RIC_INSTANCE_ID);
  //ind->ric_id.ric_inst_id = sub_req->value.choice.RICrequestID.ricInstanceID;
  //assert(sub_req->value.choice.RICrequestID.ricRequestorID < MAX_RIC_REQUEST_ID);
  //ind->ric_id.ric_req_id = sub_req->value.choice.RICrequestID.ricRequestorID;

  //RAN Function ID. Mandatory
  const RICindication_IEs_t* ran_id = out->protocolIEs.list.array[1];
  assert(ran_id->id == ProtocolIE_ID_id_RANfunctionID);
  assert(ran_id->criticality == Criticality_reject);
  assert(ran_id->value.present == RICindication_IEs__value_PR_RANfunctionID);
  //assert(ran_id->value.choice.RANfunctionID < MAX_RAN_FUNC_ID);
  //ind->ric_id.ran_func_id = ran_id->value.choice.RANfunctionID;

  // RIC Action ID. Mandatory
  const RICindication_IEs_t* ric_act = out->protocolIEs.list.array[2];
  assert(ric_act->id == ProtocolIE_ID_id_RICactionID);
  assert(ric_act->criticality == Criticality_reject);
  assert(ric_act->value.present == RICindication_IEs__value_PR_RICactionID);
  //assert(ric_act->value.choice.RICactionID < MAX_RIC_ACTION_ID);
  //ind->action_id = ric_act->value.choice.RICactionID;


  int elm = 3;
  while(elm < out->protocolIEs.list.count){
    const RICindication_IEs_t* ric_ind = out->protocolIEs.list.array[elm];
    const ProtocolIE_ID_t id = ric_ind->id;
    assert(id == ProtocolIE_ID_id_RICindicationSN ||
        id == ProtocolIE_ID_id_RICindicationType ||
        id ==  ProtocolIE_ID_id_RICindicationHeader ||
        id ==  ProtocolIE_ID_id_RICindicationMessage ||
        id ==  ProtocolIE_ID_id_RICcallProcessID );

    if(id == ProtocolIE_ID_id_RICindicationSN){
      //RIC indication SN. Optional
      assert(ric_ind->criticality == Criticality_reject);
      assert(ric_ind->value.present == RICindication_IEs__value_PR_RICindicationSN);
      //ind->sn =  malloc(sizeof(uint16_t));
      //assert(ric_ind->value.choice.RICindicationSN < MAX_RIC_INDICATION_SN );
      //*ind->sn = ric_ind->value.choice.RICindicationSN;
    } else if (id == ProtocolIE_ID_id_RICindicationType){
      // RIC indication Type. Mandatory
      assert(ric_ind->criticality == Criticality_reject);
      assert(ric_ind->value.present == RICindication_IEs__value_PR_RICindicationType);
      //assert(ric_ind->value.choice.RICindicationType < MAX_RIC_INDICATION_TYPE);
      //ind->type = ric_ind->value.choice.RICindicationType;
    } else if (id == ProtocolIE_ID_id_RICindicationHeader){
      //RIC indication header. Mandatory
      assert(ric_ind->id == ProtocolIE_ID_id_RICindicationHeader);
      assert(ric_ind->criticality == Criticality_reject);
      assert(ric_ind->value.present == RICindication_IEs__value_PR_RICindicationHeader);
      //ind->hdr = copy_ostring_to_ba(ric_ind->value.choice.RICindicationHeader);
      ret.hdr = copy_ostring_to_ba(ric_ind->value.choice.RICindicationHeader);
    } else if (id == ProtocolIE_ID_id_RICindicationMessage){
      // RIC indication message. Mandatory
      assert(ric_ind->criticality == Criticality_reject);
      assert(ric_ind->value.present == RICindication_IEs__value_PR_RICindicationMessage);
      //ind->msg = copy_ostring_to_ba(ric_ind->value.choice.RICindicationMessage);
      ret.msg = copy_ostring_to_ba(ric_ind->value.choice.RICindicationMessage);
    } else { // if (id == ProtocolIE_ID_id_RICcallProcessID)
      // RIC call process id. Optional
      assert(ric_ind->criticality == Criticality_reject);
      assert(ric_ind->value.present == RICindication_IEs__value_PR_RICcallProcessID);
      //ind->call_process_id = malloc(sizeof(byte_array_t));
      //*ind->call_process_id = copy_ostring_to_ba(ric_ind->value.choice.RICcallProcessID);
    }
    elm += 1;
  }


  return ret;
}

 __attribute__((visibility("default"))) 
void* print_e2ap_msg(void* buffer, size_t buf_size)
{
    E2AP_PDU_t *pdu =  decode_E2AP_PDU(buffer, buf_size);
    //xer_fprint(stderr, &asn_DEF_E2AP_PDU, pdu);

    hdr_msg_t* out = calloc(1, sizeof(hdr_msg_t  )); 
    *out = e2ap_dec_indication(pdu);

//    kpm_dec_ind_hdr_asn(out.hdr.buf, out.hdr.len);
    
    ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, pdu);
    return out;
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

    //fprintf(stderr, "showing xer of asn_DEF_E2AP_PDU data\n");
    //xer_fprint(stderr, &asn_DEF_E2AP_PDU, init);
    //fprintf(stderr, "\n");
   
    ssize_t const encoded = encode_E2AP_PDU(init, buffer, buf_size);
    fprintf(stderr, "Encoded amount of bits %ld \n", encoded);
    fflush(stderr);

    return  encoded;
}

                                            
