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
/*
 * Decoding interface for plain text encoding Information Elements in SM-KPM
 */
#ifndef KPM_DECRYPTION_PLAIN_H
#define KPM_DECRYPTION_PLAIN_H

#include <stddef.h>
#include "kpm_data_ie.h"


kpm_event_trigger_t kpm_dec_event_trigger_plain(size_t len, uint8_t const ev_tr[len]);
kpm_action_def_t kpm_dec_action_def_plain(size_t len, uint8_t const * action_def);
#endif