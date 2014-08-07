#include <sqlite3.h>
#include <pthread.h>
#include <iostream>

#define NUM_THREADS 20

sqlite3* db = 0;
void* worker(void* arg)
{
    std::cout<<"start thread: "<<(int)arg<<std::endl; 
    //sqlite3* db = 0;
    //sqlite3_open("npsipgateway.db", &db);
    const char* tail = 0;
    sqlite3_stmt* pVM = 0;
    sqlite3_prepare_v2(db, "select *from catalog", -1, &pVM, &tail);
    
    int nRet = sqlite3_step(pVM);
    
    int mnCols = sqlite3_column_count(pVM);
    if (nRet == SQLITE_DONE)
    {
        // no rows
        std::cout<<"no row"<<std::endl;
        return 0;
    }
    else if (nRet == SQLITE_ROW)
    {
        // at least 1 row
    std::cout<<"thread: "<<(int)arg<<"have rows"<<std::endl;    
        return 0;
    }
    sqlite3_finalize(pVM);
    pthread_exit(NULL);
}


int main(int argc, char **argv )
{

    sqlite3_open("npsipgateway.db", &db);
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int i = 0;
    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], &attr, worker, (void*)i);
    }
    
    pthread_attr_destroy(&attr);
    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_exit(NULL);
    sqlite3_close(db);
    return 0;
}
