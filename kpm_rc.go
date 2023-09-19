package main

/*
#include <stdlib.h>
#include <e2ap/wrapper.h>
#cgo CFLAGS: -I/usr/local/include/e2ap 
#cgo LDFLAGS: -le2apwrapper 
*/
import "C"

/*
#include <e2sm/wrapper.h>
#cgo LDFLAGS: -le2smwrapper -lm
#cgo CFLAGS: -I/usr/local/include/e2sm
*/
import "C"

import (
  "fmt"
  "sync"
  "os"
  "log"
  "encoding/json"
  "gerrit.o-ran-sc.org/r/ric-plt/xapp-frame/pkg/xapp"
  "gerrit.o-ran-sc.org/r/ric-plt/xapp-frame/pkg/clientmodel"
  "github.com/spf13/viper"
  "gerrit.o-ran-sc.org/r/ric-app/rc/protocol/grpc/ricmsgcommrpc/rc"
  "unsafe"
  "errors"
  "time"
)

var (
	reqId                = int64(1)
	seqId                = int64(1)
	funId                = int64(1)
	actionId             = int64(1)
	actionType           = "report"
	subsequestActioType  = "continue"
	timeToWait           = "w10ms"
	direction            = int64(0)
	procedureCode        = int64(27)
	xappEventInstanceID  = int64(1234)
	typeOfMessage        = int64(1)
	subscriptionId       = ""
	hPort                = int64(8080)
	rPort                = int64(4560)
	clientEndpoint       = clientmodel.SubscriptionParamsClientEndpoint{Host: "service-ricxapp-hw-go-rmr.ricxapp", HTTPPort: &hPort, RMRPort: &rPort}
)


const MAX_CONTROL_REQ_ATTEMPTS = 2
const RIC_CONTROL_STYLE_TYPE = 3
const RIC_CONTROL_ACTION_ID = 1

const TIME_NANOSEC = 1000000000

const RIC_CONTROL_TARGET_PRIMARY_CELL = 1
const RIC_CONTROL_TARGET_CELL = 2
const RIC_CONTROL_CELL_TYPE = 3
const RIC_CONTROL_CGI_TYPE = 4

//GRPC rsp codes
const GRPC_SUCCESS = 0
const GRPC_ERROR = -1

type Control struct {
	eventRicControlReqTimePeriod int32                //maximum time for the RIC Subscription Request event creation procedure in the E2 Node
	rcChan                       chan *xapp.RMRParams //channel for receiving rmr message
	eventRicControlReqExpiredMap map[int]bool //map for recording the RIC Subscription Request event creation procedure is expired or not
	eventRicControlReqExpiredMu  *sync.Mutex  //mutex for eventCreateExpiredMap
	ricRequestInstanceID         int
}

type RicHoControlMsg struct {
	RicControlGrpcReqPtr *rc.RicControlGrpcReq
}

type E2sm struct {
}

func (c *E2sm) SetRicControlHeader(buffer []byte, ueIDbuf []byte, ricControlStyleType int64, ricControlActionID int64) (newBuffer []byte, err error) {
	cptr := unsafe.Pointer(&buffer[0])
	cptr_ueIDbuf := unsafe.Pointer(&ueIDbuf[0])
	size := C.e2sm_encode_ric_control_header(cptr, C.size_t(len(buffer)), cptr_ueIDbuf, C.size_t(len(ueIDbuf)),
		C.long(ricControlStyleType), C.long(ricControlActionID))

	if size < 0 {
		return make([]byte, 0), errors.New("e2sm wrapper is unable to set EventTriggerDefinition due to wrong or invalid input")
	}
	newBuffer = C.GoBytes(cptr, (C.int(size)+7)/8)
	xapp.Logger.Info("E2sm SetRicControlHeader is success")
	return
}

func (c *E2sm) SetRicControlMessage(buffer []byte, targetPrimaryCell int64, targetCell int64, nrCGIOrECGI int64, nrOrEUtraCell int64, ranParameterValue []byte) (newBuffer []byte, err error) {
	cptr := unsafe.Pointer(&buffer[0])
	cptrRanParameterValue := unsafe.Pointer(&ranParameterValue[0])

	size := C.e2sm_encode_ric_control_message(cptr, C.size_t(len(buffer)), C.long(targetPrimaryCell),
		C.long(targetCell), C.long(nrOrEUtraCell), C.long(nrCGIOrECGI), cptrRanParameterValue, C.size_t(len(ranParameterValue)))
	if size < 0 {
		return make([]byte, 0), errors.New("e2sm wrapper is unable to set RicControlMessage due to wrong or invalid input")
	}
	newBuffer = C.GoBytes(cptr, (C.int(size)+7)/8) //TOCHECK: if C.int(size) is returning bits not bytes to get correct size of the buffer.
	xapp.Logger.Info("E2sm SetRicControlMessage is success")
	return
}









type E2ap struct {
}

func (c *E2ap) SetRicControlRequestPayload(payload []byte, ricRequestorID uint16, ricRequestSequenceNumber uint16, ranFunctionID uint16,
	ricControlHdr []byte, ricControlMsg []byte) (newPayload []byte, err error) {

	cptr := unsafe.Pointer(&payload[0])
	cptr_ricControlHdr := unsafe.Pointer(&ricControlHdr[0])
	cptr_ricControlMsg := unsafe.Pointer(&ricControlMsg[0])

	xapp.Logger.Debug("\n")
	xapp.Logger.Debug("ricControlHdr\n", ricControlHdr)
	xapp.Logger.Debug("\n")
	xapp.Logger.Debug("ricControlMsg\n", ricControlMsg)
	xapp.Logger.Debug("\n")

	size := C.e2ap_encode_ric_control_request_message(cptr, C.size_t(len(payload)), C.long(ricRequestorID), C.long(ricRequestSequenceNumber),
		C.long(ranFunctionID), cptr_ricControlHdr, C.size_t(len(ricControlHdr)), cptr_ricControlMsg, C.size_t(len(ricControlMsg)))
	if size < 0 {
		return make([]byte, 0), errors.New("e2ap wrapper is unable to set Subscription Request Payload due to wrong or invalid payload")
	}
	newPayload = C.GoBytes(cptr, (C.int(size)+7)/8)
	xapp.Logger.Info("SetRicControlHeader is success")
	return
}


type HWApp struct {
	stats map[string]xapp.Counter
}



//func (c *Control) xAppStartCB(d interface{}) {
	//After rigistration complete, start to initiate the other functions.
	//xapp.Logger.Info("xApp ready call back received")

	//Initiate E2IfState
	//c.e2IfState.Init(c)
//}

//func (c *Control) Run() {
	//When xApp is ready, it will reveive Callback
//	xapp.SetReadyCB(c.xAppStartCB, true)

	//Register REST Subscription Call Back
//	xapp.Subscription.SetResponseCB(SubscriptionResponseCallback)


func (e *HWApp) getGnbList() ([]*xapp.RNIBNbIdentity, error) {
	gnbs, err := xapp.Rnib.GetListGnbIds()

	if err != nil {
		xapp.Logger.Error("err: %s", err)
		return nil, err
	}

	xapp.Logger.Info("List of connected gNBs :")
	for index, gnb := range gnbs {
		xapp.Logger.Info("%d. gnbid : %s", index+1, gnb.InventoryName)
	}
	return gnbs, nil
}

func (e *HWApp) getnbList() []*xapp.RNIBNbIdentity {
	nbs := []*xapp.RNIBNbIdentity{}

	if gnbs, err := e.getGnbList(); err == nil {
		nbs = append(nbs, gnbs...)
	}
	return nbs
}

func (e *HWApp) sendSubscription(meid string) {

	xapp.Logger.Info("sending subscription request for meid : %s", meid)

	subscriptionParams := clientmodel.SubscriptionParams{
		ClientEndpoint: &clientEndpoint,
		Meid:           &meid,
		RANFunctionID:  &funId,
		SubscriptionDetails: clientmodel.SubscriptionDetailsList([]*clientmodel.SubscriptionDetail{
			{
				ActionToBeSetupList: clientmodel.ActionsToBeSetup{
					&clientmodel.ActionToBeSetup{
						ActionDefinition: clientmodel.ActionDefinition([]int64{0,1,2,3}),
						ActionID:         &actionId,
						ActionType:       &actionType,
						SubsequentAction: &clientmodel.SubsequentAction{
							SubsequentActionType: &subsequestActioType,
							TimeToWait:           &timeToWait,
						},
					},
				},
				EventTriggers:       clientmodel.EventTriggerDefinition([]int64{0, 1 }),
				XappEventInstanceID: &xappEventInstanceID,
			},
		}),
	}

	b, err := json.MarshalIndent(subscriptionParams, "", "  ")

	if err != nil {
		xapp.Logger.Error("Json marshaling failed : %s", err)
		return
	}

	xapp.Logger.Info("*****body: %s ", string(b))

	resp, err := xapp.Subscription.Subscribe(&subscriptionParams)

	if err != nil {
		xapp.Logger.Error("subscription failed (%s) with error: %s", meid, err)
		// subscription failed, raise alarm
    panic("subscription failed")
   	}
	xapp.Logger.Info("Successfully subcription done (%s), subscription id : %s", meid, *resp.SubscriptionID)
}

func (e *HWApp) xAppStartCB(d interface{}) {
	xapp.Logger.Info("xApp ready call back received")
	time.Sleep(8 * time.Second)
	xapp.Logger.Info("After sleep xApp ready call back received")

	// get the list of all NBs
	nbList := e.getnbList()

	// send subscription request to each of the NBs
	for _, nb := range nbList {
		e.sendSubscription(nb.InventoryName)
	}
}

func send_rc(){

	aRicHoControlMsg := RicHoControlMsg{}
//	aRicHoControlMsg.RicControlGrpcReqPtr = aPtrRicControlGrpcReq
  aRicHoControlMsg.RicControlGrpcReqPtr.RanName = "gnb_505_001_00000001"
  aRicHoControlMsg.RicControlGrpcReqPtr.E2NodeID = "00000000000000000000000000000001" 
  aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.TargetCellID = "1113" 
  aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID = "00006" 
  aRicHoControlMsg.RicControlGrpcReqPtr.PlmnID = "05F510" 
  aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlActionId  = 1
  aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlStyle = 3
  aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.RICControlCellTypeVal = 4
  aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RICRequestorID  =2 
  aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RanFuncId = 3


	lUEID := aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID
	xapp.Logger.Debug("HandlegRPCRICControlMsgReq UEID = %s ", lUEID)
        //Mandatory parameters validation
        if aRicHoControlMsg.RicControlGrpcReqPtr.E2NodeID == "" ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.TargetCellID == "" ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID == "" ||
                aRicHoControlMsg.RicControlGrpcReqPtr.PlmnID == "" ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlActionId < 0 ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlStyle < 0 ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.RICControlCellTypeVal < 0 ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RICRequestorID < 0 ||
                aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RanFuncId < 0 {
		xapp.Logger.Error("Mandaroty parameters missing, dont send control request ")
		return
	}


	//aRicHoControlMsg.GetSequenceNumber()

	var e2ap *E2ap
	var e2sm *E2sm

	xapp.Logger.Info("SendRicControlRequest Enter for RanName = %s", aRicHoControlMsg.RicControlGrpcReqPtr.RanName)

	//if aRicHoControlMsg.RicControlGrpcReqPtr == nil {
	//	return err
	//}

	var lRicControlStyleType int64 = aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlStyle
	var lRicControlActionID int64 = aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.ControlActionId
	//lUEID := aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID

	lUeIdBuf := []byte(aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)

	xapp.Logger.Debug("UEID:%s, lUeIdBuf: %v", aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID, lUeIdBuf)

	var lRicControlHeader []byte = make([]byte, 256) //Check the Size
	lRicControlHeaderEncoded, err := e2sm.SetRicControlHeader(lRicControlHeader, lUeIdBuf, lRicControlStyleType, lRicControlActionID)
	if err != nil {
		xapp.Logger.Error("SetRicControlHeader Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		log.Printf("SetRicControlHeader Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		return // err
	} else {
		xapp.Logger.Info("SetRicControlHeader is success: %x", lRicControlHeaderEncoded)
		fmt.Fprintf(os.Stderr, "Encoded RIC Control Header PDU:\n")
		for i := 0; i < len(lRicControlHeaderEncoded); i++ {
			fmt.Fprintf(os.Stderr, " %02x", lRicControlHeaderEncoded[i])
		}
		fmt.Fprintf(os.Stderr, "\n")
	}

	var lTargetPrimaryCell int64 = RIC_CONTROL_TARGET_PRIMARY_CELL
	var lTargetCell int64 = RIC_CONTROL_TARGET_CELL
	var lNrCGIOrECGI int64 = RIC_CONTROL_CGI_TYPE

  var aRequestSN = 1

	lNrOrEUtraCellType := aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.RICControlCellTypeVal
	lTargetCellVal := aRicHoControlMsg.RicControlGrpcReqPtr.RICControlMessageData.TargetCellID
	lTargetCellValBuf := []byte(lTargetCellVal)

	var lRicControlMessage []byte = make([]byte, 1024) 
	lRicControlMessageEncoded, err := e2sm.SetRicControlMessage(lRicControlMessage, lTargetPrimaryCell, lTargetCell, lNrCGIOrECGI, int64(lNrOrEUtraCellType), lTargetCellValBuf)
	if err != nil {
		xapp.Logger.Error("SetRicControlMessage Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		log.Printf("SetRicControlMessage Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		return // err
	} else {
		xapp.Logger.Debug("SetRicControlMessage is success: %x", lRicControlMessageEncoded)
		fmt.Fprintf(os.Stderr, "Encoded RIC Control Message PDU:\n")
		for i := 0; i < len(lRicControlMessageEncoded); i++ {
			fmt.Fprintf(os.Stderr, " %02x", lRicControlMessageEncoded[i])
		}
		fmt.Fprintf(os.Stderr, "\n")
	}

	lParams := &xapp.RMRParams{}
	lParams.Mtype = 12040 //RIC_CONTROL_REQ
	lParams.SubId = -1

	var lRequestorId uint16 = uint16(aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RICRequestorID)
	var lFuncId uint16 = uint16(aRicHoControlMsg.RicControlGrpcReqPtr.RICE2APHeaderData.RanFuncId)

	lParams.Payload = make([]byte, 2048)
	lParams.Payload, err = e2ap.SetRicControlRequestPayload(lParams.Payload, lRequestorId, uint16(aRequestSN), lFuncId, lRicControlHeaderEncoded, lRicControlMessageEncoded)
	if err != nil {
		xapp.Logger.Error("SetRicControlRequestPayload Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		log.Printf("SetRicControlRequestPayload Failed: %v, UEID:%s", err, aRicHoControlMsg.RicControlGrpcReqPtr.RICControlHeaderData.UEID)
		return // err
	} else {
		xapp.Logger.Debug("Encoding RicControlRequestPayload is success. UEID: %s, Payload: %x", lUEID, lParams.Payload)
		fmt.Fprintf(os.Stderr, "Encoded RIC Control Req PDU:\n")
		for i := 0; i < len(lParams.Payload); i++ {
			fmt.Fprintf(os.Stderr, " %02x", lParams.Payload[i])
		}
		fmt.Fprintf(os.Stderr, "\n")
	}

	valEnbId := aRicHoControlMsg.RicControlGrpcReqPtr.E2NodeID
	valRanName := aRicHoControlMsg.RicControlGrpcReqPtr.RanName
	valPlmnId := aRicHoControlMsg.RicControlGrpcReqPtr.PlmnID
	lParams.Meid = &xapp.RMRMeid{PlmnID: valPlmnId, EnbID: valEnbId, RanName: valRanName}

	xapp.Logger.Debug("The RIC Control RMR message to be sent is with MsgType:%d  SubId=%d, lParams.Meid: %v, UEID: %s", lParams.Mtype, lParams.SubId, lParams.Meid, lUEID)

	xapp.Logger.Debug("Sending RIC Control message to RanName: %s, UEID: %s  ", aRicHoControlMsg.RicControlGrpcReqPtr.RanName, lUEID)

	if !xapp.Rmr.Send(lParams, false) {
		err = errors.New("rmr.Send() failed")
		xapp.Logger.Error("Failed to rmrSend to %v", err)
		log.Printf("Failed to rmrSend to %v", err)
		xapp.Logger.Error("Failed to send RIC_CONTROL_REQ: %v", err)
		log.Printf("Failed to send RIC_CONTROL_REQ: %v", err)
	  return
	}

	xapp.Logger.Info("Sending RIC Control message to RanName: %s, UEID: %s  Success", aRicHoControlMsg.RicControlGrpcReqPtr.RanName, lUEID)

//	aRicHoControlMsg.setEventRicControlCreateExpiredTimer(aRequestSN) //TODO check if this is required as we are not expecting Control ACK

	return // nil

}

var cnt = 0

func (e *HWApp) handleRICIndication(ranName string, r *xapp.RMRParams) {
	// update metrics for indication message
	e.stats["RICIndicationRx"].Inc()


  xapp.Logger.Info("Indication message arrived")

  cnt++
  if cnt % 10 == 0 {
    //Send RC...
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
	  xapp.Logger.Error("Sending RC")
    send_rc()
  }

}

func (e *HWApp) Consume(msg *xapp.RMRParams) (err error) {
	id := xapp.Rmr.GetRicMessageName(msg.Mtype)

	xapp.Logger.Info("Message received: name=%s meid=%s subId=%d txid=%s len=%d", id, msg.Meid.RanName, msg.SubId, msg.Xid, msg.PayloadLen)

	switch id {
	// policy request handler
//	case "A1_POLICY_REQUEST":
//		xapp.Logger.Info("Recived policy instance list")

	// health check request
	case "RIC_HEALTH_CHECK_REQ":
		xapp.Logger.Info("Received health check request")

	// RIC INDICATION message
	case "RIC_INDICATION":
		xapp.Logger.Info("Received RIC Indication message")
		e.handleRICIndication(msg.Meid.RanName, msg)

	default:
		xapp.Logger.Info("Unknown message type '%d', discarding", msg.Mtype)
	}

	defer func() {
		xapp.Rmr.Free(msg.Mbuf)
		msg.Mbuf = nil
	}()
	return
}





func main() {
 
	//Set Logger Configuration
	xappname := viper.GetString("name")
	xappversion := viper.GetString("version")
	
  // xapp object is initialized at pkg/xapp (I think). Only telecom engineers can have such broken ideas
  xapp.Logger.SetMdc("Name", xappname)
	xapp.Logger.SetMdc("Version", xappversion)


  fmt.Println("Hello, World!")

	// Defind metrics counter that the xapp provides
	metrics := []xapp.CounterOpts{
		{
			Name: "RICIndicationRx",
			Help: "Total number of RIC Indication message received",
		},
	}

	hw := HWApp{
		stats: xapp.Metric.RegisterCounterGroup(metrics, "hw_go"), // register counter
	}


	xapp.SetReadyCB(hw.xAppStartCB, true)
	
  // start xapp
	xapp.RunWithParams(&hw, true)


  //  var err = 5
  //  xapp.Logger.Error("Failed to generate EventTriggerDefinition, err = %v", err)


}

