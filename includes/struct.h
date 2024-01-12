typedef struct {
    int id;
    char name[30];
    char move_forward;
    char move_backward;
    char move_left;
    char move_right;
    int speed_step;
    char change_step_button;
    int id_profile;

} Configuration;

typedef struct{
    int id;
    char username[20];
} Profile;

typedef struct{
    int id;
    char name[40];
    int duration;
    int distance;
    double average_speed;
    char time_start[30];
    int id_configuration;
    int id_profile;
} Session;

typedef struct{
    GtkBuilder *builder;
    gint id_session;
} GetSession;

typedef struct {
    Configuration configuration;
    int clientSocket;
    int id_profile;
} ControlData;

typedef struct {
    int forceFeedback;
    char * carName;
    int defaultUserId;
    int defaultConfigId;
    int maxSessionTime;
} Setting;