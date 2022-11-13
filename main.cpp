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


// 入力
int M;
double error;
int N = 100;
int E = N*(N-1)/2;
int Q = 100;
vector<string> G;

// 状態管理
vector<int> one_count;
vector<int> operated;

// 評価用 (abs(log(dif/ideal_di))の合計)
double ideal_dif;
double score = 0;

// 操作用
vector<int> dif_add;
vector<int> dif_del;

// check用
int ope_cnt = 0;

int calc_1_num(int cnt){
    return (cnt*(1-error)) + ((E-cnt)*error);
}

void input(){
    cin >> M >> error;
    G = vector<string>(M);
    one_count = vector<int>(M);
    operated = vector<int>(M);
    ideal_dif = (double)(E-M)/(M-1);
}

double calc_Eval(double x){
    return abs(log(x/ideal_dif));
}

void make_first(){
    for(int i=0; i<M; i++){
        one_count[i] = randXor()%(E+1);
    }
    sort(one_count.begin(),one_count.end());
    for(int i=0; i<M; i++){
        operated[i] = calc_1_num(one_count[i]);
    }
    for(int i=1; i<M; i++){
        int res = operated[i]-operated[i-1];
        // 初期スコア計算が入る
        score += calc_Eval(res);
    }
}


double Eval(){
    double dif = 0;
    for(auto x: dif_del) dif -= calc_Eval(x);
    for(auto x: dif_add) dif += calc_Eval(x);
    return dif;
}


void operation(){
    int idx = randXor()%M;
    int pre_pos;
    pre_pos = one_count[idx];
    dif_del = vector<int>(0);
    dif_add = vector<int>(0);
    if(idx != 0){
        int res = operated[idx]-operated[idx-1];
        dif_del.emplace_back(res);
    }
    if(idx != M-1){
        int res = operated[idx+1]-operated[idx];
        dif_del.emplace_back(res);
    }
    if(randXor()%2 == 0){
        // 左に動く
        int area;
        if(idx != 0){
            area = one_count[idx] - one_count[idx-1];
        }else{
            area = one_count[idx] - 0;
        }
        int move;
        if(area) move = randXor()%area;
        else move = 0;
        one_count[idx] -= move;
        operated[idx] = calc_1_num(one_count[idx]);
    }else{
        // 右に動く
        int area;
        if(idx != M-1){
            area = one_count[idx+1] - one_count[idx];
        }else{
            area = E - one_count[idx];
        }
        int move;
        if(area) move = randXor()%area;
        else move = 0;
        one_count[idx] += move;
        operated[idx] = calc_1_num(one_count[idx]);
    }
    if(idx != 0){
        int res = operated[idx]-operated[idx-1];
        dif_add.emplace_back(res);
    }
    if(idx != M-1){
        int res = operated[idx+1]-operated[idx];
        dif_add.emplace_back(res);
    }
    double res = Eval();
    if(res < 0 || ope_cnt%100 == 0){
        // 更新
        score += res;
        // cout << ope_cnt << " : " << score << endl;
    }else{
        //　戻す
        one_count[idx] = pre_pos;
        operated[idx] = calc_1_num(one_count[idx]);
    }
}

void solve(){
    auto STARTCLOCK = system_clock::now();
    double time = 0.0;
    const static double ENDTIME = 4.5;
    do{
        operation();
        ope_cnt++;
        time = (duration_cast<microseconds>(system_clock::now() - STARTCLOCK).count() * 1e-6);
    }while(time < ENDTIME);
}

void first_output(){
    cout << N << endl;
    for(int i=0; i<M; i++){
        G[i] = string(one_count[i],'1') + string(E-one_count[i],'0');
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
    int res = lower_bound(operated.begin(),operated.end(),cnt) - operated.begin();
    if(res == M){
        cout << M-1 << endl;
        return;
    }
    if(res == 0){
        cout << 0 << endl;
        return;
    }
    int dist1 = cnt - operated[res-1];
    int dist2 = operated[res] - cnt;
    if(dist1 < dist2){
        cout << res-1 << endl;
    }else{
        cout << res << endl;
    }
}

int main(){
    input();
    make_first();
    solve();
    // cout << ope_cnt << endl;
    // cout << ideal_dif << endl;
    // for(int i=0; i<M; i++){
    //     cout << one_count[i] << endl;
    // }
    // for(int i=1; i<M; i++){
    //     cout << operated[i]-operated[i-1] << endl;
    // }
    first_output();
    for(int i=0; i<Q; i++){
        query();
    }
    return 0;
}