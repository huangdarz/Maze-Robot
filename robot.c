#include "robot.h"

void setup_robot(struct Robot *robot){
//    robot->x = OVERALL_WINDOW_WIDTH/2-50;
//    robot->y = OVERALL_WINDOW_HEIGHT-50;
//    robot->true_x = OVERALL_WINDOW_WIDTH/2-50;
//    robot->true_y = OVERALL_WINDOW_HEIGHT-50;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;

    // Basic Maze 1
//    robot->x = 270;
//    robot->y = 460;
//    robot->true_x = 270;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;

    // Basic Maze 2
//    robot->x = 620;
//    robot->y = 380;
//    robot->true_x = 620;
//    robot->true_y = 380;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 270;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;

    // Basic Maze 3
//    robot->x = 640-10-270;
//    robot->y = 460;
//    robot->true_x = 640-10-270;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;

    // Basic Maze 4
    robot->x = 0;
    robot->y = 380;
    robot->true_x = 0;
    robot->true_y = 380;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 90;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorCustom(int x, int object1width, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,object1width,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorBottomRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+((ROBOT_WIDTH/2-2)+SENSOR_VISION-sensorSensitivityLength*i)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+((ROBOT_WIDTH/2-2)+SENSOR_VISION-sensorSensitivityLength*i)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));

        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensorCustom(xTL,sensorSensitivityLength, yTL, 2, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        //printf("%d\n", robot->angle);

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL,yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    */

    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    int xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    int yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);


    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    //Bottom Right Point right Sensor
    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int i;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2+SENSOR_VISION-sensor_sensitivity*i)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2+SENSOR_VISION-sensor_sensitivity*i)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, sensor_sensitivity, 2};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
}


void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > MAX_ROBOT_SPEED)
                robot->currentSpeed = MAX_ROBOT_SPEED;
            break;
        case DOWN :
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                robot->currentSpeed = -MAX_ROBOT_SPEED;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            break;
    }
    if (!robot->auto_mode) {
        robot->direction = 0;
    }
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}


int is_start = 1;

// 0 is stationary, 1 is moving, 2 is turning
int state = 0;
int target_angle = -1;
int at_target = 0;
int is_right = 0;
int has_turned_right = 1;

int max_speed = 4;

int instant_speed = 21;
int is_instant = 0;
// Every pixel is 5mm

void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int bottom_right_sensor) {

    if (!is_instant) {
        if (robot->direction != UP && robot->direction != DOWN) {
            robot->direction = 0;
        } else {
            if (robot->currentSpeed >= max_speed) {
                robot->direction = 0;
            }
        }
    } else {
        robot->direction = 0;
    }


    if (robot->angle == target_angle && !at_target) {
        is_start = 0;
        state = 0;
        at_target = 1;
        is_right = 0;
    }

    if (state == 2) {
        if (target_angle == 0 && !at_target) {
            if (robot->angle > 180) {
                robot->direction = RIGHT;
            } else {
                robot->direction = LEFT;
            }
        } else {
            if (robot->angle < target_angle && !at_target) {
                robot->direction = RIGHT;
            }

            if (robot->angle > target_angle && !at_target) {
                robot->direction = LEFT;
            }
        }
    }

    if (is_start) {
        if ((front_left_sensor == 0) && (bottom_right_sensor == 0)) {

            // target_angle = 90;
            target_angle = robot->angle + 90;
            if (target_angle >= 360) {
                target_angle -= 360;
            }
            state = 2;
            at_target = 0;
            is_start = 0;
        }
    } else {
        if (state == 0) {
            if (front_left_sensor == 0 && bottom_right_sensor == 0) {
                if (is_right && !has_turned_right) {
                    target_angle = robot->angle + 90;
                    if (target_angle >= 360) {
                        target_angle -= 360;
                    }
                    state = 2;
                    at_target = 0;
                    has_turned_right = 1;
                } else {
                    if (is_instant) {
                        robot->currentSpeed = instant_speed;
                    } else {
                        robot->direction = UP;
                    }
                    state = 1;
                }
            } else if (front_left_sensor > 0) {
                robot->direction = LEFT;
                target_angle = robot->angle - 90;
                if (target_angle < 0) {
                    target_angle += 360;
                }
                state = 2;
                at_target = 0;
            } else if (bottom_right_sensor > 0) {
                if (is_instant) {
                    robot->currentSpeed = instant_speed;
                } else {
                    robot->direction = UP;
                }
                robot->direction = UP;
                state = 1;
            }
        }
        if (state == 1) {
            if (bottom_right_sensor <= 0 && !has_turned_right) {
                if (is_instant) {
                    robot->currentSpeed = 0;
                } else {
                    robot->direction = DOWN;
                }
                state = 0;
                is_right = 1;
            } else if (front_left_sensor > 0) {
                if (is_instant) {
                    robot->currentSpeed = 0;
                } else {
                    robot->direction = DOWN;
                }
                state = 0;
            } else if (bottom_right_sensor > 0) {
                has_turned_right = 0;
            }
        }
    }
}
