#include <emscripten/bind.h>

#include "Engine.h"

using namespace emscripten;

float minimize(emscripten::val const & pa0, emscripten::val const & pb0) {
	assert(pa0["length"].as<int>() == pb0["length"].as<int>());
	int n_data = pa0["length"].as<int>();
	assert(n_data % 3 == 0);
	printf("n_data: %d\n", n_data);

	Eigen::MatrixXd pa(3, n_data/3);
	Eigen::MatrixXd pb(3, n_data/3);
	for (int i = 0; i < n_data; i++) {
		pa(i) = pa0[i].as<double>();
		pb(i) = pb0[i].as<double>();
	}

	CMAES::cost_type fcost = [&](const double *params, int n_params) {
		Eigen::Matrix3d rotation = Eigen::Quaterniond(params[0], params[1], params[2], params[3]).toRotationMatrix();
		
		//std::cout << q.x() << " " << q.y() << " " << q.z() << " " << q.w() << std::endl;
		Eigen::Matrix3d scale = Eigen::Vector3d(params[4], params[5], params[6]).asDiagonal();
		
		//std::cout << scale << std::endl;
		Eigen::MatrixXd y = rotation*scale*pa;
		double cost = (pb - y).squaredNorm();
		return cost;
    };

	CMAES::transform_type ftransform = [&](double *params, int n_params) {
		Eigen::Quaterniond q(params[0], params[1], params[2], params[3]); 
		q.normalize();
		params[0] = q.x();
		params[1] = q.y();
		params[2] = q.z();
		params[3] = q.w();
		params[4] = std::abs(params[4]);
		params[5] = std::abs(params[5]);
		params[6] = std::abs(params[6]);
	};

	int n_params = 7;

	CMAES::Engine cmaes;
    Eigen::VectorXd x0(n_params);
	//x0 << q00.x(), q00.y(), q00.z(), q00.w(), scale00(0,0), scale00(1,1), scale00(2,2);
	x0 << 0, 0, 0, 1, 1, 1, 1;
  	double c = fcost(x0.data(), n_params);
	std::cout << "x0: " << x0.transpose() << " fcost(x0): " << c << std::endl;
    double sigma0 = 1;
    Solution sol = cmaes.fmin(x0, n_params, sigma0, 6, 999, fcost, ftransform);
	return sol.f;
}
	

EMSCRIPTEN_BINDINGS(my_module) {
	function("minimize", &minimize);
	register_vector<double>("VectorXd");
}
