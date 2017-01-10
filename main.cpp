using namespace std;
#include "BinaryTree.h"
#include "helper.h"
#include <string>
#include <time.h>
#include <cstddef>

/*
int main(int argc, char **argv){
  time_t start = time(0);

  //Classes definition
  Helper helper;

  // Get data
  // an array with 5 rows and 2 columns.
  float a[5][2] = { {0.3,0}, {1.5,2}, {2,4}, {3,6},{4.8,8}};
 


  //print time elapsed
  double seconds_since_start = difftime( time(0), start);
  helper.ImprimirTiempoTranscurrido(seconds_since_start);


  return 0;
}*/

int main()
{
    BinarySearchTree b;
    int ch,tmp,tmp1;
    while(1)
    {
       cout<<endl<<endl;
       cout<<" Binary Search Tree Operations "<<endl;
       cout<<" ----------------------------- "<<endl;
       cout<<" 1. Insertion/Creation "<<endl;
       cout<<" 2. In-Order Traversal "<<endl;
       cout<<" 3. Pre-Order Traversal "<<endl;
       cout<<" 4. Post-Order Traversal "<<endl;
       cout<<" 5. Removal "<<endl;
       cout<<" 6. Exit "<<endl;
       cout<<" Enter your choice : ";
       cin>>ch;
       switch(ch)
       {
           case 1 : cout<<" Enter Number to be inserted : ";
                    cin>>tmp;
                    b.insert(tmp);
                    break;
           case 2 : cout<<endl;
                    cout<<" In-Order Traversal "<<endl;
                    cout<<" -------------------"<<endl;
                    b.print_inorder();
                    break;
           case 3 : cout<<endl;
                    cout<<" Pre-Order Traversal "<<endl;
                    cout<<" -------------------"<<endl;
                    b.print_preorder();
                    break;
           case 4 : cout<<endl;
                    cout<<" Post-Order Traversal "<<endl;
                    cout<<" --------------------"<<endl;
                    b.print_postorder();
                    break;
           case 5 : cout<<" Enter data to be deleted : ";
                    cin>>tmp1;
                    b.remove(tmp1);
                    break;
           case 6 : 
                    return 0;
                    
       }
    }
}