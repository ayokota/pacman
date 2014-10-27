#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <curses.h>


using namespace std;
char choice = 'd';
struct object
{
    vector< vector< vector<char> > > graphic;
    int x;
    int y;
    int clock;
    bool show_switch;
};

int count_coins(vector< vector < char> > map)
{
    int count = 0;
    for(int y = 0; y < map.size() ;  y++)
    {
        for(int x = 0; map[y][x] != '<' && x < map[y].size() ; x++)
        {
            if (map[y][x] == '.')
                count++;
        }
    }
    return count;
}


void insert_pix ( object &  pix, vector<vector<char> > & map, int pix_x, int pix_y, int pix_clock, bool pix_switch)
{
    pix.x = pix_x;
    pix.y = pix_y;
    pix.clock = pix_clock;
    pix.show_switch = pix_switch;
    
    for(int y = 0; y < pix.graphic[0].size(); y++ )
    {
        for(int x = 0; x < pix.graphic[0][y].size(); x++ )
            map[pix_y + y][pix_x+ x] = pix.graphic[0][y][x];
    }
}

void load_graphic(vector< vector<char> >& graphic , string filename)
{
    graphic.clear();
    char* fn = &filename[0];
    ifstream ifs;
    vector< char > getline;
    char single_char= ' ';
    ifs.open(fn);
    while(ifs.good())
    {
        single_char = ifs.get();
        if(single_char != '\n')
            getline.push_back(single_char);
        else
        {
            graphic.push_back(getline);
            getline.clear();
        }
    }
    ifs.close();
}

void print_graphic( vector < vector<char> > graphic)
{
    system("clear");

    for(int y = 0; y < graphic.size(); y++)
    {
        for(int x = 0; x < graphic[y].size(); x++)
            cout << graphic[y][x];
        cout << endl;
    }
}

bool check_bound(object pix, vector< vector< char > > map)
{
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            if(map[y+pix.y][x+pix.x] == '=')
                return false;
        }
    }
    return true;
}


void multiplex(object & pix, vector< vector< char > > & map, char choice)
{  
    if(choice == 'w')
    {
        pix.y--;
        if(check_bound(pix, map)== true)
        {            
            for(int y = 0; y < 3; y++)
            {
                for(int x = 0; x < 3; x++)
                    map[pix.y+y][pix.x+x] = pix.graphic[3][y][x];
            }
            for(int i = 0; i<3; i++)
                map[pix.y+3][pix.x+i] = ' ';
        }
        else
            pix.y++;
    }
 
 
 
    else if(choice == 's')
    {
        pix.y++;
        if(check_bound(pix, map)== true)
        {            
            for(int y = 0; y < 3; y++)
            {
                for(int x = 0; x < 3; x++)
                    map[pix.y+y][pix.x+x] = pix.graphic[2][y][x];
            }
            for(int i = 0; i<3; i++)
                map[pix.y-1][pix.x+i] = ' ';           
        }
        else
        {
            pix.y--;   
        }
    }
 
 
    else if(choice == 'a')
    {
        pix.x--;
        if(check_bound(pix, map)== true)
        {            
            for(int y = 0; y < 3; y++)
            {
                for(int x = 0; x < 3; x++)
                    map[pix.y+y][pix.x+x] = pix.graphic[1][y][x];
            }
            for(int i = 0; i<3; i++)
                map[pix.y+i][pix.x+3] = ' ';
        }
        else
        {
            pix.x++;
        }
    }
 
 
    else if(choice == 'd')
    {
        pix.x++;
        if(check_bound(pix, map)== true)
        {            
            for(int y = 0; y < 3; y++)
            {
                for(int x = 0; x < 3; x++)
                    map[pix.y+y][pix.x+x] = pix.graphic[0][y][x];
            }
            for(int i = 0; i<3; i++)
                map[pix.y+i][pix.x-1] = ' ';
        }
        else
        {
            pix.x--;
        }
    }
   
    
    pix.clock++;
    if(pix.clock %2 == 1)
    {
        for(int y = 0; y< 3; y++)
        {
            for(int x = 0; x < 3; x++)
                map[pix.y+y][pix.x+x] = pix.graphic[4][y][x];
        }
    }
}

void * thread_work( void *threadid)
{
    long tid;
    tid = (long) threadid;

    if(tid == 0)
    {
        struct termios old_tio, new_tio; 
        // get terminal setting for stdin
        tcgetattr(STDIN_FILENO, &old_tio);
        // keep old setting for restore purpose
        new_tio = old_tio;
        // disable canonical ode (buffered i/o) and local echo 
        new_tio.c_lflag &=(~ICANON & ~ECHO);
        // set the new setting immediately
        tcsetattr(STDIN_FILENO,TCSANOW, &new_tio);
       
        while ( choice != 'q' )
             choice = getchar();
  
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }
    else if(tid == 1)
    {
        
        vector< vector <char> > map;
        load_graphic(map, "pacman_map.txt");
        object pacman;
        vector< vector <char> > pacman_load;
        load_graphic(pacman_load, "right.txt");
        pacman.graphic.push_back(pacman_load);
        load_graphic(pacman_load, "left.txt");
        pacman.graphic.push_back(pacman_load);
        load_graphic(pacman_load, "up.txt");
        pacman.graphic.push_back(pacman_load);
        load_graphic(pacman_load, "down.txt");
        pacman.graphic.push_back(pacman_load);
        load_graphic(pacman_load, "closed.txt");
        pacman.graphic.push_back(pacman_load);
    
        insert_pix (pacman, map, 18 , 27, 0, true);
        print_graphic(map);
        time_t start;
        time(&start);
        while(choice != 'q' &&  count_coins(map) > 0)
        {
            cout << choice;
            fflush(stdout);
            system("clear");
            multiplex(pacman, map, choice);            
            print_graphic(map);
            usleep(60000);
        }
        if(count_coins(map) == 0)
        {
            cout << "Level cleared! \n";
            time_t end;
            time(&end);
            int seconds = difftime(end,start); 
            cout << "Time used: " << seconds << " seconds!\n";
            cout << "Type 'q' to exit program. \n";
            
        }
    }
    pthread_exit(NULL);
}


int main()
{
    
    int thread_no = 2;
    pthread_t threads[thread_no];
    int rc;
    long t;
    for( t = 0; t < thread_no ; t++)
        rc = pthread_create(&threads[t], NULL, thread_work, (void *) t);

    pthread_exit(NULL);

    return 0;
}
