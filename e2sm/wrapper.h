#ifndef	_WRAPPER_SM_H_
#define	_WRAPPER_SM_H_

//#include "ARP.h"
#include "BOOLEAN.h"
#include "NativeReal.h"
#include "NULL.h"
#include "REAL.h"
#include "TimeStamp.h"
#include "OCTET_STRING.h"
#include "E2SM-RC-ControlHeader-Format1.h"
#include "E2SM-RC-ControlHeader.h"
#include "E2SM-RC-ControlMessage-Format1.h"
#include "E2SM-RC-ControlMessage.h"
#include "E2SM-RC-RANFunctionDefinition.h"
#include "INTEGER.h"
#include "NativeInteger.h"
#include "OPEN_TYPE.h"
#include "PrintableString.h"
#include "RAN-ControlParameter-Item.h"
#include "RANParameter-ELEMENT.h"
#include "RANParameter-ID.h"
#include "RANParameter-Item.h"
#include "RANParameter-LIST.h"
#include "RANParameter-Name.h"
#include "RANParameter-STRUCTURE.h"
#include "RANParameter-Value.h"
#include "RANParameter-ValueType.h"
#include "RANfunction-Name.h"
#include "RIC-ControlAction-ID.h"
#include "RIC-ControlAction-Item.h"
#include "RIC-ControlAction-Name.h"
#include "RIC-ControlStyle-Item.h"
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-Format-Type.h"
#include "RIC-Style-Name.h"
#include "RIC-Style-Type.h"
#include "UE-Identity.h"


ssize_t e2sm_encode_ric_control_header(void *buffer, size_t buf_size, void *ueIDbuf, size_t ueIDbuf_size, long ricControlStyleType, long ricControlActionID);

//extern ssize_t e2sm_encode_ric_control_header(void *buffer, size_t buf_size, long ricControlStyleType, long ricControlActionID);

ssize_t e2sm_encode_ric_control_message(void *buffer, size_t buf_size, long targetPrimaryCell, long targetCell, long nrOrEUtraCell, long nrCGIOrECGI, void *ranParameterValue, size_t  ranParameterValue_size);

#endif /* _WRAPPER_H_ */
