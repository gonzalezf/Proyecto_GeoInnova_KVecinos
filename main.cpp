using namespace std;
#include "BinaryTree.h"
#include "helper.h"
#include <time.h>
#include <cstddef>
#include "../nanoflann/include/nanoflann.hpp"
using namespace nanoflann;


void dump_mem_usage();

struct PointCloud
{
  struct Point
  {
    float  x,y,z;
  };

  std::vector<Point>  pts;

  // Must return the number of data points
  inline size_t kdtree_get_point_count() const { return pts.size(); }

  // Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
  inline float kdtree_distance(const T *p1, const size_t idx_p2,size_t /*size*/) const
  {
    const float d0=p1[0]-pts[idx_p2].x;
    const float d1=p1[1]-pts[idx_p2].y;
    const float d2=p1[2]-pts[idx_p2].z;
    return d0*d0+d1*d1+d2*d2;
  }

  // Returns the dim'th component of the idx'th point in the class:
  // Since this is inlined and the "dim" argument is typically an immediate value, the
  //  "if/else's" are actually solved at compile time.
  inline float kdtree_get_pt(const size_t idx, int dim) const
  {
    if (dim==0) return pts[idx].x;
    else if (dim==1) return pts[idx].y;
    else return pts[idx].z;
  }

  // Optional bounding-box computation: return false to default to a standard bbox computation loop.
  //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
  //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
  template <class BBOX>
  bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

std::vector<std::vector<float> >  generatePointCloud(string fileName, int dimensionNumber)
{
  std::cout << "Generando nube de puntos...";
  Helper helper;
  std::vector<std::vector<float> > data = helper.loadFile(fileName,dimensionNumber);
  std::cout << "Realizado\n";
  return data;

}

PointCloud transformDataToCloud(vector<vector<float>> data){
  PointCloud cloud;
  for(size_t i = 0; i < data.size();i++){
    for(size_t j = 0; j < 3; j++){
      if(j == 0){
        cloud.pts[i][0]; // x
      }
      if(j == 1){
        cloud.pts[i][1]; // y
      }
      else{
        cloud.pts[i][2]; //z
      }

    }
  }
  return cloud;
}

void kdtree_demo(string fileName, int dimensionNumber)
{

  // Generate points:
  vector<vector<float>> data = generatePointCloud(fileName,dimensionNumber);
  PointCloud cloud = transformDataToCloud(data); //data lista para ser trabajada con nanoflann

  num_t query_pt[3] = { 0.5, 0.5, 0.5}; // elemento a buscar


  // construct a kd-tree index:
  typedef KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<float, PointCloud > ,
    PointCloud,
    3 /* dim */
    > my_kd_tree_t;

  dump_mem_usage();
  /*
  my_kd_tree_t   index(3 , cloud, KDTreeSingleIndexAdaptorParams(10 ) );
  index.buildIndex();

  dump_mem_usage();
  {
    // do a knn search
    const size_t num_results = 1;
    size_t ret_index;
    num_t out_dist_sqr;
    nanoflann::KNNResultSet<num_t> resultSet(num_results);
    resultSet.init(&ret_index, &out_dist_sqr );
    index.findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));

    std::cout << "knnSearch(nn="<<num_results<<"): \n";
    std::cout << "ret_index=" << ret_index << " out_dist_sqr=" << out_dist_sqr << endl;
  }
  */
    // Unsorted radius search:
    /*
    {
    const num_t radius = 1;
    std::vector<std::pair<size_t,num_t> > indices_dists;
    RadiusResultSet<num_t,size_t> resultSet(radius,indices_dists);

    index.findNeighbors(resultSet, query_pt, nanoflann::SearchParams());

    // Get worst (furthest) point, without sorting:
    std::pair<size_t,num_t> worst_pair = resultSet.worst_item();
    cout << "Worst pair: idx=" << worst_pair.first << " dist=" << worst_pair.second << endl;
    }
    */
  

}



int main()
{
  Helper helper;
  string fileName;
  int dimensionNumber;
  cout << "Ingrese nombre archivo: ";
  cin>>textFile;
  cout << "Ingrese número de variables que desee usar (0 para todas las que contiene el archivo): ";
  cin >> dimensionNumber;

  kdtree_demo(fileName,dimensionNumber);

}

/*

  vector<vector<float>> data = helper.loadFile(textFile,dimensionNumber);
  for(size_t i = 0; i < data.size();i++){
    for(size_t j = 0; j < data[0].size();j++){
      cout<< data[i][j]<<" ";
    }
    cout<<endl;
  }
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
    }*/