/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#ifndef CREATEOBJECTHANDLER_H
#define CREATEOBJECTHANDLER_H

#include "remote/httphandler.hpp"

namespace icinga
{

class CreateObjectHandler final : public HttpHandler
{
public:
	DECLARE_PTR_TYPEDEFS(CreateObjectHandler);

	bool HandleRequest(const ApiUser::Ptr& user, HttpRequest& request,
		HttpResponse& response, const Dictionary::Ptr& params) override;
};

}

#endif /* CREATEOBJECTHANDLER_H */
