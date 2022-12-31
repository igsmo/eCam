// Paths to the API on the web server
#define MANAGER_PREFIX_PATH             "/api/manager/robot/"
#define GET_NEXT_TASK_SUFFIX_PATH       "/nexttask"
#define UPDATE_STATUS_SUFFIX_PATH       "/updatestatus"

// #define GET_ALL_WAYPOINTS_PATH          "/api/waypoints/getall"
// #define GET_WAYPOINT_PATH               "/api/waypoints/get/"

// IP address and port of the server
#define WEB_SERVER "172.20.10.12"
#define WEB_PORT "80"


void api_update_status(int id, int task_id, int qr_id, float qr_distance);

void api_get_nexttask(int id);