#include "huffman.h"

// TODO: 能否用sheared_ptr 重新定义节点呢？？？？
int main()
{
    string s1 = "tmp.jpg";
    Huffman h(s1, "tmp.txt");
    auto t1 = chrono::system_clock::now();
    h.huffman_compress();
    auto t2 = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::seconds>(t2-t1).count() << endl;
    // exit(1);
    Huffman h2("tmp.txt", "aaaaaaaaa.jpg");
    auto t3 = chrono::system_clock::now();
    h2.decompress_to_file();
    auto t4 = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::seconds>(t4 - t3).count() << endl;
    return 0;
}