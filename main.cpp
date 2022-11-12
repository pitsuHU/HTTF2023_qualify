#include <iostream> // cout, endl, cin
#include <string> // string, to_string, stoi
#include <vector> // vector
#include <algorithm> // min, max, swap, sort, reverse, lower_bound, upper_bound
#include <utility> // pair, make_pair
#include <tuple> // tuple, make_tuple
#include <cstdint> // int64_t, int*_t
#include <cstdio> // printf
#include <map> // map
#include <queue> // queue, priority_queue
#include <set> // set
#include <stack> // stack
#include <deque> // deque
#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set
#include <bitset> // bitset
#include <cctype> // isupper, islower, isdigit, toupper, tolower
#include <iomanip> // setprecision
#include <complex> // complex
#include <math.h>
#include <functional>
#include <cassert>
using namespace std;
using ll = long long;
using P = pair<int,int>;
constexpr ll INF = 1e18;
// constexpr ll LLMAX = 9223372036854775807;
constexpr int inf = 1e9;
// constexpr ll mod = 1000000007;
constexpr ll mod = 998244353;
// 右下左上
const int dx[8] = {1, 0, -1, 0,1,1,-1,-1};
const int dy[8] = {0, 1, 0, -1,1,-1,1,-1};
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
ll gcd(ll a,ll b){if(b==0){return a;}else{return gcd(b,a%b);}};
#define eb emplace_back
#define pb pop_back
#define eol endl
// ---------------------------------------------------------------------------
#include <chrono>
#include <ctime>
using namespace chrono;

// 0以上UINT_MAX(0xffffffff)以下の整数をとる乱数
static uint32_t randXor() {
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;
    uint32_t t;
    
    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}


int M;
double error;
int N = 100;
int E = N*(N-1)/2;
int Q = 100;
vector<string> G;
vector<int> one_count;
P now_score(-1,0);
P best_score(-1,0);
vector<P> operated;
vector<int> best_one_count;
vector<P> best_operated;

int calc_1_num(int cnt){
    return (cnt*(1-error)) + ((E-cnt)*error);
}

void input(){
    cin >> M >> error;
    G = vector<string>(M);
    best_one_count = vector<int>(M);
    best_operated = vector<P>(M);
}



void calc_operated(vector<int> &A){
    vector<P> nxt_operated;
    for(int i=0; i<M; i++){
        nxt_operated.emplace_back(calc_1_num(A[i]),i);
    }
    sort(nxt_operated.begin(),nxt_operated.end());
    operated = nxt_operated;
}

P calc_score(){
    int mn = 1e9;
    int sum = 0;
    for(int i=1; i<M; i++){
        int res = operated[i].first - operated[i-1].first;
        sum += res*res;
        chmin(mn,res);
    }
    return P(mn,sum);
}

void random_first(){
    for(int i=0; i<M; i++){
        best_one_count[i] = randXor()%(E+1);
    }
    sort(best_one_count.begin(),best_one_count.end());
    calc_operated(best_one_count);
}


void operate(){
    int operating_pos = randXor()%M;
    if(randXor()%2 == 0){
        // 左に動く
        int area;
        if(operating_pos != 0){
            area = one_count[operating_pos] - one_count[operating_pos-1];
        }else{
            area = one_count[operating_pos] - 0;
        }
        one_count[operating_pos] -= randXor()%area;
    }else{ 
        // 右に動く
        int area;
        if(operating_pos != M-1){
            area = one_count[operating_pos+1] - one_count[operating_pos];
        }else{
            area = E - one_count[operating_pos];
        }
        one_count[operating_pos] -= randXor()%area;
    }
    calc_operated(one_count);
}


void judge_best(P score){
    if(score > best_score){
        best_score = score;
        best_one_count = one_count;
        best_operated = operated;
    }
}

void solve(){
    auto STARTCLOCK = system_clock::now();
    double time = 0.0;
    const static double ENDTIME = 4.5;
    do{
        one_count = best_one_count;
        operate();
        P score = calc_score();
        judge_best(score);
        time = (duration_cast<microseconds>(system_clock::now() - STARTCLOCK).count() * 1e-6);
    }while(time < ENDTIME);
}

void first_output(){
    cout << N << endl;
    for(int i=0; i<N; i++){
        G[i] = string('1',best_one_count[i]) + string('0',E-best_one_count[i]);
        cout << G[i] << endl;
    }
}


void query(){
    string S;
    cin >> S;
    int cnt = 0;
    for(int i=0; i<E; i++){
        if(S[i] == '1') cnt++;
    }
    int res = lower_bound(best_operated.begin(),best_operated.end(),cnt) - best_operated.begin();
    if(res == M){
        cout << best_operated[M-1].second << endl;
        return;
    }
    if(res == 0){
        cout << best_operated[0].second << endl;
        return;
    }
    int dist1 = cnt - best_operated[res-1].first;
    int dist2 = best_operated[res].first - cnt;
    if(dist1 < dist2){
        cout << best_operated[res-1].second << endl;
    }else{
        cout << best_operated[res].second << endl;
    }
}

int main(){
    input();
    random_first();
    solve();
    first_output();
    for(int i=0; i<Q; i++){
        query();
    }
    return 0;
}