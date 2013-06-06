/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/


#pragma once

#include "api_database.h"

namespace clan
{
/// \addtogroup clanDatabase_System clanDatabase System
/// \{

class DBConnection;
class DBTransactionProvider;
class DBTransaction_Impl;

/// \brief Database transaction.
class CL_API_DATABASE DBTransaction
{
/// \name Construction
/// \{
public:
	enum Type
	{
		deferred,
		immediate,
		exclusive
	};

	/// \brief Constructs a database transaction
	DBTransaction();

	/// \brief Constructs a DBTransaction
	///
	/// \param impl = Shared Ptr
	DBTransaction(const std::shared_ptr<DBTransaction_Impl> &impl);

	~DBTransaction();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the provider interface for this transaction
	DBTransactionProvider *get_provider();
/// \}

/// \name Operations
/// \{
public:
	/// \brief Commits the transaction
	void commit();

	/// \brief Rolls the transaction back
	void rollback();
/// \}

/// \name Implementation
/// \{
private:
	std::shared_ptr<DBTransaction_Impl> impl;

/// \}
};

}

/// \}
