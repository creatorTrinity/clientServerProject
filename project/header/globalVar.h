#ifndef _GLOBAL_VAR_
#define _GLOBAL_VAR_

#define KEY 123
#define NAME_LEN 100
#define MOB_NO_LEN 11
#define MAX_BATCH_SIZE 100
#define MAX_ARR_SIZE 100
#define USER_NAME_LEN 10
#define USER_PASS_LEN 10
#define NO_OF_SKILLS 10
#define SKILLS_LEN 20
#define MAX_CLIENT_CONNECTION 10
#define CONNECTION_TIME_OUT_SECONDS 5
#define WATCHDOG_SLEEP_SECONDS 2
#define PK_SIZE 210

#define SERVER_CONNECTED "OK"
#define SERVER_NOT_CONNECTED "NOK"
#define SERVER_PCK_ERR "PCK_ERR"
#define SERVER_UNKNOWN_ERROR "FAILED"

const char QUERY_OPTION_1_1[] = "1.1";
const char QUERY_OPTION_1_2[] = "1.2";
const char QUERY_OPTION_1_3[] = "1.3";
const char QUERY_OPTION_2_1[] = "2.1";
const char QUERY_OPTION_2_2[] = "2.2";
const char QUERY_OPTION_2_3[] = "2.3";
const char QUERY_OPTION_3_0[] = "3.0";
const char QUERY_OPTION_4_0[] = "4.0";

enum enumMsgType{CLIENT_START=1};
enum enumStructID{EMP_INFO,CLIENT_INFO,QUERY_RESULT,QUERY_STRING,SERVER_ACK,WATCH_DOG};
enum enumServerAck{OK,ERROR,PCK_ERR};

#define _EMPDB_PATH_ "../database/empDB"


#endif //_GLOBAL_VAR_