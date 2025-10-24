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
*/

#pragma once

#include "API/Core/XML/xpath_object.h"
#include "xpath_token.h"

class CL_XPathLocationStep;

class CL_XPathEvaluateResult
{
public:
	CL_XPathObject result;
	CL_XPathToken next_token;
};

class CL_XPathEvaluator_Impl
{
public:
	CL_XPathEvaluateResult evaluate(
		const CL_StringRef &expression,
		CL_DomNode context_node,
		CL_XPathToken prev_token);

private:
	typedef CL_XPathToken::Operator Operator;
	typedef CL_XPathObject Operand;

	bool do_npr(
		std::vector<Operator> &operator_stack,
		std::vector<Operand> &operand_stack);

	int compare(Operand &a, Operand b);
	int compare_node_set(Operand &a, Operand b);
	int compare_boolean(Operand &a, Operand b);
	int compare_number(Operand &a, Operand b);
	int compare_string(Operand &a, Operand b);

	CL_XPathToken read_location_path(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		CL_DomNode &context_node,
		std::vector<Operand> &operand_stack);

	CL_XPathToken read_location_steps(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		CL_DomNode &context_node,
		std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack);

	CL_XPathToken read_location_step(
		const CL_StringRef &expression,
		CL_XPathToken cur_token,
		CL_XPathLocationStep &step);

	CL_XPathToken read_token(
		const CL_StringRef &expression,
		const CL_XPathToken &previous_token = CL_XPathToken());

	CL_XPathToken skip_predicate_expression(
		const CL_StringRef &expression,
		const CL_XPathToken &previous_token = CL_XPathToken());

	void evaluate_location_step(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_ancestor(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_ancestor_or_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_attribute(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_child(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_descendant(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_descendant_or_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_following(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_following_sibling(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_namespace(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_parent(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_preceding(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_preceding_sibling(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	void select_nodes_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &out_nodeset);
	bool confirm_step_requirements(const CL_DomNode &node, int &node_index, const CL_XPathLocationStep &step, const CL_StringRef &expression);

	CL_XPathObject call_function(const CL_StringRef &name, std::vector<CL_XPathObject> parameters);
	CL_XPathObject get_variable(const CL_StringRef &name);

	CL_XPathObject function_last(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_position(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_count(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_id(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_local_name(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_namespace_uri(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_name(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_string(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_concat(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_starts_with(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_contains(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_substring_before(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_substring_after(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_substring(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_string_length(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_normalize_space(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_translate(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_boolean(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_not(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_true(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_false(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_lang(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_number(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_sum(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_floor(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_ceiling(const std::vector<CL_XPathObject> &parameters);
	CL_XPathObject function_round(const std::vector<CL_XPathObject> &parameters);

	static inline bool is_letter(const CL_StringRef::char_type &c);
	static inline bool is_combining_char(const CL_StringRef::char_type &c);
	static inline bool is_digit(const CL_StringRef::char_type &c);
	static inline bool is_extender(const CL_StringRef::char_type &c);
};
