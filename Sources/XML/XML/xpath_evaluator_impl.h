/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "API/XML/xpath_object.h"
#include "xpath_token.h"
#include "xpath_location_step.h"

namespace clan
{
	class XPathEvaluateResult
	{
	public:
		XPathObject result;
		XPathToken next_token;
	};

	class XPathEvaluator_Impl
	{
	public:
		typedef std::vector<DomNode> XPathNodeSet;

	public:
		XPathEvaluateResult evaluate(
			const std::string &expression,
			const XPathNodeSet &context,
			XPathNodeSet::size_type context_node_index,
			XPathToken prev_token) const;

	private:
		typedef XPathToken::Operator Operator;
		typedef XPathObject Operand;
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

		XPathToken read_location_path(
			const std::string &expression,
			XPathToken cur_token,
			const XPathNodeSet &context,
			XPathNodeSet::size_type context_node_index,
			std::vector<Operand> &operand_stack) const;

		XPathToken read_location_steps(
			const std::string &expression,
			XPathToken cur_token,
			const XPathNodeSet &context,
			XPathNodeSet::size_type context_node_index,
			std::vector<XPathEvaluator_Impl::Operand> &operand_stack) const;

		XPathToken read_location_step(
			const std::string &expression,
			XPathToken cur_token,
			XPathLocationStep &step) const;

		XPathToken read_token(
			const std::string &expression,
			const XPathToken &previous_token = XPathToken()) const;

		XPathToken skip_predicate_expression(
			const std::string &expression,
			const XPathToken &previous_token = XPathToken()) const;

		void evaluate_location_step(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void evaluate_location_step_predicates(const XPathNodeSet &context, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string & expression, XPathNodeSet & nodes) const;

		void select_nodes_ancestor(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_ancestor_or_self(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_attribute(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_child(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_descendant(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_descendant_or_self(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_following(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_following_sibling(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_namespace(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_parent(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_preceding(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_preceding_sibling(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		void select_nodes_self(const XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const std::vector<XPathLocationStep> &steps, std::vector<XPathLocationStep>::size_type step_index, const std::string &expression, XPathNodeSet &out_nodeset) const;
		bool confirm_step_requirements(const DomNode &node, const XPathLocationStep &step, const std::string &expression) const;
		bool confirm_step_predicate(XPathNodeSet &context, XPathNodeSet::size_type context_node_index, const XPathLocationStep::Predicate &predicate, const std::string &expression) const;

		XPathObject call_function(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::string &name, const std::vector<XPathObject> &parameters) const;
		XPathObject get_variable(const std::string &name) const;

		XPathObject function_last(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_position(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_count(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_id(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_local_name(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_namespace_uri(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_name(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_string(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_concat(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_starts_with(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_contains(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_substring_before(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_substring_after(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_substring(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_string_length(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_normalize_space(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_translate(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_boolean(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_not(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_true(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_false(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_lang(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_number(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_sum(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_floor(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_ceiling(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;
		XPathObject function_round(const XPathNodeSet& context, XPathNodeSet::size_type context_node_index, const std::vector<XPathObject> &parameters) const;

		static inline bool is_letter(const std::string::value_type &c);
		static inline bool is_combining_char(const std::string::value_type &c);
		static inline bool is_digit(const std::string::value_type &c);
		static inline bool is_extender(const std::string::value_type &c);

		static inline XPathObject boolean(const XPathObject &object);
		static inline XPathObject number(XPathObject object);
		static inline XPathObject string(const XPathObject &object);

		static inline bool boolean(const DomNode &node);
		static inline double number(const DomNode &node);
		static inline std::string string(const DomNode &node);
	};
}
