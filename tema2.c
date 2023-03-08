#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//structura de tip arbore
typedef struct tree {
	char a[3][6];

    char true_false;

	struct tree *child;
    
    struct tree *next_node;
} tree;
void cerinta1();
void cerinta2();
void generate_tree(tree *copy_tree, char player1, char player2, char begginer[1], int liniute, FILE *fisier_out, char cerinta[3]);
int is_end_game();
char true_false();
void parcurgere_true_false();
void print_table();
void print_true_false();
void dealloc_tree();

int main(int argc, char **argv) {
    FILE *fisier_in, *fisier_out;
    fisier_in = fopen (argv[2], "r");
    fisier_out = fopen (argv[3], "w");

    if (strcmp(argv[1], "-c1") == 0)
      cerinta1 (fisier_in, fisier_out);
     if (strcmp(argv[1], "-c2") == 0)
      cerinta2 (fisier_in, fisier_out);

    fclose (fisier_in);
    fclose (fisier_out); 
    return 0;
}
void cerinta1 (FILE *fisier_in, FILE *fisier_out) {
    int liniute = 0, i ,j;
    char player1, player2, cerinta[3] = "-c1", begginer[1];

    player1 = fgetc(fisier_in);
    begginer[0] = player1;

    //alegem care este jucatorul curent si care este adversarul
    if (player1 == 'X') {
        player2 = 'O';
    } else {
        player2 = 'X';
        }
    //trecem peste \n
    fgetc(fisier_in);

   //initializare arbore- nodul radacina
    tree *root;
    root = (tree *)malloc (sizeof(tree));
    root->child = NULL;
    root->next_node = NULL;

    //citim din fisierul de intrare tabla initiala de joc
    for (i = 0; i < 3; i++) {
		for (j = 0; j < 6; j++) {
			root->a[i][j] = fgetc(fisier_in);
            if(root->a[i][j] == '-')
            liniute ++;
		}
	}
    print_table(root, liniute, fisier_out);
    generate_tree(root, player1, player2,begginer, liniute, fisier_out, cerinta);
    dealloc_tree(root);
}
void cerinta2 (FILE* fisier_in, FILE* fisier_out) {

    int liniute = 0, i ,j;
    char player1, player2, cerinta[3] = "-c2", begginer[1];
    player1 = fgetc(fisier_in);
    begginer[0] = player1;

     //alegem care este jucatorul curent si care este adversarul
    if (player1 == 'X') {
        player2 = 'O';
    } else {
        player2 = 'X';
        }
    //trecem peste new line
    fgetc(fisier_in);
 
    //initializare arbore- nodul radacina
    tree *root;
    root = (tree *)malloc (sizeof(tree));
    root->child = NULL;
    root->next_node = NULL;
    root->true_false = 0;
    
     //citim din fisierul de intrare tabla initiala de joc
    for (i = 0; i < 3; i++) {
		for (j = 0; j < 6; j++) {
			root->a[i][j] = fgetc(fisier_in);
            if (root->a[i][j] == '-')
            liniute ++;
		}
	}
    generate_tree(root, player1, player2,begginer, liniute, fisier_out, cerinta);
  
    root->true_false = true_false(root, 0);

    parcurgere_true_false(root, liniute, fisier_out);
    dealloc_tree(root);
}
void generate_tree(tree *Tree, char player1, char player2, char begginer[1], int liniute, FILE *fisier_out, char cerinta[3]) {
    int end_game = 0, i, j , l, c, nr_lines = 0;
    char winner[1];
    winner[0] = 0;
    char copy_table[3][6];
    
    //copiem matricea nodului curent Tree
    for (i = 0; i < 3; i++) {
		for (j = 0; j < 6; j++) {
			copy_table [i][j] = Tree->a[i][j];
		}
	}
    //numaram cate linii sunt pe tabla de joc
    for (i = 0; i < 3; i++) {
		for (j = 0; j < 6; j++) {
			if(Tree->a[i][j] == '-')
            nr_lines++;
		}
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 6; j++) {
             end_game = 0;
            if (copy_table[i][j] == '-') {

                   if (Tree->child == NULL) {

						//alocam memorie pentru nodul child
						Tree->child = (tree *)malloc(sizeof(tree));
                        Tree = Tree->child;
						Tree->child = NULL;
						Tree->next_node = NULL;
                        Tree->true_false = 0;
 
						//initializarea matricei a nodului child
						for (l = 0; l < 3; l++) {
                            for (c = 0; c < 6; c++) {
                                Tree->a[l][c] = copy_table[l][c];
                            }
	                    }
						//adaugarea valorii player-ului
						Tree->a[i][j] = player1;
                        
                        //verificare daca este castig
                        end_game = is_end_game(Tree, winner);
                        
                        //printam matricea nodului child
                        if (strcmp (cerinta, "-c1") == 0)
                        print_table (Tree, liniute, fisier_out);
                        
                        //determinam caracterele true_false pentru nodurile
                        //terminale
                         if (strcmp(cerinta, "-c2") == 0) {
                             if ((nr_lines - 1) == 0) {
                                 if (end_game == 1) {

                                     if (begginer[0] == winner[0])
                                      Tree->true_false = 'T';

                                      else Tree->true_false = 'F'; 
 
                                    }  else Tree->true_false = 'F';
                                } 
                                if (end_game == 1) {
                                   if (begginer[0] == winner[0])
                                    Tree->true_false = 'T';
                                    else Tree->true_false = 'F';

                                } else Tree->true_false = 'F';
                            }  
                        //daca nu avem castig apelam recursiv functia 
                        if (end_game == 0)
                        generate_tree(Tree, player2, player1,begginer, liniute, fisier_out, cerinta);
                        
                        //determinam caracterele true_false pentru nodurile neterminale
                        if (strcmp(cerinta, "-c2") == 0) {
                           if (end_game == 0 && (nr_lines - 1) != 0) {
                                 int ok = 0;
                                 if (Tree->child->next_node != NULL) {
                                     tree *copy_tree = Tree;
                                     int zeroes = 0;
                                     copy_tree = copy_tree->child;

                                     while (copy_tree) {
                                          if (copy_tree->true_false == 0) 
                                           zeroes++;
                                         copy_tree = copy_tree->next_node;
                                       }
                                     if (zeroes == 0) 
                                        ok = 1; 
                                    }    
                                 else {
                                     if (Tree->child->true_false  != 0) 
                                       ok = 1; 
                                    }
                                 if (ok == 1) 
                                    Tree->true_false = true_false(Tree, (liniute - nr_lines + 1));
                                
                                }
                       }

                    } else {
                        //initializarea nodului vecin nodului Tree
                        Tree->next_node = (tree *)malloc(sizeof(tree));
                        Tree = Tree->next_node;
						Tree->child = NULL;
						Tree->next_node = NULL;
                        Tree->true_false = 0;
                        
						//initializare matrice vecina
                        for (l = 0; l < 3; l++) {
                            for (c = 0; c < 6; c++) {
                                Tree->a[l][c] = copy_table[l][c];
                            }
	                    }
                        //mutam valoarea playerului 1 pe liniuta curenta
                        Tree->a[i][j] = player1;
                
                        //verificam daca avem sfarsit de joc
                        end_game = is_end_game(Tree,winner);
                        
                        //printam tabla de joc curenta
                        if (strcmp(cerinta, "-c1") == 0)
                        print_table(Tree, liniute, fisier_out);
                        
                        //determinam caracterele true_false pentru nodurile terminale
                        if (strcmp(cerinta, "-c2") == 0) {
                            if ((nr_lines - 1)== 0) {
                                if (end_game == 1) {
                                   if (begginer[0] == winner[0])
                                   Tree->true_false = 'T';
                                  else Tree->true_false = 'F';
                               }  
                               else Tree->true_false = 'F';
                            } else {
                                 if (end_game == 1) {
                                    if (begginer[0] == winner[0])
                                    Tree->true_false = 'T';
                                   else Tree->true_false = 'F';
                                } else Tree->true_false = 'F';
                              }
                        }
                        
                        //daca nu avem win apelam recursiv functia
                        if (end_game == 0)
                        generate_tree(Tree, player2, player1, begginer, liniute, fisier_out, cerinta);

                        //determinam caracterele true_false pentru nodurile neterminale
                        if (strcmp(cerinta, "-c2") == 0) {
                           if (end_game == 0 && (nr_lines - 1) != 0) {
                               int ok = 0;
                              if (Tree->child->next_node != NULL) {
                              tree *another_tree = Tree;
                              int zeroes = 0;
                              another_tree = another_tree->child;
                              while (another_tree) {
                                  if (another_tree->true_false == 0) 
                                   zeroes++;
                                 another_tree = another_tree->next_node;
                                }
                              if (zeroes == 0) {
                                  ok = 1;
                                }

                             } else {
                                  if (Tree->child->true_false  != 0) {
                                    ok = 1;
                                   } 
                                }
                                if (ok == 1) {
                                    Tree->true_false = true_false(Tree, (liniute - nr_lines + 1));
                                }
                           }
                       }

                    }
            }
        }
    }  
}
int is_end_game (tree *Tree, char winner[1]) {
    int l, c, end_game = 0;
    //verificam daca avem castig pe orizontala
    for (l = 0 ; l < 3; l++) {
        if ((Tree->a[l][0] == Tree->a[l][2]) && (Tree->a[l][2] == Tree->a[l][4]) && Tree->a[l][0] != '-') {
            end_game = 1;
            winner[0] = Tree->a[l][0];
        }
    }
    //verificam daca avem castig pe verticala
    for (c = 0; c < 6; c++ ) {
        if ((Tree->a[0][c] == Tree->a[1][c]) && (Tree->a[1][c] == Tree->a[2][c]) && Tree->a[0][c] != '-') {
           end_game = 1;
           winner[0] = Tree->a[0][c];
        }
        c++;            
    }
    //verificam daca avem castig pe diagonala principala
    if ((Tree->a[0][0] == Tree->a[1][2]) && (Tree->a[1][2] == Tree->a[2][4]) && Tree->a[0][0] != '-') {
        end_game = 1;
        winner[0] = Tree->a[0][0];
    }
    //verificam daca avem castig pe diagonala secundara
    if ((Tree->a[0][4] == Tree->a[1][2]) && (Tree->a[1][2] == Tree->a[2][0]) && Tree->a[0][4] != '-'){
        end_game = 1;
        winner[0] = Tree->a[2][0];
    }
    return end_game;
}
char true_false(tree *root, int SI_SAU) {
    int ok_si= 1, ok_sau = 0;
    tree* Tree = root->child;
       //facem operatia SI pentru pozitie impara
        if (SI_SAU % 2 == 1) {
         do {
             if(Tree->true_false  ==  'F')
                ok_si = 0;
             Tree = Tree->next_node;
           } while (Tree);

         if(ok_si == 0)
           root->true_false = 'F';
         else root->true_false = 'T';

        } // facem operatia SAU pentru pozitie para
         else {
                do {
                      if(Tree->true_false  ==  'T')
                       ok_sau = 1;
                     Tree = Tree->next_node;
                    } while (Tree);

                       if(ok_sau == 0)
                        root->true_false = 'F';
                       else root->true_false = 'T';
        }
        free(Tree);   
        return root->true_false;
}
void parcurgere_true_false (tree *root, int liniute, FILE *fisier_out) {
    print_true_false(root,liniute, fisier_out);
    if(root->child) {
        root = root->child;
        parcurgere_true_false(root, liniute, fisier_out);
    }
     while (root->next_node) {
        root = root->next_node;
        parcurgere_true_false(root, liniute, fisier_out);
    }
}
void print_table (tree * Tree, int liniute, FILE *fisier_out) {
    int nr_lines = 0, nr_tabs, i , j, k;
      for (i = 0; i < 3; i++) {
		  for (j = 0; j < 6; j++) {
			  if (Tree->a[i][j] == '-')
              nr_lines++;
		    }
        }
        nr_tabs = liniute - nr_lines;

     for (i = 0; i < 3; i++) {
		  for (k = 0; k < nr_tabs; k++)
            fputc('\t', fisier_out);
              for(j = 0; j < 6; j++) {
                  fputc(Tree->a[i][j], fisier_out);
             }
        }
       fputc('\n', fisier_out); 
}
void print_true_false(tree * Tree, int liniute, FILE *fisier_out) {
    int nr_lines = 0, nr_tabs, i , j, k;
      for (i = 0; i < 3; i++) {
		  for (j = 0; j < 6; j++) {
			  if (Tree->a[i][j] == '-')
              nr_lines++;
		    }
        }
        nr_tabs = liniute - nr_lines;
	  for (k = 0; k < nr_tabs; k++)
          fputc('\t', fisier_out);
               fputc(Tree->true_false, fisier_out);
              
       fputc('\n', fisier_out); 

}
void dealloc_tree(tree *root) {
    if(root != NULL) {
    dealloc_tree(root->child);
    dealloc_tree(root->next_node);
    free(root);
    }
}