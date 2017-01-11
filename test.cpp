#include "../nanoflann/include/nanoflann.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace nanoflann;

template <typename T>
struct PointCloud{
	struct Point{
		T  x,y,z;
	};
	std::vector<Point>  pts;

	typedef  PointCloud Derived; //!< In this case the dataset class is myself.
	/// CRTP helper method
	inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
	/// CRTP helper method
	inline       Derived& derived()       { return *static_cast<Derived*>(this); }
	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }
	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline T kdtree_distance(const T *p1, const size_t idx_p2,size_t size) const{
		T d0=p1[0]-pts[idx_p2].x;
		T d1=p1[1]-pts[idx_p2].y;
		T d2=p1[2]-pts[idx_p2].z;
		return d0*d0+d1*d1+d2*d2;
	}
  // Returns the dim'th component of the idx'th point in the class:
  // Since this is inlined and the "dim" argument is typically an immediate value, the
  //  "if/else's" are actually solved at compile time.
  inline T kdtree_get_pt(const size_t idx, int dim) const
  {
    if (dim==0) return pts[idx].x;
    else if (dim==1) return pts[idx].y;
    else return pts[idx].z;
  }

  // Optional bounding-box computation: return false to default to a standard bbox computation loop.
  //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
  //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
  template <class BBOX>
  bool kdtree_get_bbox(BBOX &bb) const { return false; }
  // load dataset in Freiburg 3D scans format:
  PointCloud(){
    char str[300];
    cout << "Ingrese nombre archivo: ";
    scanf("%s", str);
    //FILE *f = fopen("juraset.asc","rt");
		FILE *f = fopen(str,"rt");
    if (!f) throw std::runtime_error("can't open dataset file!");
    pts.clear();
    for (size_t i = 0; i < 15; i++) {
      fgets(str,sizeof(str),f);
    }
    while (fgets(str,sizeof(str),f)){
      float x,y,z;
      if (sscanf(str,"%f %f %f",&x,&y,&z)==3){
        pts.resize(pts.size()+1);
        pts.rbegin()->x=x;
        pts.rbegin()->y=y;
        pts.rbegin()->z=z;
      }
    }
    fprintf(stderr,"Read dataset: %u points\n", static_cast<unsigned int>(pts.size()));
    fclose(f);
  }
};

template <typename num_t>
void perf_test(const size_t max_leaf_elements,PointCloud<float> cloud){
	num_t query_pt[3] = { 0, 0, 0};
	//se arma el kd-tree
	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 /* dim */
		> my_kd_tree_t;

	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(max_leaf_elements) );
	index.buildIndex();
	//busqueda en el arbol
	const size_t num_results = 2;
	int aux;
	size_t ret_index;
	num_t out_dist_sqr;
	while(true){
		 cout <<"Ingrese coordenadas x,y,z(seperados por coma): ";
		 aux=scanf("%f,%f,%f",&query_pt[0], &query_pt[1], &query_pt[2]);
		 if(aux==1 && query_pt[0]==0){
			 break;
		 }
		 else if( aux!=3 ){
			 continue;
		 }
		 else{
			 std::cout << query_pt[0]<<","<<query_pt[1]<<","<<query_pt[2] << std::endl;
	 		nanoflann::KNNResultSet<num_t> resultSet(num_results);
	 		resultSet.init(&ret_index, &out_dist_sqr );
	 		index.findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));
	 		cout<< "ret_index=" << ret_index << " out_dist_sqr=" << out_dist_sqr << endl;
		 }
	}
};

int main(int argc, char const *argv[]) {
  PointCloud<float> cloudy;
	// for (size_t i = 0; i < cloudy.pts.size(); i++) {
	//   std::cout << cloudy.pts[i].x << " "
	//   << cloudy.pts[i].y << " "
	//   << cloudy.pts[i].z << " ";
	//   std::cout << std::endl;
	// }
	perf_test<float> (30,cloudy);
  return 0;
}
