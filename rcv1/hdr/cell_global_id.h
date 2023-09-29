#ifndef CELL_GLOBAL_ID_H
#define CELL_GLOBAL_ID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "nr_cgi.h"
#include "eutra_cgi.h"

typedef enum {
	NR_CGI_RAT_TYPE,
	EUTRA_CGI_RAT_TYPE,

  END_CGI_RAT_TYPE

} rat_type_e; 

// 6.2.2.5  Cell Global ID

typedef struct{
  // CHOICE RAT type
  // Mandatory
  rat_type_e type;

  union{
    nr_cgi_t nr_cgi; // 6.2.3.7
    eutra_cgi_t eutra; // 6.2.3.11
  };

} cell_global_id_t;

bool eq_cell_global_id(cell_global_id_t const * m0, cell_global_id_t const * m1);

cell_global_id_t cp_cell_global_id(const cell_global_id_t * src);

void free_cell_global_id(cell_global_id_t * src);

#ifdef __cplusplus
}
#endif

#endif

