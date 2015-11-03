#ifndef RING
#define RING


/* Data Structure */
typedef struct Data {
  float frontLeft;        // distance seen on the front left
  float frontRight;       // distance seen on the front right
  float backLeft;         // distance seen on the back left
  float backRight;        // distance seen on the back right
  int trackState;         // Essentially a follow left or right variable
  int turn_angle;         // turn angle that the car should go next
  int fork;               // check variable to debounce fork detection
} Data_t;

/* List structure */
typedef struct List {
  Data_t data;
  struct List *next;
  struct List *prev;
} List_t;

void listCreate(List_t **head);
void listRead(List_t *head);
void ringRead(List_t **head, int ringSize);
void ringPush(List_t **head, int data);
void ringInit(List_t **head, int ringSize);
void getPrevious(List_t **current);

#endif
