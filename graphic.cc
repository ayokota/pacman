#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <curses.h>
#include <vector>
#include <fstream>
using namespace std;
char choice = 'd';
vector< vector < vector < char> > > pacman;
struct termios old_tio, new_tio;







vector< vector < char> > get_icon ( string filename, vector< vector < char> > icon_name)
{
    icon_name.clear();
    char* fn = &filename[0];    
    ifstream readin;
    readin.open(fn);
    vector<char> honrizontal;
    char c;
    while(readin.good())
     {
         c = readin.get();

         if ( c != '\n')
             honrizontal.push_back(c);
         else
         {
             icon_name.push_back(honrizontal);
             honrizontal.clear();
         } 
     }
     readin.close();
     return icon_name;
}
/*
void move_right (vector< vector<char> > icon_name, int & x, int & y)
{
    x = x +3;
    for(int i = 0; i< y ; i ++)
    {
       cout << endl; 
    }
    for( int i= 0; i < x; i++)
       cout << " ";
    
    for(int j = 0; j < icon_name.size(); j++)
    {
       for ( int k = 0; k < icon_name[j].size(); k++)
       {
            cout << icon_name[j][k];
       }
       cout << endl;
       for ( int i_i = 0; i_i < x ; i_i++)
            cout << " ";
    }
}
*/
void print (vector< vector<char> > icon_name, int & x, int &  y)
{
    for(int i = 0; i< y ; i ++)
    {
       cout << endl; 
    }
    for( int i= 0; i < x; i++)
       cout << " ";
    
    for(int j = 0; j < icon_name.size(); j++)
    {
        for ( int k = 0; k < icon_name[j].size(); k++)
        {
              cout << icon_name[j][k];
        }
        cout << endl;
        for ( int i_i = 0; i_i < x ; i_i++)
            cout << " ";
    } 
}
     
void multiplex ( vector < vector < vector < char> > > icon_name, int & x, int & y, char choice, int & clock )
{
    int i = 0;
    if(choice == 'd')
    {    
        x = x+2;
        i = 0;
        if(clock%2==1)
            i = 4;
    }
    else if(choice == 'a')
    {
        i = 1;
        x = x-2;
        if(clock%2==1)
            i=4;
    }
    else if(choice == 'w')
    {
        i = 3;
        y = y-2;
        if(clock%2==1)
            i=5;
    }
    else if(choice == 's')
    {
        i = 2;
        y = y + 2;
        if(clock%2==1)
            i=5;
    }
    print(icon_name[i], x, y);
}

void *thread_work (void *threadid)
{
    long tid;
    tid = (long) threadid;
    int x = 10;
    int y = 10;
    int clock = 0;
    if(tid == 0)
    {
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
        while (choice != 'q')
        {
            system("clear");
            if(choice != 'q')
            {
                multiplex(pacman, x, y , choice, clock);  
            }
            
            clock++;
            sleep(1);
        }
    }
    pthread_exit(NULL);


}

int main()
{
    
    vector < vector < char > > pacman_icons;
    pacman_icons = get_icon("right.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    pacman_icons = get_icon("left.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    pacman_icons = get_icon("up.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    pacman_icons = get_icon("down.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    pacman_icons = get_icon("hon_closed.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    pacman_icons = get_icon("ver_closed.txt", pacman_icons);
    pacman.push_back(pacman_icons);
    double w = 1.0;
    system("clear");
    int x;
    int y;
    int clock = 0;
    int previous_choice = '\0';


    blah = " ";int thread_no = 2;
    pthread_t threads[thread_no];
    int rc;
    long t;
    for(t = 0; t < thread_no; t++)
    {
        rc = pthread_create (&threads[t], NULL, thread_work, (void*) t);
    }
/*
    while ( choice !='q')
    {
        choice = getchar();
        system ("clear"); 
        multiplex(pacman, x, y , choice, clock);       
        clock++;
    }
*/
    pthread_exit(NULL);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    cout << endl;
    
    return 0;
}
