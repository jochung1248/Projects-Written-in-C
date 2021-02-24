
// COMP2521 Assignment1 T3
// z5308483

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "invertedIndex.h"

#define MAXCHAR 101

InvertedIndexBST InvertedIndexBSTNew(void);
InvertedIndexBST newWord(char *word);
FileList fileList(char *filename, double tf);
TfIdfList tfidfFileList(char *name, double tf);
InvertedIndexBST Insert(char *name, char *word, InvertedIndexBST root);
TfIdfList insertTfIdfFile(TfIdfList tfidftree, TfIdfList new);
InvertedIndexBST insertFile(InvertedIndexBST tree, FileList node);
void showFile(FILE *f, InvertedIndexBST tree);
double tf(char *filename, char *term);
void sort(TfIdfList tfidflist);




// Make a new empty BST (from lab03)
InvertedIndexBST InvertedIndexBSTNew(void) { 
    return NULL;
}


// This function makes a newWord node and points to the word (from lab03)
InvertedIndexBST newWord(char *word) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    new->word = malloc(sizeof(char)* MAXCHAR);
    strcpy(new->word, word);
    new->fileList = NULL;
    new->left = NULL;
    new->right = NULL;

    return new;
}

// This function makes the fileList for each word node
FileList fileList(char *name, double tf) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = malloc (sizeof(char)* MAXCHAR);
    strcpy(new->filename, name);
    new->tf = tf;
    new->next = NULL;

    return new;
}

// This function makes the tfidfFileList for each node
TfIdfList tfidfFileList(char *name, double tfidf) {
    TfIdfList new = malloc(sizeof(struct FileListNode));
    new->filename = malloc (sizeof(char)* MAXCHAR);
    strcpy(new->filename, name);
    new->tfIdfSum = tfidf;
    new->next = NULL;

    return new;
}

// This function inserts the nodes into the BST in alphabetical order
InvertedIndexBST Insert(char *name, char *word, InvertedIndexBST root) {
    // if the BST is empty, than create a new node for the word and a corresponding fileList
    if (root == NULL) {
        root = newWord(word);
        FileList new = fileList(name, tf(name, word));

        return insertFile(root, new);
    // if the word is alphabetically before the word in the previous node, insert into left node
    } else if (strcmp(word, root->word) < 0) {
        root->left = Insert(name, word, root->left);
    // if the word is alphabetically after the word in the previous node, insert into right node    
    } else if (strcmp(word, root->word) > 0) {
        root->right = Insert(name, word, root->right);
    // if the word already exists in the tree, only add filename and tf to the fileList
    } else {
        FileList new = fileList(name, tf(name, word));
        return insertFile(root, new);
    }

    return root;
}

// This function calculates the tf for a give word
double tf(char *filename, char *checkword) {
    double tf;
    int frequency = 0;

    FILE *fp = fopen(filename, "r");

    char word[MAXCHAR];
    int num = 0;
    while (fscanf(fp, "%s", word) != EOF) {
        normaliseWord(word);

        if (strcmp(word, checkword) == 0) {
            frequency++;
        }    
        num++;
    }
    tf = frequency /(double)num;

    fclose(fp);
    return (tf);
}

// This function inserts fileList node into the BST
InvertedIndexBST insertFile(InvertedIndexBST tree, FileList file) {

    if (tree->fileList == NULL) {
        tree->fileList = file;
        return tree;
    }

    FileList prev = tree->fileList;
    FileList curr = tree->fileList;
    // if the filename is alphabetically before the first filename, move it to first
    if (strcmp(file->filename, tree->fileList->filename) < 0) { 
        file->next = tree->fileList;
        tree->fileList = file;
        return tree;
    }
    // iterate through the list
    while (curr != NULL) {
        // if filename is alphabetically before the previous filename, move it before the previous
        if (strcmp(file->filename, curr->filename) < 0) {
            prev->next = file;
            file->next = curr;
            return tree;
        // if filename is the same then don't add anything    
        } else if (strcmp(file->filename, curr->filename) == 0) {
            return tree;
        }
        prev = curr;
        curr = curr->next;
    }
    prev->next = file;
    file->next = NULL;

    return tree;
}

// Normalising a given string
char *normaliseWord(char *str) {

    int i = 0;
    while (str[i] != '\0') {  
        // if there's a punctuation mark at the end of the string, remove it    
        if ((str[i] == '.' || str[i] == ',' || 
        str[i] == ';' || str[i] == '?') && str[i+1] == '\0') {
            str[i] = 0;
        }

        str[i] = tolower(str[i]);              
        i++;
    }
    return str;
}

// generates the invertedindex
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {

    InvertedIndexBST tree = InvertedIndexBSTNew();

    // open collection file
    FILE *fpr = fopen(collectionFilename, "r");
    if (fpr == NULL) {
        return 0;
    }

    // scan in filenames
    char name[MAXCHAR];
    while (fscanf(fpr, "%s", name) != EOF) {
        // open each file
        FILE *file = fopen(name, "r");
        // if the file is NULL then return;
        if (file == NULL) {
            return 0;
        }
        // scan in strings, normalise each word and then insert each into BST
        char word[MAXCHAR];
        while (fscanf(file, "%s", word) != EOF) {
            normaliseWord(word);
            tree = Insert(name, word, tree);
        }

        fclose(file);
    }

    fclose(fpr);

    return tree;
}

// prints the invertedindex into file
void printInvertedIndex(InvertedIndexBST tree) {
    // Create or overwrite the file invertedIndex.txt
    FILE *text = fopen("invertedIndex.txt", "w");

    // If tree is empty, don't print anything and return
    if (tree == NULL) {
        return;
    }

    showFile(text, tree);

    fclose(text);
} 

// This function prints the contents within the tree into the file
void showFile(FILE *f, InvertedIndexBST tree) {
    // base case -> if the tree is empty, return
    if (tree == NULL) {
        return;
    }
    // go onto left node
    showFile(f,tree->left);

    // print the word into the file  
    fprintf(f, "%s ", tree->word);
    FileList curr = tree->fileList;
    // print the filenames and tf values in order after the word
    while (curr != NULL) {
        
        fprintf(f, "%s (%lf) ", curr->filename, curr->tf);
        curr = curr->next;
    }
    fprintf(f, "\n");

    // go onto right node
    showFile(f, tree->right);
}

// This function searches through the tree to find the node containing the searchWord
InvertedIndexBST search(InvertedIndexBST tree, char *searchWord) {

    if (tree == NULL) {
        return tree;
    }
    if (strcmp(tree->word, searchWord) == 0) {
        return tree;
    } else if (strcmp(tree->word, searchWord) > 0) {
        return search(tree->left, searchWord);    
    } else if (strcmp(tree->word, searchWord) < 0) {
        return search(tree->right, searchWord);
    } 
    return tree;
}

// Calculates the tfidf value
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {

    TfIdfList new = NULL;
    TfIdfList tfidflist = NULL;

    // Set pointer to the fileList of the node containing search word
    FileList curr = search(tree, searchWord)->fileList;

    if (curr == NULL) {
        return NULL;
    }
    // Calculate how many files there are in the list
    int count = 0;
    while (curr != NULL) {
        count = count + 1;
        curr = curr->next;
    }
    
    curr = search(tree, searchWord)->fileList;
    // Calculate idf value
    double idf = log10((double)D/(double)count);

    // Calculate tfidf value and then add tfidfFileList node and then add node into List
    while (curr != NULL) {
        double tfidf = curr->tf * idf;
        new = tfidfFileList(curr->filename, tfidf);
        tfidflist = insertTfIdfFile(tfidflist, new);
        curr = curr->next;
    }    

    return tfidflist;

}  

// This functions inserts files into the list in tfidf descending and alphabetical order
TfIdfList insertTfIdfFile(TfIdfList tfidflist, TfIdfList new) {
    // if the tree is null add node as root
    if (tfidflist == NULL) {
        tfidflist = new;
        return tfidflist;
    }
    // if the tfidf value of the current node is greater than the root, put is as first
    if (tfidflist->tfIdfSum < new->tfIdfSum) { 
        new->next = tfidflist;
        tfidflist = new;
        return tfidflist;
    }

    TfIdfList prev = tfidflist;
    TfIdfList curr = tfidflist;
    
    while (curr != NULL) { 
        // If the filenames are the same, add it to the tfidfSum
        if (strcmp(new->filename, curr->filename) == 0) {
            curr->tfIdfSum += new->tfIdfSum;
            return tfidflist;
        }
        curr = curr->next;
    }
    curr = tfidflist;
    while (curr != NULL) {
        // if the tfidf value at the current node is greater than the previous node, put it before
        if (curr->tfIdfSum < new->tfIdfSum) {
            prev->next = new;
            new->next = curr;
            return tfidflist;
        } 
        // if the tfidf value is the same, order alphabetically
        if (new->tfIdfSum == curr->tfIdfSum) { 
            if (strcmp(new->filename, curr->filename) < 0) { 
                curr->next = new;
                new->next = curr;
                return tfidflist;
            } else {
                new->next = curr->next;
                curr->next = new;
                return tfidflist;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    prev->next = new;
    return tfidflist;
}

// This function is similar to calculatetfidf function
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {

    TfIdfList new = NULL;
    TfIdfList tfidflist = NULL;

    // Loop through all the searchWords
    int numsearch = 0;
    while (searchWords[numsearch] != NULL) {
        // Set curr pointer to filelist contained by node of searchWord
        FileList curr = search(tree, searchWords[numsearch])->fileList;
    
        // Calculate number of files in list
        int count = 0;
        while (curr != NULL) {
            count = count + 1;
            curr = curr->next;
        }

        curr = search(tree, searchWords[numsearch])->fileList;

        // Calculate idf value
        double idf = log10((double)D/(double)count);
        
        //Calculate tfidf value, then create a node and then add node into list
        while (curr != NULL) {
            double tfidf = curr->tf * idf;
            new = tfidfFileList(curr->filename, tfidf);
            tfidflist = insertTfIdfFile(tfidflist, new);
            curr = curr->next;
        }  
        numsearch++;
    }
    // sort the list
    sort(tfidflist);
    return tfidflist;
}


// This function sorts the list
void sort(TfIdfList list) {
    TfIdfList curr = list;
    TfIdfList curr2 = list;
    TfIdfList temp = tfidfFileList("", 0);
    while (curr != NULL) {
        while (curr2->next != NULL) {
            if (curr2->tfIdfSum < curr2->next->tfIdfSum) {
                temp->filename = curr2->next->filename;
                temp->tfIdfSum = curr2->next->tfIdfSum;
                curr2->next->filename = curr2->filename;
                curr2->next->tfIdfSum = curr2->tfIdfSum;
                curr2->filename = temp->filename;
                curr2->tfIdfSum = temp->tfIdfSum;
            }
            curr2 = curr2->next;
        }
        curr = curr->next;
    }
}
