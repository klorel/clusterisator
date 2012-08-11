/*
 * Environement.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef ENV_HPP_
#define ENV_HPP_

#include "../utils/common.h"

class Env {

public:
	std::ostream * get();
	void set(std::ostream &);
	void setNull();
public:
	static Env & Get();
private:
	std::ostream * _Stream;
	Env();
private:
	static Env _Singleton;
};
template<class T> inline std::ostream & operator<<(Env &env, T & t) {
	if (env.get() != 0) {
		(*env.get()) << t;
		return (*env.get());
	} else
		return std::cout;
}


#endif /* ENVIRONEMENT_HPP_ */
