#ifndef CLASS_CONTENT_TOO_LARGE_RESPONSE_HPP
# define CLASS_CONTENT_TOO_LARGE_RESPONSE_HPP

# include "Response.hpp"

class ContentTooLargeResponse : public Response {
	private:

	public:
		// Constructors
		ContentTooLargeResponse();
		virtual ~ContentTooLargeResponse();

		// Public methods
		void	constructResponse(Request& req);
		void	setHeaders();
};

#endif
