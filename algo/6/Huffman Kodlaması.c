// Huffman C. TreeNodes

#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_HT 100

struct MinHeapNode {
//girilen karakter, a b c d e f
  char veri;
//karakterin frekansi,  45 13 12 16 9 5
  unsigned frekans;

//cizilecek agac, dugum, sag sol 
  struct MinHeapNode *left, *right;
};

//huffman agac size/alabilirlik
struct MinHeap {
//min heap boyutu-size 
  unsigned size;
//min heap alabilirlik/kapasitesi
  unsigned kapasite;
  
  struct MinHeapNode **array;
};

//yeni bir yardımcı node- 
struct MinHeapNode *newNode(char veri, unsigned frekans) {
  struct MinHeapNode *temp = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));

  temp->left = temp->right = NULL;
  temp->veri = veri;
  temp->frekans = frekans;

  return temp;
}

// kapasite min yigin/heap - funct yaratma  
struct MinHeap *createMinH(unsigned kapasite) {
  struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

  minHeap->size = 0;
  minHeap->kapasite = kapasite;
  minHeap->array = (struct MinHeapNode **)malloc(minHeap->kapasite * sizeof(struct MinHeapNode *));

  return minHeap;
}

// iki yigin/heap swap islemi - funct yaratma
void swapMinHNode(struct MinHNode **a, struct MinHNode **b) {
  struct MinHNode *t = *a;
  *a = *b;
  *b = t;
}
//---------
// Heapify islemi- bit.ly/3mUUOPt
void minHeapify(struct MinHeap *minHeap, int idx) {

  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size &&
      minHeap->array[left]->frekans < minHeap->array[smallest]->frekans)smallest = left;

  if (right < minHeap->size && 
      minHeap->array[right]->frekans < minHeap->array[smallest]->frekans)smallest = right;

  if (smallest != idx) {
      swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
      minHeapify(minHeap, smallest);
  }
}

// Boyutunun kontrolu yapilir- function kontrol
int checkSizeOne(struct MinHeap *minHeap) {
  return (minHeap->size == 1);
}

// min deger - cikarim standard function
struct MinHeapNode *extractMin(struct MinHeap *minHeap) {

  struct MinHeapNode *temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

// Eklemek icin yardimci function - min heap icin yeni bir dugum olsm
void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *minHeapNode) {
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->frekans < minHeap->array[(i - 1) / 2]->frekans) {
    minHeap->array[i] = minHeap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
  int n = minHeap->size - 1;
  int i;

  for (i = (n - 1) / 2; i >= 0; --i)
    minHeapify(minHeap, i);
}

// bu dugumde ball-leaf  olup olmadini kontrol amacli yardimci fonksiyon
int isLeaf(struct MinHeapNode *root) {
  return !(root->left) && !(root->right);
}

// min kapasite ile yigin olusturarak boyutları esitler ardından karakterlere ekleme yapar
// bunu yeni olusturulan minKapasiteye eşitler

struct MinHeap *createAndBuildMinHeap(char veri[], int frekans[], int size) {
  struct MinHeap *minHeap = createMinH(size);

  for (int i = 0; i < size; ++i)
    minHeap->array[i] = newNode(veri[i], frekans[i]);

  minHeap->size = size;
  buildMinHeap(minHeap);

  return minHeap;
}

//Asil islemin oldugu yerdir 
// yani Huffman agacinini olusturan ana islevin oldugu kisim burada yerlesim ve esitlemeler yapilir.

struct MinHeapNode *buildHuffmanTree(char veri[], int frekans[], int size) 
{
  struct MinHeapNode *left, *right, *top;
//esitleme islemi -boyut
  struct MinHeap *minHeap = createAndBuildMinHeap(veri, frekans, size);

//heap/yigin boyutunun --bit.ly/32rR8gT
// frekansa eşit olan dügüm
    //-->toplam frekans belirle
    //-->sag sol ayırlır, buradaki $ dügümler icin degildir (ozel olarak degil)
    //-kullanılabilir degildir

  while (!checkSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    top = newNode('$', left->frekans + right->frekans);

    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  return extractMin(minHeap);
}
// yazilan kökleri yazdirir ve saklamak icin arr kullanilir
void printHCodes(struct MinHeapNode *root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1);
  }
  if (root->right) {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1);
  }
  if (isLeaf(root)) {
    printf("  %c   | ", root->veri);
    printArray(arr, top);
  }
}

//Huffman olusturan genel main function
//agaci gecis yaparak yazdirme islemilerini yapar

void HuffmanCodes(char veri[], int frekans[], int size) {
  struct MinHeapNode *root = buildHuffmanTree(veri, frekans, size);

  int arr[MAX_TREE_HT], top = 0;

  printHCodes(root, arr, top);
}

// n boyutunda bir dizi yazdirma islemi icin function
void printArray(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i)
    printf("%d", arr[i]);

  printf("\n");
}
// -->bit.ly/30YDEsb
int main() {
  char arr[] = {'A', 'B', 'C', 'D', 'E', 'F'};
  int frekans[] = {40, 13, 10, 16, 9, 5};

  int size = sizeof(arr) / sizeof(arr[0]);

  printf(" Karakter | Huffman Kod ");
  printf("\n--------------------\n");

  HuffmanCodes(arr, frekans, size);
}