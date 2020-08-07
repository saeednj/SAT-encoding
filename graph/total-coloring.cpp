/*
 * SAT instance generator for Total graph coloring problem (Behzad's conjecture)
 * The code assumes a complete graph with n nodes (n is read
 * as the first command line argument) and the chromatic number
 * K is set to the conjectured upper bound D(G)+2.
 *
 * I assume nodes are colored from 1 to n.
 *
 */

#include <stdio.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;
typedef vector<int> Clause;
typedef pair<int, int> Pair;

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int m = n * (n - 1) / 2, K = n + 1;
    vector<Pair> edges;
    vector<Clause> formula;

    /* Prevent same color on node-edge */
    for( int i=0,u=0; u<n; u++ )
        for( int v=u+1; v<n; v++,i++ )
        {
            edges.push_back( Pair(u, v) );
            formula.push_back( {-(i * K + u + 1)} );
            formula.push_back( {-(i * K + v + 1)} );
        }

    for( int i=0; i<m; i++ )
        for( int j=i+1; j<m; j++ )
            if (
                    edges[i].first == edges[j].first ||
                    edges[i].first == edges[j].second ||
                    edges[i].second == edges[j].first ||
                    edges[i].second == edges[j].second )
            {
                for( int c=0; c<K; c++ )
                {
                    /* prevent same color for edge-edge */
                    formula.push_back( {-(i * K + c + 1), -(j * K + c + 1)} );
                }
            }

    for( int i=0; i<m; i++ )
    {
        Clause edgeClause;
        for( int cx=0; cx<K; cx++ )
        {
            edgeClause.push_back( i * K + cx + 1 );
            for( int cy=cx+1; cy<K; cy++ )
            {
                /* prevent multiple colors on an edge */
                formula.push_back( {-(i * K + cx + 1), -(i * K + cy + 1)} );
            }
        }
        /* force each edge to get a color */
        formula.push_back( edgeClause );
    }

    /* Printing the SAT formula in dimacs format */
    cout << "p cnf " << m * K << " " << formula.size() << endl;
    for( auto clause : formula )
    {
        for( int var : clause )
            cout << var << " ";
        cout << "0" << endl;
    }

    return 0;
}
