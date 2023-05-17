#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_MAX_SIZE 200
int int_cnt = 0;
int int_arr[INPUT_MAX_SIZE];
int sorted_arr[INPUT_MAX_SIZE];

// Compare function for qsort() to sort in ascending order
int cmpfunc (const void * a, const void * b) {
   return (*(int*)a - *(int*)b);
}

/* structure for passing data to threads*/
typedef struct{
    int left;
    int right;
}parameters;

void *thread_left_sort(void *arg){
    parameters *data = (parameters *) arg;
    // Sort the array using qsort()
    qsort(int_arr, data->right - data->left, sizeof(int), cmpfunc);
    return NULL;
}
void *thread_right_sort(void *arg){
    parameters *data = (parameters *) arg;
    // Sort the array using qsort()
    qsort(int_arr + data->right, int_cnt - data->right, sizeof(int), cmpfunc);
    return NULL;
}
void *thread_merge(void *arg){
    parameters *data = (parameters *) arg;
    int left_size = data->right - data->left, right_size = int_cnt - data->right;
    int i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size) {
        if (int_arr[i] <= int_arr[j + data->right]) {
            sorted_arr[k] = int_arr[i];
            i++;
        } else {
            sorted_arr[k] = int_arr[j + data->right];
            j++;
        }
        k++;
    }
    while (i < left_size) {
        sorted_arr[k] = int_arr[i];
        i++;
        k++;
    }
    while (j < right_size) {
        sorted_arr[k] = int_arr[j + data->right];
        j++;
        k++;
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
        int_arr[int_cnt++] = atoi(token);
        token = strtok(NULL, " ");
    }
    // End of the extraction
    data->left = 0;
    data->right = int_cnt/2;

    pthread_t threads[3]; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr); /* get the default attributes */
    void (*thread_func[3]) = {thread_left_sort, thread_right_sort, thread_merge};

    /* create the thread */
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], &attr, thread_func[i], &*data);
    }
    /* now wait for the thread to exit */
    for (int i = 0; i < 3; i++){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < int_cnt; i++){
        printf("%d ", sorted_arr[i]);
    }

    free(data);
    return 0;
}
