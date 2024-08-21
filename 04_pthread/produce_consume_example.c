#include <cstage.h>
typedef struct node_s{
    int value;
    struct node_s *p_next;
}node_t;

typedef struct queue_s{
    node_t *pHead;
    node_t *pEnd;
    int size;
}queue_t;

void offer(queue_t *pQueue, int value){
    node_t *pNewNode = (node_t *)calloc(1, sizeof(node_t));
    pNewNode->value = value;

    if(pQueue->size == 0){
        pQueue->pHead = pNewNode;
        pQueue->pEnd = pNewNode;
    }else{
        pQueue->pEnd->p_next = pNewNode;
        pQueue->pEnd = pNewNode;
    }
    pQueue->size++;
}
int poll(queue_t *pQueue){
    if(pQueue == NULL || pQueue->size == 0){
        return 0;
    }
    node_t *pOldHead = pQueue->pHead;
    pQueue->pHead = pOldHead->p_next;
    pQueue->size--;
    if(pQueue->size == 0){
        pQueue->pEnd = NULL;
    }
    int pollValue = pOldHead->value;
    free(pOldHead);
    return pollValue;
}
void toString(queue_t *pQueue){
    node_t *mid = pQueue->pHead;
    if(mid == NULL){
        return;
    }
    printf("==  ");
    while(mid){
        printf("%3d  ", mid->value);
        mid = mid->p_next;
    }
    printf("== \n");
}
typedef struct share_value_s{
    queue_t queue;
    pthread_mutex_t mLock;
    pthread_cond_t cond;
}share_value_t;
void *produce(void *arg){
    share_value_t *pShareValue = (share_value_t *)arg;
    while(1){
        pthread_mutex_lock(&pShareValue->mLock);
        while(pShareValue->queue.size >= 20){
            pthread_cond_wait(&pShareValue->cond, &pShareValue->mLock);
        }
        int value = rand()%1000;
        offer(&pShareValue->queue, value);
        printf("add: %d \n", value);
        toString(&pShareValue->queue);

        pthread_cond_broadcast(&pShareValue->cond);

        pthread_mutex_unlock(&pShareValue->mLock);
        sleep(3);
    }

}
void *consume(void *arg){
    share_value_t *pShareValue = (share_value_t *)arg;
    sleep(5);
    while(1){
        pthread_mutex_lock(&pShareValue->mLock);
        while(pShareValue->queue.size <= 0 ){
            pthread_cond_wait(&pShareValue->cond, &pShareValue->mLock);
        }
        int value = poll(&pShareValue->queue);
        printf("take out: %d \n", value);
        toString(&pShareValue->queue);

        pthread_cond_broadcast(&pShareValue->cond);

        pthread_mutex_unlock(&pShareValue->mLock);
        sleep(1);       
    }
}
int main(){
    share_value_t shareValue;

    memset(&shareValue.queue, 0 , sizeof(queue_t));
    for(int i=0; i<10; i++){
        int data = rand()%1000;
        offer(&shareValue.queue, data);
    }
    toString(&shareValue.queue);
    pthread_mutex_init(&shareValue.mLock, NULL);
    pthread_cond_init(&shareValue.cond, NULL);

    pthread_t  tid1, tid2, tid3, tid4, tid5;
    pthread_create(&tid1, NULL, produce, &shareValue);
    pthread_create(&tid2, NULL, produce, &shareValue);
    pthread_create(&tid3, NULL, consume, &shareValue);
    pthread_create(&tid4, NULL, consume, &shareValue);
    pthread_create(&tid5, NULL, consume, &shareValue);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

    pthread_mutex_destroy(&shareValue.mLock);
    pthread_cond_destroy(&shareValue.cond);
}
