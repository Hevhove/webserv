#ifndef CLASS_CONNECTION_HPP
# define CLASS_CONNECTION_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "utils.hpp"
# include "Socket.hpp"

class Connection : public Socket {
	private:
        Request     request;
        Response    response;

	public:
		// Constructors
		Connection();
		virtual ~Connection();
		Connection(const Connection& src);
		Connection& operator=(const Connection& rhs);

		// Methods

		// Getters

};

#endif

