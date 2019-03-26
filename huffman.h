#pragma once
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <bitset>
using namespace std;
struct Node
{
    int ch;
    int num;
    Node *left;
    Node *right;
    string code;
    Node(unsigned int val1, unsigned int val2) : ch(val1), num(val2), left(nullptr), right(nullptr), code("") {}

    // 重载一个小于号进行排序
    // bool operator < (const Node* rhs ) const
    // {
    //     return num< rhs->num;
    // }
};
class Huffman
{
  public:
    Huffman(string s1, string s2);   
    void huffman_compress();
    void decompress_to_file();

  protected:
    fstream in_file;
    fstream out_file;
    string in_file_name;
    string out_file_name;
    map<unsigned int, int> huffnode;
    map<unsigned int, string> code;         // 用来存放code
    map<string, unsigned int> reverse_code; // 用来反转 rever_code
    vector<Node *> tree;    
    
    void get_map();
    void create_huffman_node();
    Node *create_huffman_tree();
    void create_huffman_code(Node *root);
    
};
bool cmp(Node *p1, Node *p2);