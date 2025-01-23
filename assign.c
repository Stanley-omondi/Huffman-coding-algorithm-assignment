#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    int frequency;
    struct Node *left, *right;
} Node;

typedef struct MinHeap {
    int size;
    int capacity;
    Node **array;
} MinHeap;

// Function to create a new node
Node *createNode(char character, int frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to create a min heap
MinHeap *createMinHeap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (Node **)malloc(capacity * sizeof(Node *));
    return heap;
}

// Swap two nodes in the heap
void swapNodes(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify the heap
void minHeapify(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->array[left]->frequency < heap->array[smallest]->frequency)
        smallest = left;

    if (right < heap->size && heap->array[right]->frequency < heap->array[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        swapNodes(&heap->array[smallest], &heap->array[index]);
        minHeapify(heap, smallest);
    }
}

// Extract the node with the smallest frequency
Node *extractMin(MinHeap *heap) {
    Node *minNode = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return minNode;
}

// Insert a new node into the heap
void insertMinHeap(MinHeap *heap, Node *node) {
    int i = heap->size++;
    while (i && node->frequency < heap->array[(i - 1) / 2]->frequency) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}


void buildMinHeap(MinHeap *heap) {
    for (int i = (heap->size - 2) / 2; i >= 0; --i)
        minHeapify(heap, i);
}

// Check if a node is a leaf
int isLeaf(Node *node) {
    return !(node->left || node->right);
}

// Print the Huffman codes (recursive traversal)
void printCodes(Node *root, char *code, int top) {
    if (root->left) {
        code[top] = '0';
        printCodes(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = '1';
        printCodes(root->right, code, top + 1);
    }

    if (isLeaf(root)) {
        code[top] = '\0';
        printf("%c: %s\n", root->character, code);
    }
}

// Calculate the compressed size of the text
void calculateCompressedSize(Node *root, int *compressedSize, int depth) {
    if (root->left) calculateCompressedSize(root->left, compressedSize, depth + 1);
    if (root->right) calculateCompressedSize(root->right, compressedSize, depth + 1);
    if (isLeaf(root)) *compressedSize += root->frequency * depth;
}

// Main function to generate Huffman codes
void generateHuffmanCodes(char *text) {
    
    int frequency[256] = {0};
    for (int i = 0; text[i] != '\0'; i++) {
        frequency[(unsigned char)text[i]]++;
    }

    
    MinHeap *heap = createMinHeap(256);
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            insertMinHeap(heap, createNode((char)i, frequency[i]));
        }
    }

    buildMinHeap(heap);

    // Build the Huffman tree
    while (heap->size > 1) {
        Node *left = extractMin(heap);
        Node *right = extractMin(heap);

        Node *parent = createNode('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        insertMinHeap(heap, parent);
    }

    Node *root = extractMin(heap);

    // Generate and print the Huffman codes
    char code[256];
    printf("Huffman Codes:\n");
    printCodes(root, code, 0);

  
    int uncompressedSize = strlen(text) * 8;
    int compressedSize = 0;
    calculateCompressedSize(root, &compressedSize, 0);

    printf("\nOriginal size: %d bits\n", uncompressedSize);
    printf("Compressed size: %d bits\n", compressedSize);
}

int main() {
    char text[1024];

    printf("Enter text to compress: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; 

    generateHuffmanCodes(text);

    return 0;
}
