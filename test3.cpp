#include "../nanoflann/include/nanoflann.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace nanoflann;

#define REAL_DATASET_FILE "juraset.asc"

void dump_mem_usage();

// This is an exampleof a custom data set class
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
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

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



#ifdef REAL_DATASET_FILE
PointCloud<float> cloud(REAL_DATASET_FILE);
#endif



template <typename num_t>
void kdtree_demo()
{
	#ifndef REAL_DATASET_FILE

		cout << "No existe dataset"<<endl;

	#else
		num_t query_pt[3] = { 0, 0, 0}; //elemento a buscar
	#endif
	// construct a kd-tree index:
	typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<num_t, PointCloud<num_t> > ,
		PointCloud<num_t>,
		3 /* dim */
		> my_kd_tree_t;

	dump_mem_usage();

	my_kd_tree_t   index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
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

		std::cout << "knnSearch(number results="<<num_results<<"): \n";
		std::cout << "ret_index=" << ret_index << " distancia max= " << out_dist_sqr << endl;
	}
	{
		// Unsorted radius search:
		const num_t radius = 1;
		std::vector<std::pair<size_t,num_t> > indices_dists;
		RadiusResultSet<num_t,size_t> resultSet(radius,indices_dists);

		index.findNeighbors(resultSet, query_pt, nanoflann::SearchParams());

		// Get worst (furthest) point, without sorting:
		std::pair<size_t,num_t> worst_pair = resultSet.worst_item();
		cout << "Worst pair: idx=" << worst_pair.first << " dist=" << worst_pair.second << endl;
	}

}

int main()
{
	// Randomize Seed
	srand(time(NULL));
	kdtree_demo<float>();
	return 0;
}

void dump_mem_usage()
{
	FILE* f=fopen("/proc/self/statm","rt");
	if (!f) return;
	char str[300];
	size_t n=fread(str,1,200,f);
	str[n]=0;
	printf("MEM: %s\n",str);
	fclose(f);
}
