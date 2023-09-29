/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */


#ifndef RAN_CTRL_SERVICE_MODEL_ID_H
#define RAN_CTRL_SERVICE_MODEL_ID_H 


/*
 * Service Model ID needed for the agent as well as for the ric to ensure that they match. 
 */

#include <stdint.h>

static
const uint16_t SM_RC_ID = 3; 

static
const uint16_t SM_RC_REV = 1; 

__attribute__((unused)) static
const char SM_RAN_CTRL_SHORT_NAME[] = "ORAN-E2SM-RC";

//iso(1) identified-organization(3)
//dod(6) internet(1) private(4)
//enterprise(1) 53148 e2(1)
// version1 (1) e2sm(2) e2sm-RC-
// IEs (3)

__attribute__((unused)) static
const char SM_RAN_CTRL_OID[] = "1.3.6.1.4.1.53148.1.1.2.3"; 

__attribute__((unused)) static
const char SM_RAN_CTRL_DESCRIPTION[] = "RAN Control";

#endif

