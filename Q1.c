#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_MAX_SIZE 200
int int_cnt = 0;
float average;
int min = INT8_MAX;
int max = INT8_MIN;

/* structure for passing data to threads*/
typedef struct{
    int int_arr[INPUT_MAX_SIZE];
}parameters;

void *thread_average(void *arg){
    parameters *data = (parameters *) arg;
    int sum = 0;
    for (int j = 0; j < int_cnt; j++) {
        sum += data->int_arr[j];
    }
    average = sum/int_cnt;
    return NULL;
}
void *thread_min(void *arg){
    parameters *data = (parameters *) arg;

    for (int j = 0; j < int_cnt; j++) {
        if (data->int_arr[j] < min){
            min = data->int_arr[j];
        }
    }
    return NULL;
}
void *thread_max(void *arg){
    parameters *data = (parameters *) arg;

    for (int j = 0; j < int_cnt; j++) {
        if (data->int_arr[j] > max){
            max = data->int_arr[j];
        }
    }
    return NULL;
}

int main(){
    char input[INPUT_MAX_SIZE];
    // memory allocation of data
    parameters *data = (parameters*) malloc(sizeof(parameters));

    printf("Please input all integers separated by blank spaces.\n");
    fgets(input, INPUT_MAX_SIZE, stdin);

    // Extraction of intergers in the input string
    char *token = strtok(input, " ");
    while (token != NULL) {
        data->int_arr[int_cnt++] = atoi(token);
        token = strtok(NULL, " ");
    }
    // End of the extraction

    pthread_t threads[3]; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr); /* get the default attributes */
    void (*thread_func[3]) = {thread_average, thread_min, thread_max};

    /* create the thread */
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], &attr, thread_func[i], &*data);
    }
    /* now wait for the thread to exit */
    for (int i = 0; i < 3; i++){
        pthread_join(threads[i], NULL);
    }

    printf("The average value is %.4f \n", average);
    printf("The minimum value is %d \n", min);
    printf("The maximum value is %d \n", max);

    free(data);
    return 0;
}
