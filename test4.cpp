
#include "../nanoflann/include/nanoflann.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace nanoflann;
#define REAL_DATASET_FILE "juraset.asc"

// This is an example of a custom data set class
template <typename T>
struct PointCloud
{
	struct Point
	{
		T  x,y,z;
	};

	std::vector<Point>  pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline T kdtree_distance(const T *p1, const size_t idx_p2,size_t /*size*/) const
	{
		const T d0=p1[0]-pts[idx_p2].x;
		const T d1=p1[1]-pts[idx_p2].y;
		const T d2=p1[2]-pts[idx_p2].z;
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
	bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }
	// Default ctor.
	PointCloud() {}

	// load dataset in Freiburg 3D scans format:
	PointCloud(const char* sFil)
	{
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

template <typename T>
void generateRandomPointCloud(PointCloud<T> &point, const size_t N, const T max_range = 10)
{
	std::cout << "Generating "<< N << " point cloud...";
	point.pts.resize(N);
	for (size_t i=0;i<N;i++)
	{
		point.pts[i].x = max_range * (rand() % 1000) / T(1000);
		point.pts[i].y = max_range * (rand() % 1000) / T(1000);
		point.pts[i].z = max_range * (rand() % 1000) / T(1000);
	}

	std::cout << "done\n";
}





template <typename num_t>
void kdtree_demo(const size_t N)
{
	//PointCloud<num_t> cloud;
	#ifdef REAL_DATASET_FILE
		PointCloud<float> cloud(REAL_DATASET_FILE);
	#endif
	// Generate points:
	//generateRandomPointCloud(cloud, N);

	// construct a kd-tree index:
	#ifndef REAL_DATASET_FILE

		cout << "No existe dataset"<<endl;

	#else
		num_t query_pt[3] = { 0, 0, 0}; //elemento a buscar
	#endif

	/*
	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 
		> my_kd_tree_t;

	my_kd_tree_t   index(3 , cloud, KDTreeSingleIndexAdaptorParams(10));
	index.buildIndex();
	*/

	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 /* dim */
		> my_kd_tree_t;

	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(N) );
	index.buildIndex();

	// ----------------------------------------------------------------
	// knnSearch():  Perform a search for the N closest points
	// ----------------------------------------------------------------
	{
		size_t num_results = 5;
		std::vector<size_t>   ret_index(num_results);
		std::vector<num_t> out_dist_sqr(num_results);

		num_results = index.knnSearch(&query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);
		
		// In case of less points in the tree than requested:
		ret_index.resize(num_results);
		out_dist_sqr.resize(num_results);

		cout << "knnSearch(): num_results=" << num_results << "\n";
		for (size_t i=0;i<num_results;i++)
			cout << "idx["<< i << "]=" << ret_index[i] << " dist["<< i << "]=" << out_dist_sqr[i] << endl;
		cout << "\n";
	}

	// ----------------------------------------------------------------
	// radiusSearch():  Perform a search for the N closest points
	// ----------------------------------------------------------------
	{
		const num_t search_radius = static_cast<num_t>(10);
		std::vector<std::pair<size_t,num_t> >   ret_matches;

		nanoflann::SearchParams params;
		//params.sorted = false;

		const size_t nMatches = index.radiusSearch(&query_pt[0],search_radius, ret_matches, params);

		cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
		for (size_t i=0;i<nMatches;i++)
			cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
		cout << "\n";
	}

}

 int main()
{
	// Randomize Seed
	srand(time(NULL));
	//kdtree_demo<float>(4);
	kdtree_demo<float>(10);
	return 0;
}

