//HDU3377 插头DP
//最小表示法, 相同连通块的插头用相同数字，独立插头为1，没有插头为0
#include <bits/stdc++.h>
#define ll long long
#define mst(a,x) memset(a,x,sizeof(a))
#define For(i, t) for(int i = 0; i < t; i++)
#define Debug(x) cerr << #x << " = "  << x << endl;
using namespace std;


const int N = 15;
const int MOD = 30011;
//s表示轮廓线上插头，左->右，标号s[0] ~ s[m]
int n, m, a[N][N], vis[N], s[N];

void encode(int *s, ll &sta) {
	int tot = 2;
	
	mst(vis, -1);
	vis[0] = 0;
	vis[1] = 1;

	sta = 0;
	For(i, m + 1) {
		if(vis[s[i]] == -1) vis[s[i]] = tot++;
		s[i] = vis[s[i]];
		sta <<= 3;
		sta |= s[i];
	}
}

void decode(ll sta, int *s) {
	for(int i = m; ~i; i--) {
		s[i] = sta & 7;
		sta >>= 3;
	}
}

//储存dp到当前格，插头为sta对应的dp值
//sta很大，用hash来存
struct HashMap{
	//M表示状态总数，不知道怎么算，看情况开大吧
	#define M 500005 
	int fst[MOD], tot;
	int nxt[M], dp[M];
	ll v[M];
	
	void init() {
		tot = 0;
		mst(fst, -1);
	}

	void pushSta(ll sta, int val) {
		int Hash = sta % MOD;

		for(int p = fst[Hash]; ~p; p = nxt[p]) {
			if(v[p] == sta) {
				dp[p] = max(dp[p], val);
				return;
			}
		}
		v[tot] = sta;
		dp[tot] = val;
		nxt[tot] = fst[Hash];
		fst[Hash] = tot++;
	}

	void push(int *s, int val) {
		ll sta;

		encode(s, sta);
		pushSta(sta, val);
	}
}HM[2];
#define cur HM[now]
#define pre HM[now ^ 1]

int main() {
	int cas = 0;

	while(~scanf("%d%d", &n, &m)) {
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= m; j++) 
				scanf("%d", &a[i][j]);

		int now = 0;
		cur.init();
		mst(s, 0);
		s[0] = 1;
		cur.push(s, 0);

		for(int i = 1; i <= n; i++)
			for(int j = 0; j <= m; j++) {
				now ^= 1;
				cur.init();
				For(p, pre.tot) {
					ll sta = pre.v[p];
					int val = pre.dp[p];
					decode(sta, s);
					if(!j) {
						//最后一列转移到第0列
						if(s[m]) continue;
						for(int t = m; t; t--) s[t] = s[t - 1];
						s[0] = 0;
						cur.push(s, val);
					}
					else {
						//  __        
						// |    -> __| i行j列的左上转移到右下
						// 如果时间卡的紧，应该尽早判断能否加新插头(判断R, D)
						int L = s[j - 1];
						int U = s[j];
						//00 -> 00  xx
						if(!L && !U) {
							cur.push(s, val);
							s[j] = s[j - 1] = 7;
							cur.push(s, val + a[i][j]);
						}
						//0x x0 -> 0x x0
						if(!L && U || L && !U) {
							cur.push(s, val + a[i][j]);
							swap(s[j - 1], s[j]);
							cur.push(s, val + a[i][j]);
						}
						//12 -> 00 余下2变成0	
						if(L == 1 && U > 1) {
							s[j] = s[j - 1] = 0;
							For(t, m + 1) if(s[t] == U) s[t] = 1;
							cur.push(s, val + a[i][j]);
						}
						//21 -> 00 ...	
						if(L > 1 && U == 1) {
							s[j] = s[j - 1] = 0;
							For(t, m + 1) if(s[t] == L) s[t] = 1;
							cur.push(s, val + a[i][j]);
						}
						//23 -> 00, 余下23变成0
						if(L > 1 && U > 1) {
							if(L == U) continue;
							s[j] = s[j - 1] = 0;
							For(t, m + 1) if(s[t] == L || s[t] == U) s[t] = 7;
							cur.push(s, val + a[i][j]);
						}
					}
				}
			}
		ll ans = 0;
		for(int p = cur.fst[1]; ~p; p = cur.nxt[p]) {
			if(cur.v[p] == 1) {
				ans = cur.dp[p];
				break;
			}
		}
		printf("Case %d: %lld\n", ++cas, ans);
	}
	return 0;
}

