#include "huffman.h"
bool cmp(Node *p1, Node *p2)
{
    return p1->num < p2->num;
}
Huffman::Huffman(string s1, string s2)
{
    in_file_name = s1;
    out_file_name = s2;
}
// 创建节点
void Huffman::create_huffman_node()
{
    for (auto start = huffnode.cbegin(); start != huffnode.cend(); ++start)
    {
        Node *tmp = new Node(start->first, start->second);
        tree.push_back(tmp);
    }
}
// 创建哈夫曼树
Node *Huffman::create_huffman_tree()
{
    create_huffman_node();
    while (1)
    {
        // 从大到小排序
        sort(tree.rbegin(), tree.rend(), cmp);
        auto node1 = tree.back();
        tree.pop_back();
        auto node2 = tree.back();
        tree.pop_back();

        Node *tmp = new Node(-1, node1->num + node2->num);
        tmp->left = node1;
        tmp->right = node2;
        tree.push_back(tmp);
        if (tree.size() == 1)
            break;
    }
    // 返回根路径
    return tree.back();
}

// 创建huffman编码树
// TODO: 用count做一遍
void Huffman::create_huffman_code(Node *root)
{
    if (root == nullptr)
    {
        ;
    }
    else
    {
        // 不是叶子节点把它变成-1
        if (root->ch != -1)
        {
            // 记录各个叶子节点的编码
            code[root->ch] = root->code;
            reverse_code[root->code] = root->ch;
        }

        if (root->left != nullptr)
        {
            root->left->code = root->code + '0';
            create_huffman_code(root->left);
        }
        if (root->right != nullptr)
        {
            root->right->code = root->code + '1';
            create_huffman_code(root->right);
        }
    }
}
// 得到关联容器
void Huffman::get_map()
{
    unsigned char ch;
    unsigned int num;
    in_file.open(in_file_name, ios::in | ios::binary);
    while (!in_file.eof())
    {
        in_file.read((char *)&ch, sizeof(char));
        num = static_cast<unsigned int>(ch);
        ++huffnode[num];
    }
    in_file.close();
}

void Huffman::huffman_compress()
{
    // cout << this->in_file_name << endl; 
    // 4s
    auto t3 = chrono::system_clock::now();
    get_map();
    auto t4 = chrono::system_clock::now();
    cout << "get_map time = " << chrono::duration_cast<chrono::seconds>(t4-t3).count() << endl;



    // 测试得到花了多长时间 0s
    auto t1 = chrono::system_clock::now();
    // 得到huffman树
    Node *root = create_huffman_tree();
    auto t2 = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::seconds>(t2-t1).count() << endl;


    // 编码Huffman树 0s
    auto t5 = chrono::system_clock::now();
    create_huffman_code(root);
    auto t6 = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::seconds>(t6-t5).count() << endl;


    auto t7 = chrono::system_clock::now();
    
    unsigned int size = huffnode.size();
    // 开始写入
    // 打开输出文件
    out_file.open(out_file_name, ios::out | ios::binary);

    unsigned char byte1 = 0;
    unsigned char byte2 = 0;
    unsigned char byte3 = 0;
    unsigned char byte4 = 0;
    byte1 = size & 0xff;
    size = size >> 8;
    byte2 = size & 0xff;
    size = size >> 8;
    byte3 = size & 0xff;
    size = size >> 8;
    byte4 = size & 0xff;
    size = size >> 8;
    out_file.write((char *)&byte4, sizeof(char));
    out_file.write((char *)&byte3, sizeof(char));
    out_file.write((char *)&byte2, sizeof(char));
    out_file.write((char *)&byte1, sizeof(char));

    for (auto start = huffnode.cbegin(); start != huffnode.cend(); ++start)
    {
        out_file.write((char *)&start->first, sizeof(char));
        unsigned int tmp = start->second;
        unsigned char byt1 = 0;
        unsigned char byt2 = 0;
        unsigned char byt3 = 0;
        unsigned char byt4 = 0;
        byt1 = tmp & 0xff;
        tmp = tmp >> 8;

        byt2 = tmp & 0xff;
        tmp = tmp >> 8;

        byt3 = tmp & 0xff;
        tmp = tmp >> 8;

        byt4 = tmp & 0xff;
        tmp = tmp >> 8;

        out_file.write((char *)&byt4, sizeof(char));
        out_file.write((char *)&byt3, sizeof(char));
        out_file.write((char *)&byt2, sizeof(char));
        out_file.write((char *)&byt1, sizeof(char));
    }
    unsigned char temp;
    // int i;
    // int bytenum = 0;
    // 打开文件
    in_file.open(in_file_name, ios::in | ios::binary);
    // 开始读文件
    in_file.read((char *)&temp, sizeof(char));
    
    // 写入文件
    // 被我舍弃掉结束符了 \0
    string code_str = "";
    while (!in_file.eof())
    // while(1)
    {
        code_str += code[temp];
        // cout << "the temp= " << (int) temp << endl;
        // cout << code_str << endl;
        // for (i = 0; i < code[temp].length(); i++)
        // {
        // 	ch <<= 1;   //将读到的字符左移一位
        // 	bytenum++;
        // 	if (code[temp][i] == '1')
        // 		ch = ch | 1;
        // 	if (bytenum == 8) //当字节位移满八次以后进行一次压缩
        // 	{
        // 		out_file.write((char *)&ch, sizeof(char));
        // 		bytenum = 0;
        // 		ch = 0;
        // 	}
        // }
        // cout << code_str << endl;

        while (code_str.length() >= 8)
        {
            // bitset直接把二进制字符串转成数字
            string nnn(code_str.begin(), code_str.begin() + 8);
            bitset<8> aaa(nnn);
            char mmm = (char)aaa.to_ulong();
            
            out_file.write((char *)&mmm, sizeof(char));
            code_str.erase(code_str.begin(), code_str.begin() + 8);
        }
        // 继续读取文件
        in_file.read((char *)&temp, sizeof(char));
    }
    int length = code_str.length(); //12 秒
                                    // //如果压缩的字节不足8位,则用0补足
    for (int i = 0; i < 8 - length; i++)
    {
        code_str += '0';
    }
    bitset<8> aaa(code_str);
    char mmm = (char)aaa.to_ulong();
    out_file.write((char *)&mmm, sizeof(char));
    // if (bytenum > 0 && bytenum < 8)
    // {   // 73 秒
    //     ch <<= (8 - bytenum);
    // 	out_file.write((char*)&ch, sizeof(char));
    // }


    auto t8 = chrono::system_clock::now(); // 12s
    cout << "compress time= " << chrono::duration_cast<chrono::seconds>(t8-t7).count() << endl;
    in_file.close();
    out_file.close();
}
void Huffman::decompress_to_file()
{
    int temp, num, i, totalcount;
    unsigned char ch;
    in_file.open(out_file_name, ios::out | ios::binary);
    out_file.open(in_file_name, ios::in | ios::binary);

    unsigned int decodingnum = 0;
    for (int i = 0; i < 4; i++)
    {
        decodingnum <<= 8;
        out_file.read((char *)&ch, sizeof(char));
        decodingnum = decodingnum | int(ch);
    }

    //表头所记录的不同类型的字符数目
    num = decodingnum;
    while (num)
    {
        out_file.read((char *)&ch, sizeof(char));
        temp = (int)ch;

        // 读出频数
        //unsigned int a1, a2, a3, a4;
        unsigned int freq_num = 0;
        for (int i = 0; i < 4; i++)
        {
            freq_num <<= 8;
            out_file.read((char *)&ch, sizeof(char));
            freq_num = freq_num | int(ch);
        }

        huffnode[temp] = freq_num;

        num--;
    }
    // 得到huffman树
    Node *root = create_huffman_tree();

    create_huffman_code(root);
    // unsigned int size = huffnode.size();

    Node *tmp = root;
    totalcount = root->num;
    string code_str = "";
    while (!out_file.eof())
    {
        out_file.read((char *)&ch, sizeof(char));
        /*TODO: 反转字典值来做

        
        bitset<8> aaa((int)ch);
        code_str += aaa.to_string();
        int length = code_str.length();
        string tmp = "";
        while(length > 0)
        {
            string bbb(code_str.begin(), code_str.begin()+1);
            tmp += bbb;
            string key = aaa.to_string();
            if(reverse_code.count(key) > 0)
            {
                
                in_file << static_cast<char> (reverse_code[key]);
            }
        }
        string key = aaa.to_string();
        if(reverse_code.count(key) > 0)
        {
            
            in_file << static_cast<char> (reverse_code[key]);
        }
        */
        for (i = 7; i >= 0; i--)
        {
            if (root->left == nullptr && root->right == nullptr)
            {
                in_file << static_cast<char>(root->ch);
                root = tmp;
                totalcount--;
                if (totalcount == 1)        // 写入的时候并没有把最后一个结束写上？？
                                            // 还是编译器的问题？？？？
                {
                    return;
                }
            }
            temp = (ch >> i) & 1;
            if (temp == 0)
            {
                root = root->left;
            }
            else
            {
                root = root->right;
            }
        }
    }
    out_file.close();
    in_file.close();
}
