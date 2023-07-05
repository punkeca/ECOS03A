// Developers: Patricia B Ribeiro, Ronys W R Santana, Vivian L Fragoso
// Subject: Embedded Operating Systems
// Course: Computer Engineering
// Department: Institute of Sytems Engineering and Informational Technology
// University: Universidade Federal de Itajubá
// Location: Itajubpa - MG - Brazil
#include <iostream>
#include <cstring>
#include <vector>
#define P_BUFFERSIZE 11
#define MAX_PROC 11

typedef void (*func_ptr)(void);

std::vector<char*>  itens{ //to name processes
        "The Witcher 3",
        "opera",
        "eclipse",
        "Visual Studio Code",
        "firefox",
        "steam",
        "spotify",
        "discord",
        "photoshop",
        "illustrator",
        "skype",
        "spotify",
        "vlc",
        "itunes",
        "outlook",
        "discord",
        "steam",
        "visualstudio",
        "slack",
        "zoom",
        "gimp",
        "winrar",
        "dropbox",
        "utorrent",
        "github",
        "putty",
        "teamviewer",
        "androidstudio",
        "sourcetree",
};

typedef struct {
    char* processName;
    int period;
    int start;
    int priority;
    func_ptr func;
} process;

FILE *in, *out;

// priority based 
process priorityBuffer[P_BUFFERSIZE];
int start=0;
int end=0;

int addPriorityProc(char* name, int period, int priority, func_ptr someFunc)
{
    if((end+1)%P_BUFFERSIZE!=start) //check if buffer is full
    {
        priorityBuffer[end].processName = name;
        priorityBuffer[end].period = period;
        priorityBuffer[end].start = period;
        priorityBuffer[end].priority = priority;
        priorityBuffer[end].func = someFunc;
        
        end = (end+1)%P_BUFFERSIZE;
        return 0;
    }
    return -1;
}

int removePriorityProc()
{
    if(end != start)//check if buffer is empty
    { 
        start = (start+1)%P_BUFFERSIZE;
        return 0;
    }
    return -1;
}

void priorityPool()
{
    int proxy = start;
    if(end!=start){//not empty
     fprintf(out,"----------------------------------- Process Pool --------------------------------------\n");
        while(proxy != end){
            fprintf(out,"-> Process Name: %s - Period: %d - Priority: %d \n ", priorityBuffer[proxy].processName, priorityBuffer[proxy].period, priorityBuffer[proxy].priority);
            
            proxy = (proxy+1)%P_BUFFERSIZE;
        }
        fprintf(out,"---------------------------------------------------------------------------------------\n\n");
        return;
    }
    fprintf(out,"Ops! Looks like your buffer is empty...\n");
    return;
}

int getMaxPriorityProcess(){
    int proxy = start;
        int next = start;
        while(proxy!=end){
            if(priorityBuffer[proxy].priority >= priorityBuffer[next].priority){
                next = proxy;
            }
            proxy = (proxy+1)%P_BUFFERSIZE;
        }

        process temp = priorityBuffer[start];
        priorityBuffer[start] = priorityBuffer[next];
        priorityBuffer[next] = temp;
        return 0;
}


int priorityScheduler(){
    if(end!=start)
    {
       
        getMaxPriorityProcess();
    }
        return 0;
    
    fprintf(out,"Ops! Looks like your buffer is empty...\n");
    return -1;
}

int priorityExec()
{
    if(start!=end){
       priorityBuffer[start].func();
       priorityBuffer[start].start--;
       return 0;
    }
    return -1;
}

void priorityHello(){
    fprintf(out,"Hello! It's me... %s! \n \n", priorityBuffer[start].processName);
    return;
}

int priorityKernelInit(){
    int date,dur, pri, i=0;
    in = fopen("input.txt", "r");

     while(fscanf(in, "%d %d %d", &date, &dur, &pri) != EOF){
        addPriorityProc(itens[i], dur, pri, priorityHello);  
        i++;
    }

    fclose(in);
    
    return 0;
}

int priorityKernelLoop(){
    while(start!=end){
        priorityPool();
        priorityScheduler();
        priorityExec();
        if(priorityBuffer[start].period > 0){
           addPriorityProc(priorityBuffer[start].processName, priorityBuffer[start].start, priorityBuffer[start].priority, priorityBuffer[start].func);
        }
        removePriorityProc();
    }
    return 0;
}

//Multiple Qeueus
process buffer[2][P_BUFFERSIZE]; //Buffer with 2 queues and n processes
int start_[2];                   //start of queues
int end_[2];                     //end of queues
int active_queue = 0;            //used to alternate between the queues in the buffer

bool aloc;
bool name; //to update the key in the name
bool key;  //used to alternate queues when allocating processes

int addMultiQueuesProc(char* name, int period, int priority, func_ptr someFunc) {

    if ((end_[key] + 1)% P_BUFFERSIZE != start_[key]) {                // check if buffer is full
        buffer[aloc][end_[aloc]].processName = name;     //allocate the name
        buffer[aloc][end_[aloc]].period = period;        //allocate the period to the variable to be decremented
        buffer[aloc][end_[aloc]].priority = priority;    //allocate the priority
        buffer[aloc][end_[aloc]].func = someFunc;        //choose the function associated with the thread

        end_[aloc] = (end_[aloc] + 1) % P_BUFFERSIZE;    //update the end of queue x
        return 0;
    }
    return -1;                                        //error case
}

int removeMultiQueuesProc() {
    if (start_[key] != end_[key]) {                       // check if buffer is empty
        start_[key] = (start_[key] + 1) % P_BUFFERSIZE;   //update the start of the queue
        return 0;
    }
    return -1;                                            //error case
}

void MultiQueuesPool() {
    fprintf(out,"----------------------------------- Process Pool --------------------------------------\n");
    for (int q = 0; q < 2; q++) {                   //iterate through the 2 queues
        int proxy = start_[q];
        if (end_[q] != start_[q]) {                 // not empty
            while (proxy != end_[q]) {              //until it's not the end of queue x
                fprintf(out,"-> Process Name: %s - Period: %d - Priority: %d \n", buffer[q][proxy].processName, buffer[q][proxy].period, buffer[q][proxy].priority);
                proxy = (proxy + 1) % P_BUFFERSIZE; //update the proxy
            }
        }
    }
    fprintf(out,"---------------------------------------------------------------------------------------\n\n");
}


int getMultiQueuesProcessPriority() {
    fprintf(out,"\n---------------------------------Priority Queue--------------------------------\n");
    int proxy = start_[key];        //proxy to mark the start
    int next = start_[key];         //to mark the next
    while (proxy != end_[key]) {    //while the proxy doesn't reach the end
        //compare the first with the next and choose the higher priority
        if (buffer[key][proxy].priority > buffer[key][next].priority) {
            next = proxy;
        }
        //update the next
        proxy = (proxy + 1) % P_BUFFERSIZE;
    }

    //swap the process with the highest priority with the first process
    process temp = buffer[key][start_[key]];
    buffer[key][start_[key]] = buffer[key][next];
    buffer[key][next] = temp;

    return 0;
}

int getMultiQueuesProcessMinPeriod() {
    fprintf(out,"\n---------------------------------EDF Queue--------------------------------\n");
    int proxy = start_[key];        //proxy to mark the start
    int next = start_[key];         //to mark the next
    while (proxy != end_[key]) {    //while the proxy doesn't reach the end
        //compare the first with the next and choose the smaller period
        if (buffer[key][proxy].period < buffer[key][next].period) {
            next = proxy;
        }
        //update the next
        proxy = (proxy + 1) % P_BUFFERSIZE;
    }

    //swap the process with the smallest period with the first process
    process temp = buffer[key][start_[key]];
    buffer[key][start_[key]] = buffer[key][next];
    buffer[key][next] = temp;

    return 0;
}

int multiQueuesScheduler() {
    if (end_[key] != start_[key] || end_[!key] != start_[!key]) {  //if both queues are not empty
        //invoke the function depending on the key: 0 for priority, 1 for period
        if (key == 1) {
            fprintf(out,"Queue = %d", key);
            getMultiQueuesProcessMinPeriod();
        }
        else {
            fprintf(out,"Queue = %d", key);
            getMultiQueuesProcessPriority();
        }
        return 0;
    }

    fprintf(out,"Oops! Looks like your buffer is empty...\n");
    return -1;  //error case
}

int multiQueuesExec() {
    bool current_queue = key;    //store my current queue
    name = current_queue;        //to know in which queue to call Hello()

    //check if both queues are empty, if they are, end the program *********
    if (start_[current_queue] == end_[current_queue] && start_[!current_queue] == end_[!current_queue]) {
        fprintf(out,"Both buffers are empty. Exiting...\n");
        exit(0);
    }
    else {
        if (buffer[current_queue][start_[current_queue]].period > 5 && current_queue == 0) {
            //if the period is greater than 5, allocate it in queue 1 for slow processes
                aloc = 1;
                addMultiQueuesProc( buffer[current_queue][start_[current_queue]].processName, 
                                    buffer[current_queue][start_[current_queue]].period,
                                    buffer[current_queue][start_[current_queue]].priority,
                                    buffer[current_queue][start_[current_queue]].func);
                removeMultiQueuesProc();
                fprintf(out,"\nreallocated process\n");
        }
        if (start_[current_queue] != end_[current_queue]) {                   //if the queue is not empty
            buffer[current_queue][start_[current_queue]].func();              //execute the function

            //if it's queue 1, decrement normally; if it's queue 2, decrement more
            if (current_queue == 1) {
                buffer[current_queue][start_[current_queue]].period = buffer[current_queue][start_[current_queue]].period - 1;
            }
            else {
                buffer[current_queue][start_[current_queue]].period--;
            }

            //if queue A is empty
        }
        else {
            current_queue = !current_queue;
            name = !current_queue;
            if (start_[current_queue] != end_[current_queue]) {                 //if the queue is not empty
                buffer[current_queue][start_[current_queue]].func();            //execute the function

                //if it's queue 1, decrement normally; if it's queue 2, decrement more
                if (current_queue == 1) {
                    if (buffer[current_queue][start_[current_queue]].period > 1) {
                        buffer[current_queue][start_[current_queue]].period = buffer[current_queue][start_[current_queue]].period - 2;
                    }
                    else {
                        buffer[current_queue][start_[current_queue]].period--;
                    }
                }
                else {
                    buffer[current_queue][start_[current_queue]].period--;
                }
            }

        }
        
    }
        
    return 0;
}

void multiQueuesHello() {
    fprintf(out,"Hello! It's me... %s! \n\n", buffer[name][start_[name]].processName);
    return;
}

int multiQueuesKernelInit() {
    aloc = 0;
    int date, dur, pri, i=0;
    in = fopen("input.txt", "r");

    while(fscanf(in, "%d %d %d", &date, &dur, &pri) != EOF){
        addMultiQueuesProc(itens[i], dur, pri, multiQueuesHello);  
        i++;
    }

    fclose(in);
     
    return 0;
}

int multiQueuesKernelLoop() {
    //if one queue is empty
    while (start_[key] != end_[key] || start_[!key] != end_[!key]) {
        MultiQueuesPool();
        multiQueuesScheduler();
        //calls chosen process
        multiQueuesExec();

        if (buffer[key][start_[key]].period == 0) {   //if period ended
            fprintf(out,"\nEnding process %s!\n\n", buffer[key][start_[key]].processName);
            removeMultiQueuesProc();                                      //removes process
        }
//-----------------------------------------------------------------------------------------------------------------------
        //if both queues are full switching between them is allowed
        if(start_[key] != end_[key] || start_[!key] != end_[!key]){
            key = !key;
            //verifies if process exists in current queue, if not, changes index
            if(start_[key] == end_[key] && start_[!key] != end_[!key]){
                key = !key;
            }else{
                ;
            }
//------------------------------------------------------------------------------------------------------------------------
        }
    }
    return 0;
}

int main() {
    out = fopen("output.txt", "w");

    fprintf(out, "* * * Welcome to the Operating Systems Scheduling Simulator! * * *\n\n");
    fprintf(out, "----> Test your Scheduler:\n");
    fprintf(out, "  1- PriorityScheduler       vs        2- Multi-queues Scheduler\n");
    fprintf(out, "  User choice: ");

    fprintf(out, "\n\n\n");
    fprintf(out, "* * * Running cooperative priority based scheduler: * * *\n\n");
    priorityKernelInit();
    priorityKernelLoop();

    fprintf(out, "\n\n\n");
    fprintf(out, "* * * Running preemptive multiqueues scheduler: * * *\n\n");
    
    multiQueuesKernelInit();
    multiQueuesKernelLoop();

    fprintf(out, "\n* * * * End of Program * * * *\n");

    //fclose(in);
    fclose(out);
    return 0;
}
