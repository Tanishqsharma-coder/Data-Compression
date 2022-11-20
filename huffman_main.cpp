#include <bits/stdc++.h>
using namespace std;

struct rotation {
	int index;
	char* suffix;
};

struct node {
	int data;
	struct node* next;
};

// Compares the characters of bwt_arr[]
// and sorts them alphabetically
int cmpfunc2(const void* a, const void* b)
{
	const char* ia = (const char*)a;
	const char* ib = (const char*)b;
	return strcmp(ia, ib);
}

// Creates the new node
struct node* getNode(int i)
{
	struct node* nn =
		(struct node*)malloc(sizeof(struct node));
	nn->data = i;
	nn->next = NULL;
	return nn;
}

// Does insertion at end in the linked list
void addAtLast(struct node** head, struct node* nn)
{
	if (*head == NULL) {
		*head = nn;
		return;
	}
	struct node* temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = nn;
}

// Computes l_shift[]
void* computeLShift(struct node** head, int index,	int* l_shift)
{
	l_shift[index] = (*head)->data;
	(*head) = (*head)->next;
}

string invert(char bwt_arr[])
{
	int i,len_bwt = strlen(bwt_arr);
	char* sorted_bwt = (char*)malloc(len_bwt * sizeof(char));
	strcpy(sorted_bwt, bwt_arr);
	int* l_shift = (int*)malloc(len_bwt * sizeof(int));

	// Index at which original string appears
	// in the sorted rotations list
	int x = 0;
	for(int i=0 ; i<strlen(bwt_arr) ;i++)
    {

        if(bwt_arr[i]=='#')
            break;

        x++;
    }

	// Sorts the characters of bwt_arr[] alphabetically
	qsort(sorted_bwt, len_bwt, sizeof(char), cmpfunc2);

	// Array of pointers that act as head nodes
	// to linked lists created to compute l_shift[]
	struct node* arr[128] = { NULL };

	// Takes each distinct character of bwt_arr[] as head
	// of a linked list and appends to it the new node
	// whose data part contains index at which
	// character occurs in bwt_arr[]
	for (i = 0; i < len_bwt; i++) {
		struct node* nn = getNode(i);
		addAtLast(&arr[bwt_arr[i]], nn);
	}

	// Takes each distinct character of sorted_arr[] as head
	// of a linked list and finds l_shift[]
	for (i = 0; i < len_bwt; i++)
		computeLShift(&arr[sorted_bwt[i]], i, l_shift);

	// Decodes the bwt
	string str="";
	for (i = 0; i < len_bwt; i++) {
		x = l_shift[x];
		str = str + bwt_arr[x];
		//cout << bwt_arr[x];
	}

	return str;
}


// Compares the rotations and
// sorts the rotations alphabetically
int cmpfunc(const void* x, const void* y)
{
	struct rotation* rx = (struct rotation*)x;
	struct rotation* ry = (struct rotation*)y;
	return strcmp(rx->suffix, ry->suffix);
}

// Takes text to be transformed and its length as
// arguments and returns the corresponding suffix array
int* computeSuffixArray(char* input_text, int len_text)
{
	// Array of structures to store rotations and
	// their indexes
	struct rotation suff[len_text];

	// Structure is needed to maintain old indexes of
	// rotations after sorting them
	for (int i = 0; i < len_text; i++) {
		suff[i].index = i;
		suff[i].suffix = (input_text + i);
	}

	// Sorts rotations using comparison
	// function defined above
	qsort(suff, len_text, sizeof(struct rotation),cmpfunc);

	// Stores the indexes of sorted rotations
	int* suffix_arr
		= (int*)malloc(len_text * sizeof(int));
	for (int i = 0; i < len_text; i++)
		suffix_arr[i] = suff[i].index;

	// Returns the computed suffix array
	return suffix_arr;
}

// Takes suffix array and its size
// as arguments and returns the
// Burrows - Wheeler Transform of given text
char* findLastChar(char* input_text,
				int* suffix_arr, int n)
{
	// Iterates over the suffix array to find
	// the last char of each cyclic rotation
	char* bwt_arr = (char*)malloc(n * sizeof(char));
	int i;
	for (i = 0; i < n; i++) {
		// Computes the last char which is given by
		// input_text[(suffix_arr[i] + n - 1) % n]
		int j = suffix_arr[i] - 1;
		if (j < 0)
			j = j + n;

		bwt_arr[i] = input_text[j];
	}

	bwt_arr[i] = '\0';

	// Returns the computed Burrows - Wheeler Transform
	return bwt_arr;
}

struct Tree
{
    int frequency;
    unsigned char character;
    Tree *left = NULL;
    Tree *right = NULL;
};

class TreeComparator
{
  public:
    bool operator()(Tree *a, Tree *b)
    {
        return a->frequency > b->frequency;
    }
};

Tree *buildHuffmanTree(vector<pair<unsigned char, int> > freqtable)
{

    priority_queue<Tree *, vector<Tree *>, TreeComparator> huffqueue;
    for (int i = 0; i < freqtable.size(); i++)
    {
        Tree *node = new Tree();
        node->frequency = freqtable[i].second;
        node->character = freqtable[i].first;

        huffqueue.push(node);
    }



    while (huffqueue.size() > 1)
    {
        Tree *a, *b;
        a = huffqueue.top();
        huffqueue.pop();

        b = huffqueue.top();
        huffqueue.pop();
        Tree *c = new Tree();
        c->frequency = a->frequency + b->frequency;
        c->left = a;
        c->right = b;
        huffqueue.push(c);
    }

    return huffqueue.top();
}

string toBinary(unsigned  char a)
{
    string output  = "";
    while(a!=0)
    {
        string bit = a%2==0?"0":"1";
        output+=bit;
        a/=2;
    }

    if(output.size()<8)
    {
        int deficit = 8 - output.size();
        for(int i=0; i<deficit; i++)
        {
            output+="0";
        }
    }

    reverse(output.begin(), output.end());
    return output;

}

void traverseHuffmanTree(Tree *root, string prev, string toAppend, map<unsigned char, string> &codemap)
{

    prev+=toAppend;

    if (root->right == NULL && root->left == NULL)
    {
        // cout<<root->character<<" "<<prev<<endl;
        codemap[root->character] = prev;
    }
    if (root->right != NULL)
    {
        traverseHuffmanTree(root->right, prev, "1", codemap);
    }

    if (root->left != NULL)
    {
        traverseHuffmanTree(root->left, prev, "0", codemap);
    }
}

unsigned char *readFileIntoBuffer(char *path, int &sz)
{
    FILE *fp = fopen(path, "rb");
    sz = 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(sz);
    fread(buffer, 1, sz, fp);
    return buffer;
}

void writeFileFromBuffer(char *path, unsigned char *buffer, int sz, int flag)
{
    FILE *fp;
    if(flag==0)
    {
        fp = fopen(path, "wb");
    }

    else{
        fp = fopen(path, "ab");
    }

    fwrite(buffer, 1, sz, fp);

    fclose(fp);
}


vector<pair<unsigned char, int> > convertToVector(map<unsigned char, int> codes)
{
    vector<pair<unsigned char, int> > codesV;

    for (map<unsigned char, int>::iterator i = codes.begin(); i != codes.end(); i++)
    {
        codesV.push_back(make_pair(i->first, i->second));
    }

    return codesV;
}

string getHuffmanBitstring(unsigned char *buffer, map<unsigned char, string> codes, int sz, int& paddedBits)
{
    string outputBuffer="";
    for(int i=0; i<sz; i++)
    {
        outputBuffer=outputBuffer+codes[buffer[i]];
    }

    if(outputBuffer.size()%8!=0)
    {
        int deficit = 8*((outputBuffer.size()/8)+1)-outputBuffer.size();
        paddedBits = deficit;
        for(int i=0; i<deficit; i++)
        {
            outputBuffer+="0";
        }
    }

    //Instead of adding zeroes to the end, add zero to the begining of the last byte

    return outputBuffer;

}

unsigned char* getBufferFromString(string bitstring, vector<unsigned char>&outputBuffer, int& sz)
{
    int interval = 0;
    unsigned char bit = 0;

    for(int i=0; i<sz; i++)
    {
         bit = (bit<<1)|(bitstring[i]-'0');

        interval++;
        if(interval==8)
        {
            interval = 0;
            outputBuffer.push_back(bit);
            bit = 0;

        }
    }
    sz = outputBuffer.size();
    return outputBuffer.data();
}

string getStringFromBuffer(unsigned char* buffer, int sz)
{
    string bitstring = "";
    for(int i=0; i<sz; i++)
    {
        bitstring+=toBinary(buffer[i]);
    }

    return bitstring;
}

unsigned char* getDecodedBuffer(string bitstring, vector<unsigned char>&buffer, map<unsigned char, string> codes, int &sz, int paddedBits)
{
    string bit = "";
    map<string, unsigned char> reversecodes;

    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        reversecodes[i->second] = i->first;
    }

    for(int i=0; i<bitstring.size()-paddedBits; i++)
    {
        bit+=string(1, bitstring[i]);
        if(reversecodes.find(bit)!=reversecodes.end())
        {
           buffer.push_back(reversecodes[bit]);
           bit = "";
        }
    }

    sz = buffer.size();
    return buffer.data();
}


void writeHeader(char* path,map<unsigned char, string> codes,  int paddedBits){

    int size = codes.size();
    writeFileFromBuffer(path, (unsigned char*)&paddedBits, sizeof(int), 0);
    writeFileFromBuffer(path, (unsigned char*)&size, sizeof(int), 1);
    char nullBit = '\0';
    for(map<unsigned char, string>::iterator i = codes.begin(); i!=codes.end(); i++)
    {
        writeFileFromBuffer(path, (unsigned char*)&i->first, 1, 1);
        int len = i->second.size();
        writeFileFromBuffer(path, (unsigned char*)&len, sizeof(int), 1);
        writeFileFromBuffer(path, (unsigned char*)i->second.c_str(), i->second.size(), 1);
    }
}


unsigned char* readHeader(unsigned char* buffer, map<unsigned char, string> &codes, int& paddedBits, int &sz)
{
   paddedBits = *((int*)buffer);
   cout<<paddedBits<<"PADDED"<<endl;
   buffer = buffer+4;
   sz-=4;
   int size = *((int*)buffer);
   buffer = buffer+4;
   sz-=4;
   for(int i=0; i<size; i++)
   {
       unsigned char key = buffer[0];
       buffer++;
       sz--;
       int len = *((int*)buffer);
       buffer+=4;
       sz-=4;
       char* value = (char*)malloc(len+1);

       for(int j = 0; j<len; j++)
       {
           value[j]=buffer[j];
       }
    //    value = (char*)buffer;
       buffer+=len;
       sz-=len;
       value[len]='\0';
       codes[key] = value;
       cout<<key<<" "<<value<<endl;
   }

   return buffer;
}



//add amount padded
void compressFile(char *path, char *output_path, map<unsigned char, string> &codes)
{
    int sz = 0;
    int paddedBits = 0;
    map<unsigned char, int> freqtable;
    unsigned char *buffer = readFileIntoBuffer(path, sz);
    for (int i = 0; i < sz; i++)
    {
            freqtable[buffer[i]]++;
    }
    Tree *root = buildHuffmanTree(convertToVector(freqtable));
    cout<<root<<endl;
    traverseHuffmanTree(root, "", "", codes);
    string outputString = getHuffmanBitstring(buffer, codes, sz, paddedBits);
    sz  = outputString.size();
    vector<unsigned char> outputBufferV;
    getBufferFromString(outputString, outputBufferV, sz);
    unsigned char* outputBuffer = outputBufferV.data();
    writeHeader(output_path, codes, paddedBits);
    writeFileFromBuffer(output_path, outputBuffer, sz, 1);
}

void decompressFile( char* inputPath,  char* outputPath)
{
    int sz = 0;
    map<unsigned char, string> codes;
    int paddedBits = 0;
    unsigned char* fileBuffer = readFileIntoBuffer(inputPath, sz);
    fileBuffer = readHeader(fileBuffer, codes, paddedBits, sz);
    string fileBitString = getStringFromBuffer(fileBuffer, sz);
    // cout<<fileBitString<<endl;
    vector<unsigned char> outputBufferV;
    unsigned char* outputBuffer;
    getDecodedBuffer(fileBitString,outputBufferV, codes, sz, paddedBits);
    outputBuffer = outputBufferV.data();
    writeFileFromBuffer(outputPath, outputBuffer,sz, 0);
    //take care of appended zeroes
}

int main()
{
    //fp state?
    char* dEI = "test.txt";
    char* dEO = "test1.txt";
    char *dDO = "decoded.txt";
    map<unsigned char, string> codes;
    compressFile(dEI, dEO, codes);
    decompressFile(dEO, dDO);
}

//Bugs: Take care of last character
