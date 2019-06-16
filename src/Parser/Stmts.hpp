/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the BSD 3-Clause license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef PARSER_COMMON_HPP
#define PARSER_COMMON_HPP

#include "IO.hpp"
#include "ParseHelper.hpp"

enum GrammarTypes
{
	GRAM_INVALID = -1,
	GRAM_SIMPLE,
	GRAM_EXPR,
	GRAM_ENUM,
	GRAM_LDMOD,
	GRAM_IMPORT,

	_GRAM_LAST,
};

extern const char * GrammarTypeStrs[ _GRAM_LAST ];

class stmt_base_t
{
public:
	const GrammarTypes m_type;
	const int m_tok_ctr;
	stmt_base_t( const GrammarTypes type, const int tok_ctr );
	virtual ~stmt_base_t();
	virtual void disp( const bool has_next ) const = 0;
};

enum SimpleType
{
	SIMPLE_TOKEN,
	SIMPLE_OPER,

	_SIMPLE_LAST,
};

extern const char * SimpleTypeStrs[ _SIMPLE_LAST ];

class stmt_simple_t : public stmt_base_t
{
public:
	const SimpleType m_stype;
	const tok_t * m_val;
	stmt_simple_t( const SimpleType stype, const tok_t * val,
		       const int tok_ctr );
	~stmt_simple_t();
	void disp( const bool has_next ) const;
};

class stmt_enum_t : public stmt_base_t
{
	const tok_t * m_name;
	const std::vector< tok_t * > m_vals;
public:
	stmt_enum_t( const tok_t * name, const std::vector< tok_t * > & vals,
		     const int tok_ctr );
	~stmt_enum_t();
	void disp( const bool has_next ) const;
};

class stmt_ldmod_t : public stmt_base_t
{
	const tok_t * m_what;
	const tok_t * m_as;
public:
	stmt_ldmod_t( const tok_t * what, const tok_t * as,
		      const int tok_ctr );
	~stmt_ldmod_t();
	void disp( const bool has_next ) const;
};

class stmt_import_t : public stmt_base_t
{
	const std::vector< tok_t * > m_what;
	const tok_t * m_as;
public:
	stmt_import_t( const std::vector< tok_t * > & what, const tok_t * as,
		       const int tok_ctr );
	~stmt_import_t();
	void disp( const bool has_next ) const;
};

enum ExprType
{
	EXPR_BASIC,
	EXPR_ARRAY,
	EXPR_MAP,
	EXPR_STRUCT,

	_EXPR_LAST,
};

extern const char * ExprTypeStrs[ _EXPR_LAST ];

class stmt_expr_t : public stmt_base_t
{
	const stmt_base_t * m_lhs, * m_rhs;
	const stmt_simple_t * m_oper;
public:
	ExprType m_etype;
	bool m_is_top_expr;
	const stmt_simple_t * m_struct_decl;
	stmt_expr_t( const ExprType etype, const stmt_base_t * lhs, const stmt_simple_t * oper,
		     const stmt_base_t * rhs, const int tok_ctr );
	~stmt_expr_t();
	void disp( const bool has_next ) const;
};

#define PARSE_FAIL( ... ) src_fail( src.code[ ph.peak()->line - 1 ], \
				    ph.peak()->line, ph.peak()->col, __VA_ARGS__ )

#endif // PARSER_COMMON_HPP