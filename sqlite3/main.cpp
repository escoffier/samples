#include <sqlite3.h>
#include <pthread.h>
#include <iostream>

#define NUM_THREADS 10

sqlite3* db = 0;
void* worker(void* arg)
{
    std::cout<<"start thread: "<<(int)arg<<std::endl; 
    //sqlite3* db = 0;
    //sqlite3_open("npsipgateway.db", &db);
    const char* tail = 0;
    sqlite3_stmt* pVM = 0;
    sqlite3_prepare_v2(db, "select *from information", -1, &pVM, &tail);
    
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
        std::cout<<"thread: "<<(int)arg<<" have rows"<<std::endl;    
        //return 0;
    }
	std::cout<<mnCols<<std::endl;
    sqlite3_finalize(pVM);
    pthread_exit(NULL);
}

void insert(const char* name, const char *id)
{
	sqlite3_stmt* pVM = 0;
	const char *tail;
	int ret = 0;
    const char *error = 0;

    ret = sqlite3_prepare_v2(db, "insert into information values(?, ?);", -1, &pVM, &tail);
	if(SQLITE_OK == ret)
	{
		sqlite3_bind_text(pVM, 1, name, -1 , SQLITE_TRANSIENT);
		sqlite3_bind_text(pVM, 2, id, -1, SQLITE_TRANSIENT);
		ret = sqlite3_step(pVM);
		if(SQLITE_DONE == ret)
		{
			//sqlite3_changes(pVM);
			sqlite3_reset(pVM);
		}
		else
		{
			ret = sqlite3_reset(pVM);
            error = sqlite3_errmsg(db);
			std::cout<<"step error: "<<error<<std::endl;
		}
	}
}

void * insertwork(void *arg)
{
	char name[20] = {0};
	char id[20] = {0};

	sprintf(name, "name-%04d", (int)arg);
	sprintf(id, "1000%04d", (int)arg);
	insert(name, id);
}

void bind(sqlite3_stmt* pVM, int feild, const char* value)
{
	int ret = 0;

	ret = sqlite3_bind_text(pVM, feild, value, -1, SQLITE_TRANSIENT);
	if(SQLITE_OK != ret)
	{
		std::cout<<"bind fail: "<<ret<<std::endl;
		return;
	}
}

int main(int argc, char **argv )
{

    sqlite3_open("npsipgateway.db", &db);
	//char *error;
	//int ret = 0;
	//ret = sqlite3_exec(db, "create table information(name char(20), id char(20));", 0, 0, &error);
	//if(0 == ret)
	//{
	//	sqlite3_changes(db);
	//}
	//else
	//{
	//	std::cout<<"create tabke failed, errror is: "<<error<<std::endl;
	//	return 0;
	//}
	insert("robbie", "122");
    pthread_t threads[2* NUM_THREADS];
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int i = 0;
    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], &attr, worker, (void*)i);
    }

    for(i = NUM_THREADS; i <(2 * NUM_THREADS); ++i)
    {
        pthread_create(&threads[i], &attr, insertwork, (void*)i);
    }   
    //pthread_attr_destroy(&attr);
    for(i = 0; i < 2*NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    sqlite3_close(db);
    return 0;
}
