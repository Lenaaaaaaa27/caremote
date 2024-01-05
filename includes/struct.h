typedef struct {
    int id;
    char name[30];
    char move_forward;
    char move_backward;
    char move_left;
    char move_right;
    double max_speed_first_step;
    double max_speed_second_step;
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
    double max_speed;
    double average_speed;
    char time_start[30];
    int id_configuration;
    int id_profile;
} Session;


