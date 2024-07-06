#pragma GCC optimize("Ofast,unroll-loops,fast-math")
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define F first
#define S second
#define pii pair<ll, ll>
#define pb push_back
#define migmig ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
#define file_io freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout)
#define RESET   "\033[0m"
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
ofstream out("output.txt");

const ll ROW_SIZE = 50, COLUMN_SIZE = 50, NUM_BALL = 100;
const ll TOTAL_ANTS = 1e3, STEP = 30;
ll row_size, column_size, num_ball, num_hole, remind_ants = TOTAL_ANTS;
ll phromone[ROW_SIZE][COLUMN_SIZE][ROW_SIZE][COLUMN_SIZE];
ll used[ROW_SIZE][COLUMN_SIZE];
ll mark[5][ROW_SIZE][COLUMN_SIZE];
pii par[5][ROW_SIZE][COLUMN_SIZE];
pii ball[NUM_BALL], hole[NUM_BALL];
// 0 --> Not Visited
// 1 --> Visited
// 2 --> Ball
// 3 --> Hole
// 4 --> Beginnig Location
ll cell[ROW_SIZE][COLUMN_SIZE];
pii beginnig_location; 
vector <pii> graph[5][ROW_SIZE][COLUMN_SIZE];

struct Ant_DS
{
    pii location = beginnig_location;
    ll mod = 1, counter = 0;
};

void Initial()
{
    // Input
    cout << "\n\tEnter Row Size: \t";   cin >> row_size;
    cout << "\n\tEnter Column Size: \t";    cin >> column_size;
    cout << "\n\tEnter Number of Balls: \t";    cin >> num_ball;
    cout << "\n\tEnter Number of Holls: \t";    cin >> num_hole;
    out << row_size << ' ' << column_size << ' ' << num_ball << ' ' << num_hole << endl;
    do{
        beginnig_location.F = (rand() % row_size) + 1, beginnig_location.S = (rand() % column_size) + 1;
    }while(cell[beginnig_location.F][beginnig_location.S] != 0);
    out << beginnig_location.F - 1 << ' ' << beginnig_location.S - 1 << endl;
    cell[beginnig_location.F][beginnig_location.S] = 4;
    for(int i = 0; i < num_ball; i++)
    {
        do{
            ball[i].F = (rand() % row_size) + 1, ball[i].S = (rand() % column_size) + 1;
        }while(cell[ball[i].F][ball[i].S] != 0);
        out << ball[i].F - 1 << ' ' << ball[i].S - 1 << endl;
        cell[ball[i].F][ball[i].S] = 2;
    }
    for(int i = 0; i < num_hole; i++)
    {
        do{
            hole[i].F = rand() % row_size + 1, hole[i].S = rand() % column_size + 1;
        }while(cell[hole[i].F][hole[i].S] != 0);
        out << hole[i].F - 1 << ' ' << hole[i].S - 1 << endl;
        cell[hole[i].F][hole[i].S] = 3;
    }
    // Making Graph
    for(int r = 0; r < 5; r++)
    {
        ll go[4] = {1, 0, -1 , 0};
        for(int i = 1; i <= row_size; i++)
            for(int j = 1; j <= column_size; j++)
                for(int ii = 0; ii < 4; ii++)
                {
                    ll x = i + go[ii];
                    ll y = j + (go[(ii + 1) % 4]);
                    if(x <= row_size && x > 0 && y <= column_size && y > 0)
                        graph[r][i][j].pb({x, y});
                }
    }
}

Ant_DS Move(ll ant_num, Ant_DS ant)
{
    out << ant_num << ' ' << ant.location.F - 1 << ' ' << ant.location.S - 1 << ' ' << remind_ants <<  endl; 
    vector <pair<ll, pii>> node_random;
    for(auto node : graph[ant_num][ant.location.F][ant.location.S])
        if(mark[ant_num][node.F][node.S] < ant.mod)
        {
            ll last = 0;
            ll Phromone = phromone[ant.location.F][ant.location.S][node.F][node.S] * 3;
            if(node_random.size())
                last = node_random[node_random.size() - 1].F;
            node_random.pb({last + Phromone + 1, node});
        }
    if(!node_random.size())
    {   
        pii node = par[ant_num][ant.location.F][ant.location.S];
        ant.location.F = node.F;
        ant.location.S = node.S;
        ant.counter++;
        return ant;
    }  
    ll next = (rand() % node_random[node_random.size() - 1].F) + 1;
    for(auto i : node_random)
        if(next <= i.F)
        {
            par[ant_num][i.S.F][i.S.S] = ant.location;
            mark[ant_num][ant.location.F][ant.location.S] = ant.mod;
            ant.counter++;
            if(ant.mod == 3 || ant.mod == 2 || ant.mod == 4)
            {
                phromone[ant.location.F][ant.location.S][i.S.F][i.S.S]++;
                phromone[i.S.F][i.S.S][ant.location.F][ant.location.S]++;
            }
            ant.location.F = i.S.F, ant.location.S = i.S.S;
            if(ant.mod < 4 && ant.mod + 1 == cell[i.S.F][i.S.S])
                ant.mod++;
            break;
        }
    return ant;
}

void Ants_Travers()
{
    Ant_DS ant[4];
    while(remind_ants)
    {
        for(int i = 0; i < 4; i++)
        {
            ant[i] = Move(i, ant[i]);
            if(remind_ants && (ant[i].counter == STEP || ant[i].mod == 4))
            {
                remind_ants--;
                for(int i1 = 0; i1 < ROW_SIZE; i1++)
                    for(int i2 = 0; i2 < COLUMN_SIZE; i2++)
                        par[i][i1][i2] = {i1, i2}, mark[i][i1][i2] = 0;
                ant[i].location = beginnig_location;
                ant[i].counter = 0;
                ant[i].mod = 1;
            }
        }
    }
}

pii Move_Agent(pii node, ll mode)
{
    queue<pii> q;
    q.push(node);
    par[4][node.F][node.S] = node;
    while (q.size())
    {
        pii top = q.front();
        q.pop();
        mark[4][top.F][top.S] = 1;
        for (auto i : graph[4][top.F][top.S])
            if(!mark[4][i.F][i.S])
            {
                par[4][i.F][i.S] = top;
                if(cell[i.F][i.S] == mode + 2 && !used[i.F][i.S])
                {
                    vector <pii> ans;
                    used[i.F][i.S] = 1;
                    ans.pb(i);
                    do
                    {
                        ans.pb(par[4][ans[ans.size() - 1].F][ans[ans.size() - 1].S]);
                    }while(ans[ans.size() - 1] != node);
                    for( int j = ans.size() - 2; j >= 0; j--)
                        out << 4 << ' ' << ans[j].F - 1 << ' ' << ans[j].S - 1 << endl;
                    return i;
                }
                q.push(i);
            }
    }
    return node;
}

void Last_Agent_Travers()
{
    ll mode = 0;
    pii last = beginnig_location;
    for(int t = 0; t < num_ball * 2; t++)
    {
        for(int i = 0; i < ROW_SIZE; i++)
            for(int j = 0; j < COLUMN_SIZE; j++)
                mark[4][i][j] = 0;
        for(int i = 1; i <= ROW_SIZE; i++)
            for(int j = 1; j <= COLUMN_SIZE; j++)
                par[4][i][j] = {i, j};
        last = Move_Agent(last, mode);
        mode ^= 1;
    }
    
}

void Agent_Travers()
{
    Ant_DS Agent;
    ll counter = 30;
    while(counter && Agent.mod != 4)
    {
        Agent = Move(4, Agent);
        counter--;
    }
}

int main()
{
    // migmig;

    srand(time(0)); 
    Initial();
    Ants_Travers();
    Agent_Travers();
    // Last_Agent_Travers();
}