#include <assert.h>

#include "kpm_data_ie.h"

/* KPM RIC INDICATION DATA */

void free_kpm_ind_data(kpm_ind_data_t* ind) 
{
  assert(ind != NULL);
  
  free_kpm_ind_hdr(&ind->hdr);
  free_kpm_ind_msg(&ind->msg);
  assert(ind->proc_id == NULL && "Not implemented");
}

kpm_ind_data_t cp_kpm_ind_data(kpm_ind_data_t const* src) 
{
  assert(src != NULL);
  assert(src->proc_id == NULL && "Not implemented");
  kpm_ind_data_t ret = {0};
  
  ret.hdr = cp_kpm_ind_hdr(&src->hdr);
  ret.msg = cp_kpm_ind_msg(&src->msg);

  return ret;
}

bool eq_kpm_ind_data(kpm_ind_data_t const* ind0, kpm_ind_data_t const* ind1)
{
  assert(ind0 != NULL);
  assert(ind1 != NULL);

  if (ind0 == ind1) return true;

  if(ind0 == NULL || ind1 == NULL)
    return false;

  // Indication Header
  if (eq_kpm_ind_hdr(&ind0->hdr, &ind1->hdr) != true)
    return false;

  // Indication Message
  if (eq_kpm_ind_msg(&ind0->msg, &ind1->msg) != true)
    return false;

  assert(ind0->proc_id == NULL && "Not implemented");
  assert(ind1->proc_id == NULL && "Not implemented");

  return true;
}


/* KPM RIC SUBSCRIPTION DATA */

void free_kpm_sub_data(kpm_sub_data_t* src)
{
  assert(src != NULL);

  //kpm_event_trigger_t et; 

  //action definition
  for(size_t i = 0; i < src->sz_ad; ++i){
    free_kpm_action_def(&src->ad[i]);
  }
  free(src->ad);
}

bool eq_kpm_sub_data(kpm_sub_data_t const* m0, kpm_sub_data_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(eq_kpm_event_trigger_def(&m0->ev_trg_def, &m1->ev_trg_def) == false)
    return false;

  // [1-16]
  assert(m0->sz_ad > 0 && m0->sz_ad < 17); 
  assert(m1->sz_ad > 0 && m1->sz_ad < 17); 

  if(m0->sz_ad != m1->sz_ad)
    return false;

  for(size_t i = 0; i < m0->sz_ad; ++i){
    if(eq_kpm_action_def(&m0->ad[i], &m1->ad[i]) == false)
      return false;
  }

  return true;
}


/* E2 SETUP */

void free_kpm_e2_setup(kpm_e2_setup_t * e2_setup)
{
  assert(e2_setup != NULL);

  free_kpm_ran_function_def(&e2_setup->ran_func_def);
}

kpm_e2_setup_t cp_kpm_e2_setup(kpm_e2_setup_t const* src)
{
  assert(src != NULL);

  kpm_e2_setup_t dst = {0};

  dst.ran_func_def = cp_kpm_ran_function_def(&src->ran_func_def);

  return dst;
}

bool eq_kpm_e2_setup(kpm_e2_setup_t const * m0, kpm_e2_setup_t const * m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if (eq_kpm_ran_function_def(&m0->ran_func_def, &m1->ran_func_def) != true)
    return false;


  return true;
}
