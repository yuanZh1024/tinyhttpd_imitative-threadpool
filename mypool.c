typedef struct work{
	void *(*Callback)(void* arg);
	void *arg;
	struct work *next;
}work_t;//work queue
typedef struct manager{
	pthread_mutex_t mutex;
	pthread_cond_t queue_ready;
	work_t *work_list;
	int max_thread_num;
	int cur_queue_size;
	int close;
	pthread_t *tid;//array of pthreadID
}manager_t;//thread pool manager

static manager_t *pool =NULL;

int init_pool(int max_thread_num){
	pool = (thread_pool*)malloc(sizeof(manager_t));
	pthread_mutex_init(&(pool->mutex),NULL);
	pthread_cond_init(&(pool->queue_ready),NULL);
	pool->work_list = NULL;
	pool->max_thread_num = max_thread_num;
    pool->cur_queue_size = 0;
	pool->close = 0;
    pool->tid = (pthread_t)malloc(sizeof(pthread_t)*maxnum);
    int i=0;
    for(int i=0;i<maxnum;++i){
    	if(pthread_create(&(pool->tid[i]),NULL,thread_execute,NULL)!=0){
    		perror("pthread_create!");
    	}
    }
    return 0;
}

int add_work_and_allocthread(void *(*Callback)(void* arg),void *arg){
	work_t work;
	work.Callback = Callback;
	work.arg = arg;
	work.next = NULL;
	pthread_mutex_lock();//?
	work_t *Tmp = pool->work_list;//head
	//尾插
	if(!Tmp){
		while(!Tmp && !Tmp->next){
			Tmp=Tmp->next;
		}
		Tmp->next = &work;
	}
	else{
		pool->work_list = &work;
	}
	if(pool->work_list==NULL) return -1;
	pool->cur_queue_size++;
	pthread_cond_signal(&(pool->queue_ready));
	return 0;
}
//释放malloc得到的内存
int pool_destroy(){
	if(pool->close==1) return -1;
	pool->close = 1;
	pthread_cond_broadcast(&(pool->queue_ready));
	int i;
	for(i=0;i<pool->max_thread_num;++i){
		pthread_join(pool->tid[i],NULL);
	}
	free(pool->tid);



	free(pool);
	pool = NULL;
	return 0;
}