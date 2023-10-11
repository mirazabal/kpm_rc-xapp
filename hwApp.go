/*
==================================================================================
  Copyright (c) 2020 Samsung

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

==================================================================================
*/



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

/*
#include <kpmv2/wrapper.h>
#cgo LDFLAGS: -lkpmv2wrapper -lm
#cgo CFLAGS: -I/usr/local/include/kpmv2
*/
import "C"


/*
#include <rcv1/wrapper.h>
#cgo LDFLAGS: -lrcv1wrapper -lm
#cgo CFLAGS: -I/usr/local/include/rcv1
*/
import "C"

import (
	"encoding/json"
	//"encoding/binary"
	"gerrit.o-ran-sc.org/r/ric-plt/alarm-go.git/alarm"
	"gerrit.o-ran-sc.org/r/ric-plt/xapp-frame/pkg/clientmodel"
	"gerrit.o-ran-sc.org/r/ric-plt/xapp-frame/pkg/xapp"
	"log"
	"time"
	"unsafe"
	"errors"
)

type DRB int32

const (
    Create  DRB = 0
    Release DRB = 1
)

func SetRicControlHeader(buffer []byte) (newBuffer []byte, err error) {
        cptr := unsafe.Pointer(&buffer[0])
	size := C.rc_encode_rc_ric_ctl_hdr(cptr, 4096)
        newBuffer = C.GoBytes(cptr,  C.int(size)) // (C.int(size)+7)/8)
        return
}

func SetRicControlMessage(buffer []byte, d DRB) (newBuffer []byte, err error) {
	cptr := unsafe.Pointer(&buffer[0])
	if d == Create {
	  size := C.create_drb_rc_msg(cptr, C.size_t(len(buffer)))
          newBuffer = C.GoBytes(cptr, (C.int(size))) //TOCHECK: if C.int(size) is returning bits not bytes to get correct size of the buffer.
	} else {
	  size := C.release_drb_rc_msg(cptr, C.size_t(len(buffer)))
          newBuffer = C.GoBytes(cptr, (C.int(size))) //TOCHECK: if C.int(size) is returning bits not bytes to get correct size of the buffer.
	}
	xapp.Logger.Info("E2sm SetRicControlMessage is success")
        return
}

func SetRicControlRequestPayload(payload []byte, ricRequestorID uint16, ricRequestSequenceNumber uint16, ranFunctionID uint16,
        ricControlHdr []byte, ricControlMsg []byte) (newPayload []byte, err error) {

        cptr := unsafe.Pointer(&payload[0])
        cptr_ricControlHdr := unsafe.Pointer(&ricControlHdr[0])
        cptr_ricControlMsg := unsafe.Pointer(&ricControlMsg[0])

        size := C.e2ap_encode_ric_control_request_message(cptr, C.size_t(len(payload)), C.long(ricRequestorID), C.long(ricRequestSequenceNumber),
                C.long(ranFunctionID), cptr_ricControlHdr, C.size_t(len(ricControlHdr)), cptr_ricControlMsg, C.size_t(len(ricControlMsg)))
        if size < 0 {
                return make([]byte, 0), errors.New("e2ap wrapper is unable to set Subscription Request Payload due to wrong or invalid payload")
        }
        newPayload = C.GoBytes(cptr, (C.int(size)+7)/8)
        //newPayload = C.GoBytes(cptr, (C.int(size)))
        return
}

type HWApp struct {
	stats map[string]xapp.Counter
}

var (
	A1_POLICY_QUERY      = 20013
	POLICY_QUERY_PAYLOAD = "{\"policy_type_id\":20000}"
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

func (e *HWApp) ConfigChangeHandler(f string) {
	xapp.Logger.Info("Config file changed")
}

func (e *HWApp) getEnbList() ([]*xapp.RNIBNbIdentity, error) {
	enbs, err := xapp.Rnib.GetListEnbIds()

	if err != nil {
		xapp.Logger.Error("err: %s", err)
		return nil, err
	}

	xapp.Logger.Info("List for connected eNBs :")
	for index, enb := range enbs {
		xapp.Logger.Info("%d. enbid: %s", index+1, enb.InventoryName)
	}
	return enbs, nil
}

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

	if enbs, err := e.getEnbList(); err == nil {
		nbs = append(nbs, enbs...)
	}

	if gnbs, err := e.getGnbList(); err == nil {
		nbs = append(nbs, gnbs...)
	}
	return nbs
}

//func read_int64(data []byte) (ret []int64) {
//    buf := bytes.NewBuffer(data)
//    for _, b := range buf{
//    	binary.Read(b, binary.LittleEndian, &ret)
//    } 
//    return
//}



func (e *HWApp) sendSubscription(meid string) {
	xapp.Logger.Info("sending subscription request for meid : %s", meid)
	if meid != "gnb_505_001_00000001" {
		return
	} 

	meid = "gnb_505_001_00000001"


	//ad := make([]byte, 8192)
	//cptr_ad := unsafe.Pointer(&ad[0])	
        //size := C.kpm_action_def(cptr_ad, C.size_t(8192))

	//byteSlice := ad[:size] // binary.BigEndian.Uint32(ad[:size]) // ad[:size] //
	//intSlice := make([]int64, size)   
	//for idx, elm := range byteSlice {
    // index is the index where we are
    // element is the element from someSlice for where we are
    	  //binary.Read(elm, binary.BigEndian, &intSlice[idx])
	//}

	//intSlice := new( []int64 )
	//binary.Write(intSlice, binary.BigEndian,  byteSlice)
        //intSlice := make([]int64, len(byteSlice))

	subscriptionParams := clientmodel.SubscriptionParams{
		ClientEndpoint: &clientEndpoint,
		Meid:           &meid,
		RANFunctionID:  &funId,
		SubscriptionDetails: clientmodel.SubscriptionDetailsList([]*clientmodel.SubscriptionDetail{
			{
				ActionToBeSetupList: clientmodel.ActionsToBeSetup{
					&clientmodel.ActionToBeSetup{
						ActionDefinition: clientmodel.ActionDefinition([]int64{1, 2, 3, 4}),
						ActionID:         &actionId,
						ActionType:       &actionType,
						SubsequentAction: &clientmodel.SubsequentAction{
							SubsequentActionType: &subsequestActioType,
							TimeToWait:           &timeToWait,
						},
					},
				},
				EventTriggers:       clientmodel.EventTriggerDefinition([]int64{0 , 1}),
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
		err := xapp.Alarm.Raise(8086, alarm.SeverityCritical, meid, "subscriptionFailed")
		if err != nil {
			xapp.Logger.Error("Raising alarm failed with error %v", err)
		}

		return
	}
	xapp.Logger.Info("Successfully subcription done (%s), subscription id : %s", meid, *resp.SubscriptionID)
}

func (e *HWApp) xAppStartCB(d interface{}) {
	xapp.Logger.Info("xApp ready call back received")

	// RMR may still not be ready, so be gentle in this mist and sleep	
	time.Sleep(15 * time.Second)

	// get the list of all NBs
	//nbList := e.getnbList()

	// send subscription request to each of the NBs
	//for _, nb := range nbList {
	//	e.sendSubscription(nb.InventoryName)
	//}

	for i := 0; i < 100; i++{
		time.Sleep(5 * time.Second)
		if i % 2 == 0 {
			create_second_bearer()
		} else {
			release_second_bearer()
		}

	}



}

func send_rc(drb DRB){
	var lRicControlHeader []byte = make([]byte, 4096) //Check the Size
	lRicControlHeaderEncoded, err := SetRicControlHeader(lRicControlHeader)
	var lRicControlMessage []byte = make([]byte, 4096)
	lRicControlMessageEncoded, err := SetRicControlMessage(lRicControlMessage, drb)
	
	lParams := &xapp.RMRParams{}
	lParams.Mtype = 12040 //RIC_CONTROL_REQ
	lParams.SubId = -1

	var aRequestSN uint16 = 1
	var lRequestorId uint16 = 2 
	var lFuncId uint16 =  3  

	lParams.Payload = make([]byte, 8192)
	lParams.Payload, err = SetRicControlRequestPayload(lParams.Payload, lRequestorId, aRequestSN, lFuncId, lRicControlHeaderEncoded, lRicControlMessageEncoded)
	valEnbId := "00000000000000000000000000000001" 
	valRanName := "gnb_505_001_00000002" 
	valPlmnId :=  "05F510" 
	lParams.Meid = &xapp.RMRMeid{PlmnID: valPlmnId, EnbID: valEnbId, RanName: valRanName}

	if !xapp.Rmr.Send(lParams, false) {
		err = errors.New("rmr.Send() failed")
		xapp.Logger.Error("Failed to rmrSend to %v", err)
		log.Printf("Failed to rmrSend to %v", err)
		xapp.Logger.Error("Failed to send RIC_CONTROL_REQ: %v", err)
		log.Printf("Failed to send RIC_CONTROL_REQ: %v", err)
		return
	}

	return // nil
}


func create_second_bearer(){
	send_rc(Create)	
}

func release_second_bearer(){
	send_rc(Release)	
}

const one_ms C.int64_t = 1000 
const ten_ms C.int64_t = 10000 
const three_sec int64  = 3000000 

var last_us  int64 = 0 
var num_drbs int64 = 1 

func (e *HWApp) handleRICIndication(ranName string, r *xapp.RMRParams) {

  xapp.Logger.Debug("Received RIC Indication message ")

  cptr := unsafe.Pointer(&r.Payload[0])
  cptr2 := C.print_e2ap_msg(cptr, C.size_t(r.PayloadLen) )
  var sojourn_time C.int64_t = C.kpm_dec_ind_asn(cptr2)

  xapp.Logger.Info("Sojourn time %ld", sojourn_time)

  if(sojourn_time > ten_ms && num_drbs == 1){
      create_second_bearer()
      num_drbs = 2
      last_us = time.Now().UnixNano() / 1e3 
      xapp.Logger.Info("Creating second bearer")
    } else if(sojourn_time < one_ms && num_drbs == 2 && (((time.Now().UnixNano() / 1e3) - last_us) > three_sec) ){
      release_second_bearer()
      num_drbs = 1
      xapp.Logger.Info("Releasing second bearer")
    }
}

func (e *HWApp) Consume(msg *xapp.RMRParams) (err error) {
	id := xapp.Rmr.GetRicMessageName(msg.Mtype)

	xapp.Logger.Info("Message received: name=%s meid=%s subId=%d txid=%s len=%d", id, msg.Meid.RanName, msg.SubId, msg.Xid, msg.PayloadLen)

	switch id {
	// policy request handler
	case "A1_POLICY_REQUEST":
		xapp.Logger.Info("Recived policy instance list")

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

func (e *HWApp) Run() {

	// set MDC
	xapp.Logger.SetMdc("HWApp", "0.0.1")

	// set config change listener
	xapp.AddConfigChangeListener(e.ConfigChangeHandler)

	// register callback after xapp ready
	xapp.SetReadyCB(e.xAppStartCB, true)

	// reading configuration from config file
	waitForSdl := xapp.Config.GetBool("db.waitForSdl")

	// start xapp
	xapp.RunWithParams(e, waitForSdl)
}

func main() {
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
	hw.Run()
}
