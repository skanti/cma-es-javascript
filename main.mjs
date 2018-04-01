import CMAES from "./CMAES";

function dummy() {
    return new Promise((resolve, reject) => {
		resolve(database);
	});
}

function run() {
	var a = CMAES.fun(9);
	console.log(a)
}


run()
