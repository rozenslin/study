// Aho-Corasick algo.
// problem:
//   given a list of patterns, P1, P2, ... Pk, total len = n
//   also a text string T, len = m
//   find all matches of those patterns in that T, with good timecost
//   which means it cannot be naive way of O(n*m), also not the KMP linear way of (k*m + n)
//
// main steps:
// 1. build a trie from the patterns, and make it an automaton, with intput as T, and output 
//    as the 'the matches info of patterns'
//    1.1 build the trie, time: O(n), with nodes starts from root, 
//        each node contains:
//              child nodes info
//    1.2 add the suffix/failure link into each node, this is for next jump in case of current path ends,
//    or next char from T not matches current path, instead of restart from root again
//    1.3 add the output links into each node, this is for the case of current matches means all its substring patterns
//    also match, to save the time
//
// 2. do the search work upon such automaton.
#include <iostream>
#include <string>
#include <map>
#include <queue>
using namespace std;

struct Node 
{
    // child links, with element as {char, Node*}
    map<char, Node*> child;
    Node* suffix;
    Node* output;
    int index;
};

Node* add_node()
{
// XXX no free now...
// mem issue? for child?
    Node* node = new Node;
    node->suffix = NULL; 
    node->output = NULL; 
    node->index = -1; 

    return node;
}

// in total: O(n)
Node* build_trie(vector<string>& patterns)
{
    Node* root = add_node();
    // traverse the trie to add the nodes for the chars in patterns.
    for (int i = 0; i < patterns.size(); i++)
    {
        Node* cur = root;
        string pat = patterns[i];
        for (int j = 0; j < pat.size(); j++)
        {
            char ch = pat[j];
            // found such char, follow it
            if (cur->child.count(ch))
            {
                cur = cur->child[ch];
                continue;
            }

            // if not found, create new node and add into trie
            Node* node = add_node();
            cur->child[ch] = node;
            cur = node;
        }

        // pattern ends, set the index
        cur->index = i;
    }

    return root;
}

// in total : O(n)
void suffix_n_output(Node* root)
{
    // do breadth first search for all nodes, and check all them, add the links
    // queue for FIFO
    queue<Node*> q;

    // root first, all root children has suffix to root 
    for (const auto& [k, v] : root->child) 
    {
        v->suffix = root;
        q.push(v);
    }
    root->suffix = root;

    // do BFS for all nodes.
    while (q.size())
    {
        Node* cur = q.front();
        q.pop();

        // 1. check for all children, add suffix links and add them into queue
        for (const auto& [k, v] : cur->child)
        {
            Node* temp = cur->suffix;
            // the suffix link of child must be the child of parent's suffix link.
            // or the parent suffix's suffix'suffix... till root
            // b c d e
            // a b c d e
            while (temp != root && !temp->child.count(k))
            {
                temp = temp->suffix;
            }

            if (temp->child.count(k))
            {
                v->suffix = temp->child[k];
            }
            else 
            {
                v->suffix = root;
            }

            // 1.2 add current child to queue
            q.push(v);
        }

        // 2. add output for current 
        //    the output link must be also the suffix of current one,
        //    i, tin, sting
        if (cur->suffix->index >= 0)
            cur->output = cur->suffix;
        else 
            cur->output = cur->suffix->output;
    }
}

// time: (m + z), z is number of matches
void search_matches(Node* root, string& text, map<char, vector<int> >& result)
{
    Node* cur = root;
    Node* temp;

    // go thru the trie, when found, follow the nodes, when failed/ended,
    // follow the suffix, when got output, store the matches
    for (int i = 0; i < text.size(); i++)
    {
        char ch = text[i];
        if (cur->child.count(ch))
        {
            cur = cur->child[ch];

            // if current is pattern ending
            if (cur->index >= 0) 
            {
                result[cur->index].push_back(i);
            }

            // if this has output link, follow and add all
            temp = cur->output;
            while (temp != NULL)
            {
                result[temp->index].push_back(i);
                temp = temp->output;
            }

            continue;
        }

        // if not found in current child, follow suffix
        cur = cur->suffix;
        while (cur != root && !cur->child.count(ch))
        {
            cur = cur->suffix;
        }

        // if found now, 
        if (cur->child.count(ch))
        {
            --i;
        }
        // means such char cannot be found anywhere...
    }
}

int main()
{
    vector<string> patterns = {"in", "no", "tin", "tina",  "tino",  "sting"};
    string text = "stinoin";

    Node* root = build_trie(patterns);

    suffix_n_output(root);

    // element as (char, list of pos matched)
    map<char, vector<int> > result;
    search_matches(root, text, result);

    cout << "patterns: ";
    for (const auto& s : patterns) 
    {
        cout << s << ", ";
    }
    cout << endl;

    cout << "text: " << text << endl;

    // output_result(&result);
    for (const auto& [k, v] : result) 
    {
        cout << "pattern : " << patterns[k] << " occurs at: ";
        for (const auto& pos : v) 
        {
            cout << pos << ", ";
        }
        cout << endl;
    }

    return 0;
}
