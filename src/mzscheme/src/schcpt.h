
enum {
  CPT_ESCAPE,
  CPT_SYMBOL,
  CPT_SYMREF,
  CPT_STRING,
  CPT_CHAR,
  CPT_INT,
  CPT_NULL,
  CPT_TRUE,
  CPT_FALSE,
  CPT_VOID,
  CPT_BOX,
  CPT_PAIR,
  CPT_LIST,
  CPT_VECTOR,
  CPT_STX,
  CPT_GSTX,
  CPT_MARSHALLED,
  CPT_QUOTE,
  CPT_REFERENCE,
  CPT_LOCAL,
  CPT_LOCAL_UNBOX,
  CPT_SVECTOR,
  CPT_APPLICATION,
  CPT_MODULE_INDEX,
  CPT_MODULE_VAR,
  _CPT_COUNT_
};

#define CPT_SMALL_NUMBER_START 25
#define CPT_SMALL_NUMBER_END   40

#define CPT_SMALL_SYMBOL_START 40
#define CPT_SMALL_SYMBOL_END   50

#define CPT_SMALL_MARSHALLED_START 50
#define CPT_SMALL_MARSHALLED_END 75

#define _SMALL_LIST_MAX_ 50

#define CPT_SMALL_PROPER_LIST_START 75
#define CPT_SMALL_PROPER_LIST_END  (CPT_SMALL_PROPER_LIST_START + _SMALL_LIST_MAX_) 

#define CPT_SMALL_LIST_START CPT_SMALL_PROPER_LIST_END
#define CPT_SMALL_LIST_END   (CPT_SMALL_LIST_START + _SMALL_LIST_MAX_)

#define CPT_SMALL_LOCAL_START 175
#define CPT_SMALL_LOCAL_END 190
#define CPT_SMALL_LOCAL_UNBOX_START 190
#define CPT_SMALL_LOCAL_UNBOX_END 205

#define CPT_SMALL_SVECTOR_START 205
#define CPT_SMALL_SVECTOR_END 230

#define CPT_SMALL_APPLICATION_START 230
#define CPT_SMALL_APPLICATION_END 240

#define CPT_BETWEEN(x, s) ((x >= CPT_ ## s ## _START) && (x < CPT_ ## s ## _END))
#define CPT_RANGE(s) (CPT_ ## s ## _END - CPT_ ## s ## _START)

