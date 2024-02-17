#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
dfs�� ���� ����Ŭ�� ���� �Ǻ�
1. �ܼ� ����Ŭ�� ���� : dfs tree���� ������ ������ ��
2. ���� ����Ŭ�� ���� : ����Ŭ�� �����ִ� ���°� ����
A. �ܼ� ����Ŭ ���� �˰����� ����
*/
const int MAX_N = 100005;
const int INF = 1e9;

int N, M;

struct Cycle{
    int Size;
    vector<int> c;
    vector< pair<int, int> > uni;
    int get_val(int idx){
        while(idx<0) idx += Size;
        idx%=Size;
        return c[idx];
    }
    bool operator < (const Cycle &a){
        return Size > a.Size;
    }
} C[MAX_N];

bool cmp(const Cycle &a, const Cycle &b)
{
    return a.Size < b.Size;
}

Cycle new_;
int cn = 0;

void print_(Cycle &a)
{
    printf("\nSize : %d\n", a.Size);
    for(int i=0; i<a.Size; i++){
        printf("%d ", a.c[i]);
    }
    printf("\n");
}

int Next[MAX_N][4], check[MAX_N], pos[MAX_N], start_pos;
bool Possible_a, Possible_b;

void addEdge(int a, int b, int check_num)
{
    if(check[a]!=check_num){
        check[a] = check_num;
        pos[a] = 0;
    }
    if(pos[a]>=2){
        Possible_b = 0;
        return;
    }
    if(start_pos == -1) start_pos = a;
    Next[a][pos[a]++] = b;
}

bool in_check(Cycle &c)
{
    for(int i=1; i<=cn; i++){
        if(c.Size == C[i].Size){
            bool f = 1;
            for(int j=0; j<c.Size; j++){
                if(c.uni[j].first != C[i].uni[j].first){
                    f = 0;
                    break;
                }
            }
            if(f) return true;
        }
    }
    return false;
}

// O(NlogN)
bool Cycle_XOR(Cycle &a, Cycle &b, int check_num)
{
    // print_(a);
    // print_(b);
    int p1 = 0, p2 = 0;
    start_pos = -1;
    Possible_a = 0, Possible_b = 1;
    while(1){
        if(p1 == a.Size && p2 == b.Size) break;
        int P = (p1<a.Size ? a.uni[p1].first : INF);
        int Q = (p2<b.Size ? b.uni[p2].first : INF);

        int na1, na2, nb1, nb2;
        if(P!=INF) na1 = a.get_val(a.uni[p1].second-1), na2 = a.get_val(a.uni[p1].second+1);
        if(Q!=INF) nb1 = b.get_val(b.uni[p2].second-1), nb2 = b.get_val(b.uni[p2].second+1);
        if(P == Q){
            if(na1!=nb1 && na1!=nb2) addEdge(na1, P, check_num);
            if(na2!=nb1 && na2!=nb2) addEdge(na2, P, check_num);
            if(nb1!=na1 && nb1!=na2) addEdge(nb1, Q, check_num);
            if(nb2!=na1 && nb2!=na2) addEdge(nb2, Q, check_num);
            if(na1==nb1 || na2==nb1 || na1==nb2 || na2==nb1) Possible_a = 1;
            p1++, p2++;
        }
        else if(P < Q){
            addEdge(na1, P, check_num);
            addEdge(na2, P, check_num);
            p1++;
        }
        else{
            addEdge(nb1, Q, check_num);
            addEdge(nb2, Q, check_num);
            p2++;
        }
        if(!Possible_b) break;
    }
    if(!(Possible_a && Possible_b)) return 0;

    Cycle new_C;
    int x = start_pos, parent = -1, cnt = 0;
    do{
        new_C.c.push_back(x);
        new_C.uni.push_back({x, cnt++});
        if(pos[x]!=2) return 0;
        if(parent!=Next[x][0]) parent = x, x = Next[x][0];
        else parent = x, x = Next[x][1];
    } while(x!=start_pos);

    sort(new_C.uni.begin(), new_C.uni.end());
    new_C.Size = new_C.c.size();
    if(!in_check(new_C)) new_ = new_C;
    else return 0;
    return 1;
}

bool Sub(Cycle &a, Cycle &b)
{
    if(a.Size < b.Size) return 0;
    for(auto &i : b.uni){
        int x = i.first;
        int s = -1, e = a.Size;
        while(s+1 < e){
            int mid = s + e >> 1;
            if(a.uni[mid].first > x) e = mid;
            else s = mid;
        }
        if(a.uni[s].first != x) return 0;
    }
    return 1;
}

vector<int> adj[MAX_N];
int num[MAX_N], cnt, par[MAX_N];
int finished[MAX_N], inCycle[MAX_N], CountBack;

void dfs(int x)
{
    num[x] = ++cnt;
    for(int nxt : adj[x]){
        if(par[x] == nxt) continue;
        if(!num[nxt]){
            par[nxt] = x;
            dfs(nxt);
        }
        else if(num[nxt] > num[x]) continue;
        else if(!finished[nxt]){
            CountBack++;
            int k = x, idx = 0;
            cn++;
            while(k!=nxt){
                C[cn].c.push_back(k);
                C[cn].uni.push_back({k, idx++});
                k = par[k];
            }
            C[cn].c.push_back(nxt);
            C[cn].uni.push_back({nxt, idx++});
            sort(C[cn].uni.begin(), C[cn].uni.end());
            C[cn].Size = C[cn].c.size();
        }
    }
    finished[x] = 1;
}

bool visited[MAX_N];

int main()
{
    scanf("%d %d", &N, &M);
    for(int i=1; i<=M; i++){
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1);
    for(int i=1; i<=cn; i++){
        for(int j=0; j<C[i].c.size(); j++){
            printf("%d ", C[i].c[j]);
        }
        printf("\n");
        for(auto &j : C[i].uni){
            printf("(%d %d) ", j.first, j.second);
        }
        printf("\n");
    }
    int num = 1;
    /*
    while(1){
        bool Br = 0;
        priority_queue< pair<int, int> > pq;
        for(int i=1; i<=cn; i++) pq.push({-C[i].Size, i});
        while(!pq.empty()){
            int x = pq.top().second;
            if(C[x].Size!=-pq.top().first){
                pq.pop();
                break;
            }
            pq.pop();
            for(int i=1; i<=cn; i++){
                if(i!=x && Cycle_XOR(C[x], C[i], num++)){
                    if(new_.Size < C[i].Size){
                        C[i] = new_;
                        pq.push({new_.Size, i});
                        Br = 1;
                    }
                }
            }
        }
        if(!Br) break;
    }
    */
    for(int i=1; i<=cn; i++){
        for(int j=1; j<=cn; j++){
            if(i==j) continue;
            if(Cycle_XOR(C[i], C[j], num++)){
                C[++cn] = new_;
            }
        }
    }
    sort(C+1, C+cn+1, cmp);
    printf("======================================\n");
    for(int i=1; i<=cn; i++){
        if(!visited[i]){
            for(auto &j : C[i].c) printf("%d ", j);
            printf("\n");
        }
    }
    printf("%d\n", CountBack);
    return 0;
}


/*
11 12
1 2
1 3
1 11
2 6
2 9
3 4
4 5
4 9
5 10
6 7
7 8
8 11


11 12
1 3
1 6
1 9
2 7
3 7
4 5
4 8
5 6
6 7
7 10
8 11
10 11


11 12
1 2
2 7
2 8
2 9
3 11
4 5
4 9
4 11
5 11
6 7
8 9
9 10


15 22
1 2
1 7
1 11
1 13
1 15
2 12
3 5
3 8
3 14
4 7
4 14
5 14
6 11
7 9
7 11
7 13
7 14
8 11
9 15
11 12
11 15
14 15
*/
