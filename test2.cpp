#include "../nanoflann/include/nanoflann.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace nanoflann;

// Comment-out for using random points:
#define REAL_DATASET_FILE "juraset.asc"

#define VERBOSE_OUTPUT

#ifdef VERBOSE_OUTPUT
#	define VERB_COUT std::cout
#else
#	define VERB_COUT if (0) std::cout
#endif


template <typename T>
struct PointCloud
{
	struct Point
	{
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
	inline T kdtree_distance(const T *p1, const size_t idx_p2,size_t size) const
	{
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

	// Default ctor.
	PointCloud() {}

	// load dataset in Freiburg 3D scans format:
	PointCloud(const char* sFil)
	{
		size_t c=0;
		FILE *f = fopen(sFil,"rt");
		if (!f) throw std::runtime_error("can't open dataset file!");
		pts.clear();

		char str[300];
		for (size_t i = 0; i < 15; i++) {
			fgets(str,sizeof(str),f);
		}
		while (fgets(str,sizeof(str),f))
		{
			float x,y,z;
			if (sscanf(str,"%f %f %f",&x,&y,&z)==3)
			{
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

double get_time()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec+tv.tv_usec/1000000.0;
}

double my_random(const double min,const double max)
{
	return min+(max-min)*(rand() % 1000) / 1000.0;
}

template <typename T>
void generateRandomPointCloud(PointCloud<T> &point, const size_t N, const T max_range = 10)
{
	VERB_COUT << "Generating "<< N << " point cloud...";
	const double t0=get_time();

	point.pts.resize(N);
	for (size_t i=0;i<N;i++)
	{
		point.pts[i].x = max_range * (rand() % 1000) / T(1000);
		point.pts[i].y = max_range * (rand() % 1000) / T(1000);
		point.pts[i].z = max_range * (rand() % 1000) / T(1000);
	}

	const double t1=get_time();
	VERB_COUT << "done in "<< (t1-t0)*1e3 << " ms\n";
}

// Load dataset only once:
#ifdef REAL_DATASET_FILE
PointCloud<float> cloud(REAL_DATASET_FILE);
#endif


//FIN CARGAR DATASET!




template <typename num_t>

//funcion de prueba, entrega vecino más cercano. se debe definir el max numero
void perf_test(const size_t max_leaf_elements)
{
#ifndef REAL_DATASET_FILE

	cout << "No existe dataset"<<endl;

#else
	float xUser, yUser, zUser;
	num_t query_pt[3] = { 0, 0, 0}; //elemento a buscar
#endif

	// construct an randomized kd-tree index using 4 kd-trees
	double t0=get_time();

	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 /* dim */
		> my_kd_tree_t;

	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(max_leaf_elements) );
	index.buildIndex();
	double t1=get_time();
	const double At_build = t1-t0;
	VERB_COUT << "Build Index<>: " << (t1-t0)*1e3 << " ms\n";

	// do a knn search
	t0=get_time();

	const size_t num_results = 1;
	size_t ret_index;
	num_t out_dist_sqr;
	nanoflann::KNNResultSet<num_t> resultSet(num_results);
	resultSet.init(&ret_index, &out_dist_sqr );
	index.findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));
	//index.knnSearch(query, indices, dists, num_results, nanoflann::SearchParams(10));

	t1=get_time();
	const double At_search = t1-t0;
	VERB_COUT << "knnSearch(num results= "<<num_results<<"): " << (t1-t0)*1e3 << " ms\n";
	VERB_COUT << "ret_index=" << ret_index << " distancia max" << out_dist_sqr << endl;

	// Output for stats generation:
	cout
		<< cloud.pts.size() << "\t "
		<< max_leaf_elements << "\t "
		<< At_build << "\t "
		<< At_search << "\n";

}



int main(int argc, char** argv)
{

	size_t maxLeaf = 50; // VARIAR ENTRE 10 Y 50!
	perf_test<float>(maxLeaf);



	return 0;
}
