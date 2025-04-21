// #define _GLIBCXX_DEBUG 1
#include <bits/stdc++.h>
using namespace std;

#define int long long
#define ll long long
#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
#define printv(x) for (int iz = 0; iz < sz(x); iz++) cout << (x)[iz] << ((iz == sz(x)-1) ? '\n' : ' ')
#define printvv(x) for (int jz = 0; jz < sz(x); jz++) printv((x)[jz])

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;

const int inf = 1e15;

int neddlemanWunsch(const string& s, const string& t, vvi& dp)
{
    dp.assign(sz(s)+1, vi(sz(t)+1, -inf));
    dp[0][0] = 0;

    rep(i, 0, sz(s)+1)
    {
        rep(j, 0, sz(t)+1)
        {
            if (i)
                dp[i][j] = dp[i-1][j] - 2;
            if (j)
                dp[i][j] = max(dp[i][j], dp[i][j-1] - 2);
            if (i&&j)
                dp[i][j] = max(dp[i][j], dp[i-1][j-1] + (s[i-1] == t[j-1] ? 1 : -1));
        }
    }

    return dp[sz(s)][sz(t)];
}

pair<string, string> neddlemanWunschRecoverOne(const string& s, const string& t, const vvi& dp)
{
    string sp, tp;

    for (int i = sz(s), j = sz(t); true; )
    {
        if (i>0 && dp[i-1][j] - 2 == dp[i][j])
        {
            tp.push_back('-');
            sp.push_back(s[i-1]);
            i--;
        }
        else if (j>0 && dp[i][j-1] - 2 == dp[i][j])
        {
            sp.push_back('-');
            tp.push_back(t[j-1]);
            j--;
        }
        else if (i>0 && j>0 && dp[i-1][j-1] + (s[i-1] == t[j-1] ? 1 : -1) == dp[i][j])
        {
            sp.push_back(s[i-1]);
            tp.push_back(t[j-1]);
            i--;
            j--;
        }
        else
            break;
    }

    reverse(all(sp));
    reverse(all(tp));

    return {sp, tp};
}

void neddlemanWunschRecoverAll(const string& s, const string& t, const vvi& dp, vector<pair<string, string>>& out)
{
    out.clear();

    queue<pair<pair<int, int>, pair<string, string>>> q;
    q.push({{sz(s), sz(t)}, {"", ""}});

    while (!q.empty())
    {
        auto [i, j] = q.front().first;
        auto [sp, tp] = q.front().second;
        q.pop();
        
        while (true)
        {
            bool processed = false;
            int ci = i, cj = j;
            string csp = sp, ctp = tp;

            if (i>0 && dp[i-1][j] - 2 == dp[i][j])
            {
                sp.push_back(s[i-1]);
                tp.push_back('-');
                i--;
                processed = true;
            }
            if (j>0 && dp[i][j-1] - 2 == dp[i][j])
            {
                if (!processed)
                {
                    sp.push_back('-');
                    tp.push_back(t[j-1]);
                    j--;
                    processed = true;
                }
                else
                    q.push({{ci, cj-1}, {csp + "-", ctp + t.substr(cj-1, 1)}});
            }
            if (i>0 && j>0 && dp[i-1][j-1] + (s[i-1] == t[j-1] ? 1 : -1) == dp[i][j])
            {
                if (!processed)
                {
                    sp.push_back(s[i-1]);
                    tp.push_back(t[j-1]);
                    i--;
                    j--;
                    processed = true;
                }
                else
                    q.push({{ci-1, cj-1}, {csp + s.substr(i-1, 1), ctp + t.substr(j-1, 1)}});
            }

            if (!processed)
                break;
        }

        reverse(all(sp));
        reverse(all(tp));
        out.push_back({sp, tp});
    }
}

int SPScore(const vector<string>& s)
{
    int n = sz(s);
    int ans = 0;
    rep(k, 0, sz(s[0]))
    {
        rep(i, 0, n)
        {
            rep(j, i+1, n)
            {
                char const* p1 = &s[i][k];
                char const* p2 = &s[j][k];

                if (*p1 > *p2)
                    swap(p1, p2);

                if (*p1 == '-')
                {
                    if (*p2 != '-')
                        ans -= 2;
                    continue;
                }
                ans += (*p1 == *p2) ? 1 : -1;
            }
        }

        // cout << '\t' << ans << endl;
    }
    return ans;
}

void MSAStar(const vector<string>& s, vector<string>& ans)
{
    int n = sz(s);
    vvi dp;

    vi vals(n);
    rep(i, 0, n)
    {
        rep(j, i+1, n)
        {
            int v = neddlemanWunsch(s[i], s[j], dp);
            vals[i] += v;
            vals[j] += v;
        }
    }

    int c = max_element(all(vals)) - vals.begin();

    ans.assign(n, "");
    ans[c] = s[c];

    rep(i, 0, n)
    {
        if (i == c)
            continue;

        neddlemanWunsch(s[c], s[i], dp);
        auto [asc, asi] = neddlemanWunschRecoverOne(s[c], s[i], dp);
        ans[i] = asi;

        vi addPrev;
        vi addCurr;

        int k, j;
        for (k = 0, j = 0; k < sz(asc) && j < sz(ans[c]); )
        {
            if (asc[k] == ans[c][j])
            {
                j++; k++;
                continue;
            }
            if (asc[k] == '-')
            {
                k++;
                addPrev.push_back(j);
                continue;
            }
            if (ans[c][j] == '-')
            {
                j++;
                addCurr.push_back(k);
                continue;
            }
        }

        for (; k < sz(asc); k++)
            addPrev.push_back(sz(ans[c]));

        for (; j < sz(ans[c]); j++)
            addCurr.push_back(sz(asc));

        reverse(all(addPrev));
        reverse(all(addCurr));
        for (auto pos : addPrev)
        {
            rep(j, 0, i)
            {
                if (j != c)
                    ans[j].insert(pos, 1, '-');
            }
            ans[c].insert(pos, 1, '-');
        }
        for(auto pos : addCurr)
            ans[i].insert(pos, 1, '-');

        // for (auto& s : ans)
        // {
        //     cout << s << endl;
        // }
        // cout << "-----" << endl;
    }
}

void jukesCantorMatrix(const vector<string>& s, vector<vector<double>>& d)
{
    d.assign(sz(s), vector<double>(sz(s)));

    vector<string> alignment;
    MSAStar(s, alignment);

    int tot = 0;
    int diff = 0;
    rep(i, 0, sz(s))
    {
        rep(j, i+1, sz(s))
        {
            rep(k, 0, sz(s[i]))
            {
                if (s[i][k] == '-' || s[j][k] == '-')
                    continue;
                tot++;
                diff += s[i][k] != s[j][k];
            }
            d[i][j] = d[j][i] = -0.75 * log(1 - (4.0/3.0) * diff / tot);
        }
    }
}

void distanceMatrix(const vector<string>& s, vector<vector<double>>& d)
{
    d.assign(sz(s), vector<double>(sz(s)));

    vector<string> alignment;
    MSAStar(s, alignment);

    int tot = 0;
    int diff = 0;
    rep(i, 0, sz(s))
    {
        rep(j, i+1, sz(s))
        {
            rep(k, 0, sz(s[i]))
            {
                if (s[i][k] == '-' || s[j][k] == '-')
                    continue;
                tot++;
                diff += s[i][k] != s[j][k];
            }
            d[i][j] = d[j][i] = (double)diff / tot;
        }
    }
}

struct DSU {
	vi e;
	DSU(int n):e(n,-1) {}
	int size(int x) {return -e[find(x)];}
	int find(int x) {return e[x]<0?x:(e[x]=find(e[x]));}
	bool join(int a, int b) {
		a=find(a), b=find(b);
		if(a==b) return 0;
		if(e[a]>e[b]) swap(a,b);
		e[a]+=e[b];
		e[b]=a;
		return 1;
	}
};

void UPGMA(const vector<vector<double>>& dist, vector<pair<double, array<int, 2>>>& merge)
{
    int n = sz(dist);
    merge.clear();
    vector<vector<double>> d = dist;
    vector<char> active(n, true);

    set<pair<double, array<int, 2>>> pq;
    DSU dsu(n);

    rep(i, 0, n)
    {
        rep(j, i+1, n)
        {
            pq.insert({d[i][j], {i, j}});
        }
    }

    // for (auto &[dd, pp] : pq)
    //     cout << dd << '|' << pp[0] << '|' << pp[1] << ' ';
    // cout << endl;

    while (!pq.empty())
    {
        auto [dd, pp] = *pq.begin();
        auto [i, j] = pp;
        pq.erase(pq.begin());
        int a = dsu.find(i); int sza = dsu.size(a);
        int b = dsu.find(j); int szb = dsu.size(b);
        dsu.join(i, j);
        if (a != dsu.find(i))
        {
            swap(a, b);
            swap(sza, szb);
        }
        active[b] = false;
        merge.push_back({dd, {a, b}});

        for (int k = 0; k < n; k++)
        {   
            if (active[k] && k != a)
            {
                pq.erase({d[a][k], {min(a, k), max(a, k)}});
                pq.erase({d[b][k], {min(b, k), max(b, k)}});

                d[a][k] = (sza * d[a][k] + szb * d[b][k]) / (sza + szb);
                d[k][a] = d[a][k];

                pq.insert({d[a][k], {min(a, k), max(a, k)}});
            }
        }

        // cout << endl << '\t' << a << ' ' << b << endl;
        // for (auto &[dd, pp] : pq)
        //     cout << dd << '|' << pp[0] << '|' << pp[1] << ' ';
        // cout << endl;
    }
}

void neighborJoining(const vector<vector<double>>& dist, vector<pair<array<double, 2>, array<int, 2>>>& merge)
{
    int n = sz(dist);
    merge.clear();
    vector<vector<double>> d = dist;
    vector<char> active(n, true);
    vector<double> u(n);

    // set<pair<double, array<int, 2>>> pq;
    DSU dsu(n);

    rep(i, 0, n)
        rep(j, 0, n)
            u[i] += d[i][j];

    for (int it = 0; it < n-2; it++)
    {
        int ii, jj, dd = inf;
        rep(i, 0, n)
        {
            if (!active[i])
                continue;
            rep(j, i+1, n)
            {
                if (!active[j])
                        continue;
                if ((n-2)*d[i][j]-u[i]-u[j] < dd)
                {
                    dd = (n-2)*d[i][j]-u[i]-u[j];
                    ii = i; jj = j;
                }
            }
        }
        int a = dsu.find(ii); int sza = dsu.size(a);
        int b = dsu.find(jj); int szb = dsu.size(b);
        dsu.join(ii, jj);
        if (a != dsu.find(ii))
        {
            swap(a, b);
            swap(sza, szb);
        }
        active[b] = false;
        merge.push_back({
            {(d[a][b] + (u[a]-u[b])/(n-it-2)) / 2, (d[a][b] + (u[b]-u[a])/(n-it-2)) / 2},
            {a, b}
        });
        
        u[a] = 0;
        for (int k = 0; k < n; k++)
        {   
            if (active[k] && k != a)
            {
                u[k] -= d[a][k] + d[b][k];

                d[a][k] = (d[a][k] + d[b][k] - d[a][b]) / 2;
                d[k][a] = d[a][k];

                u[k] += d[a][k];
                u[a] += d[a][k];
            }
        }
    }

    rep(i, 0, n)
    {
        if (!active[i])
            continue;
        rep(j, i+1, n)
        {
            if (active[j])
            {
                merge.push_back({{d[i][j]/2, d[i][j]/2}, {i, j}});
                return;
            }
        }
    }
}

string newickStringUPGMA(const vector<pair<double, array<int, 2>>>& merge, const vector<string>& labels)
{
    int n = sz(labels);
    vector<string> partial(n);
    vector<double> last(n);
    rep(i, 0, n)
    {
        partial[i] = labels[i];
    }
    for (auto &[d, p] : merge)
    {
        partial[p[0]] = "(" + partial[p[0]] + ":" + to_string(d/2 - last[p[0]]) + "," + partial[p[1]] + ":" + to_string(d/2 - last[p[1]]) + ")";
        partial[p[1]].clear();
        last[p[0]] = d/2;
        last[p[1]] = d/2;
    }
    return partial[merge[sz(merge)-1].second[0]] + ";";
}

string newickStringUPGMA(const vector<pair<double, array<int, 2>>>& merge)
{    
    int n = -inf;
    for (auto &[d, p] : merge)
    {
        n = max(n, p[0]);
        n = max(n, p[1]);
    }
    n++;
    vector<string> labels(n);
    rep(i, 0, n)
        labels[i] = to_string(i+1);
    return newickStringUPGMA(merge, labels);
}

string newickStringNeighborJoining(const vector<pair<array<double, 2>, array<int, 2>>>& merge, const vector<string>& labels)
{
    int n = sz(labels);
    vector<string> partial(n);
    rep(i, 0, n)
    {
        partial[i] = labels[i];
    }
    for (auto &[d, p] : merge)
    {
        partial[p[0]] = "(" + partial[p[0]] + ":" + to_string(d[0]) + "," + partial[p[1]] + ":" + to_string(d[1]) + ")";
        partial[p[1]].clear();
    }
    return partial[merge[sz(merge)-1].second[0]] + ";";
}

string newickStringNeighborJoining(const vector<pair<array<double, 2>, array<int, 2>>>& merge)
{    
    int n = -inf;
    for (auto &[d, p] : merge)
    {
        n = max(n, p[0]);
        n = max(n, p[1]);
    }
    n++;
    vector<string> labels(n);
    rep(i, 0, n)
        labels[i] = to_string(i+1);
    return newickStringNeighborJoining(merge, labels);
}

void solve()
{
    vector<vector<double>> d = {
        {0, 8, 7, 12},
        {8, 0, 9, 14},
        {7, 9, 0, 11},
        {12, 14, 11, 0}
    };

    vector<pair<double, array<int, 2>>> merge;
    UPGMA(d, merge);
    cout << newickStringUPGMA(merge, {"A", "B", "C", "D"}) << '\n';

    vector<pair<array<double, 2>, array<int, 2>>> merge2;
    neighborJoining(d, merge2);
    cout << newickStringNeighborJoining(merge2, {"A", "B", "C", "D"}) << '\n';

    vector<string> s = {
        "ATTGCCATT",
        "ATGGCCATT",
        "ATCCAATTTT",
        "ATCTTCTT",
        "ACTGACC"
    };
    jukesCantorMatrix(s, d);
    UPGMA(d, merge);    
    cout << newickStringUPGMA(merge, {"S1", "S2", "S3", "S4", "S5"}) << '\n';
    neighborJoining(d, merge2);    
    cout << newickStringNeighborJoining(merge2, {"S1", "S2", "S3", "S4", "S5"}) << '\n';
    
    s = {
        "CACERES URQUIZO",
        "CARAZAS BARRIOS",
        "LEON ZARATE",
        "TUPAC VALDIVIA",
        "GUTIERREZ PACHAS",
        "GUTIERREZ CACERES"
    };
    vector<string> ini = {
        "C.U.",
        "C.B.",
        "L.Z.",
        "T.V.",
        "G.P.",
        "G.C."
    };
    distanceMatrix(s, d);
    UPGMA(d, merge);    
    cout << newickStringUPGMA(merge, ini) << '\n';
    neighborJoining(d, merge2);    
    cout << newickStringNeighborJoining(merge2, ini) << '\n';
}

signed main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit);

    // int t;
    // cin >> t;
    // while (t--)
        solve();
    
}