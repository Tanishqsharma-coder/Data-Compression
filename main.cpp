// C++ program to encode and decode a string using
// Huffman Coding.
#include <bits/stdc++.h>
#include<string.h>
#define MAX_TREE_HT 256
using namespace std;

// to map each character its huffman value
map<char, string> codes;

// to store the frequency of character of the input data
map<char, int> freq;

// A Huffman tree node
struct MinHeapNode
{
    char data;             // One of the input characters
    int freq;             // Frequency of the character
    MinHeapNode *left, *right; // Left and right child

    MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// utility function for the priority queue
struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

// utility function to print characters along with
// there huffman value
void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// utility function to store characters along with
// there huffman value in a hash table, here we
// have C++ STL map
void storeCodes(struct MinHeapNode* root, string str)
{
    if (root==NULL)
        return;
    if (root->data != '$')
        codes[root->data]=str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

// function to build the Huffman tree and store it
// in minHeap
void HuffmanCodes(int size)
{
    struct MinHeapNode *left, *right, *top;
    for (map<char, int>::iterator v=freq.begin(); v!=freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
    while (minHeap.size() != 1)
    {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its
// frequency in input string
void calcFreq(string str, int n)
{
    for (int i=0; i<str.size(); i++)
        freq[str[i]]++;
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct MinHeapNode* root, string s)
{
    string ans = "";
    struct MinHeapNode* curr = root;
    for (int i=0;i<s.size();i++)
    {
        if (s[i] == '0')
           curr = curr->left;
        else
           curr = curr->right;

        // reached leaf node
        if (curr->left==NULL and curr->right==NULL)
        {
            ans += curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl;
    return ans+'\0';
}

// Driver program to test above functions
int main()
{
    string str;
    getline(cin,str);
      //cin.getline(str);
    //getline(str);
    string encodedString, decodedString;
    calcFreq(str, str.length());
    HuffmanCodes(str.length());

    cout << "Character With there Frequencies:\n";
    for (auto v=codes.begin(); v!=codes.end(); v++)
        cout << v->first <<' ' << v->second << endl;


    for (auto i: str)
        encodedString+=codes[i];

    cout << "\nEncoded Huffman data:\n" << encodedString << endl;

    decodedString = decode_file(minHeap.top(), encodedString);
    cout << "\nDecoded Huffman Data:\n" << decodedString << endl;
    return 0;
}
[15:46, 15/12/2021] +91 88538 48902: huffman coding
[15:49, 15/12/2021] Tanishq: Google me project to bahut mil rhe hai file compression oe
[15:49, 15/12/2021] Tanishq: Pura source code ke saath
[15:49, 15/12/2021] Tanishq: Sab try kro unko run krne ki
[15:49, 15/12/2021] Sanskar: #include <bits/stdc++.h>
using namespace std;
vector<int> encoding(string s1)
{
	cout << "Encoded output\n";
	unordered_map<string, int> dictionary;
	for (int i = 0; i <= 255; i++) {
		string temp = "";
		temp += char(i);
		dictionary[temp] = i;
	}

	string current_str = "", read = "";
	current_str += s1[0];
	int code = 256;
	vector<int> output_code;
	cout << "String\tOutput_Code\tDictionary update\n";
	for (int i = 0; i < s1.length(); i++)
        {
		if (i != s1.length() - 1)
			read += s1[i + 1];
		if (dictionary.find(current_str + read) != dictionary.end()) {
			current_str = current_str + read;
		}
		else {
			cout << current_str << "\t" << dictionary[current_str] << "\t\t"
				<< current_str + read << "\t" << code << endl;
			output_code.push_back(dictionary[current_str]);
			dictionary[current_str + read] = code;
			code++;
			current_str = read;
		}
		read = "";
	}
	cout << current_str << "\t" << dictionary[current_str] << endl;
	output_code.push_back(dictionary[current_str]);
	return output_code;
}

void decoding(vector<int> op)
{
	cout << "\nDecoded output\n";
	unordered_map<int, string> dictionary;
	for (int i = 0; i <= 255; i++) {
		string temp = "";
		temp += char(i);
		dictionary[i] = temp;
	}
	int old = op[0], n;
	string current_str = dictionary[old];
	string read = "";
	read += current_str[0];
	cout << current_str;
	int count = 256;
	for (int i = 0; i < op.size() - 1; i++) {
		n = op[i + 1];
		if (dictionary.find(n) == dictionary.end()) {
			current_str = dictionary[old];
			current_str = current_str + read;
		}
		else {
			current_str = dictionary[n];
		}
		cout << current_str;
		read = "";
		read += current_str[0];
		dictionary[count] = dictionary[old] + read;
		count++;
		old = n;
	}
}
int main()
{

    string s;
    getline(cin,s);
	vector<int> output_code = encoding(s);
	cout << "Output Codes are: ";
	for (int i = 0; i < output_code.size(); i++) {
		cout << output_code[i] << " ";
	}
	cout << endl;
	decoding(output_code);
}
