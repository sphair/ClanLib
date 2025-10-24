/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Thomas Gottschalk Larsen
*/

#pragma once

#include "API/Core/XML/xpath_object.h"
#include "xpath_token.h"
#include "xpath_location_step.h"

class CL_XPathEvaluateResult
{
public:
	CL_XPathObject result;
	CL_XPathToken next_token;
};

class CL_XPathEvaluator_Impl
{
public:
	typedef std::vector<CL_DomNode> CL_XPathNodeSet;

public:
	CL_XPathEvaluateResult evaluate(
		const CL_StringRef &expression,
		const CL_XPathNodeSet &context,
		CL_XPathNodeSet::size_type context_node_index,
		CL_XPathToken prev_token) const;

private:
	typedef CL_XPathToken::Operator Operator;
	typedef CL_XPathObject Operand;
	enum ErrorType
	{
		syntax_error,
		expression_error
	};

	bool do_npr(
		std::vector<Operator> &operator_stack,
		std::vector<Operand> &operand_stack) const;

	template<typename T>
	bool compare(const T &a, const T &b, Operator oper) const;
	bool compare_operands(const Operand &a, const Operand &b, Operator oper) const;
	bool compare_node_set(const Operand &a, const Operand &b, Operator oper) const;
	bool compare_boolean(const Operand &a, const Operand &b, Operator oper) const;
	bool compare_number(const Operand &a, const Operand &b, Operator oper) const;
	bool compare_string(const Operand &a, const Operand &b, Operator oper) const;

	CL_XPathToken read_location_path(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		const CL_XPathNodeSet &context,
		CL_XPathNodeSet::size_type context_node_index,
		std::vector<Operand> &operand_stack) const;

	CL_XPathToken read_location_steps(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		const CL_XPathNodeSet &context,
		CL_XPathNodeSet::size_type context_node_index,
		std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack) const;

	CL_XPathToken read_location_step(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		CL_XPathLocationStep &step) const;

	CL_XPathToken read_token(
		const CL_StringRef &expression,
		const CL_XPathToken &previous_token = CL_XPathToken()) const;

	CL_XPathToken skip_predicate_expression(
		const CL_StringRef &expression,
		const CL_XPathToken &previous_token = CL_XPathToken()) const;

	void evaluate_location_step(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void evaluate_location_step_predicates(const CL_XPathNodeSet &context, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef & expression, CL_XPathNodeSet & nodes) const;

	void select_nodes_ancestor(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_ancestor_or_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_attribute(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_child(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_descendant(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_descendant_or_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_following(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_following_sibling(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_namespace(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_parent(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_preceding(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_preceding_sibling(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	void select_nodes_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, std::vector<CL_XPathLocationStep>::size_type step_index, const CL_StringRef &expression, CL_XPathNodeSet &out_nodeset) const;
	bool confirm_step_requirements(const CL_DomNode &node, const CL_XPathLocationStep &step, const CL_StringRef &expression) const;
	bool confirm_step_predicate(CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const CL_XPathLocationStep::Predicate &predicate, const CL_StringRef &expression) const;

	CL_XPathObject call_function(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const CL_StringRef &name, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject get_variable(const CL_StringRef &name) const;

	CL_XPathObject function_last(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_position(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_count(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_id(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_local_name(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_namespace_uri(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_name(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_string(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_concat(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_starts_with(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_contains(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_substring_before(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_substring_after(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_substring(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_string_length(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_normalize_space(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_translate(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_boolean(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_not(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_true(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_false(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_lang(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_number(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_sum(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_floor(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_ceiling(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;
	CL_XPathObject function_round(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const;

	static inline bool is_letter(const CL_StringRef::char_type &c);
	static inline bool is_combining_char(const CL_StringRef::char_type &c);
	static inline bool is_digit(const CL_StringRef::char_type &c);
	static inline bool is_extender(const CL_StringRef::char_type &c);

	static inline CL_XPathObject boolean(const CL_XPathObject &object);
	static inline CL_XPathObject number(CL_XPathObject object);
	static inline CL_XPathObject string(const CL_XPathObject &object);

	static inline bool boolean(const CL_DomNode &node);
	static inline double number(const CL_DomNode &node);
	static inline CL_String string(const CL_DomNode &node);
};
