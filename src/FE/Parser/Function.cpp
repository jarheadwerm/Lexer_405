/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the GNU GPL 3.0 license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#include "Internal.hpp"
#include "../Ethereal.hpp"

stmt_func_t * parse_func( src_t & src, parse_helper_t * ph )
{
	int tok_ctr = ph->tok_ctr();

	bool is_member_func = false;
	if( ph->peak()->type == TOK_MFN ) is_member_func = true;

	std::vector< GrammarTypes > parents;

	std::vector< stmt_simple_t * > mem_types;
	if( is_member_func ) {
		NEXT_VALID( TOK_LT );
	begin_mfn_type:
		NEXT_VALID2( TOK_IDEN, TOK_STR );
		mem_types.push_back( new stmt_simple_t( SIMPLE_TOKEN, ph->peak(), ph->tok_ctr() ) );
		NEXT_VALID2( TOK_GT, TOK_COMMA );
		if( ph->peak()->type == TOK_COMMA ) goto begin_mfn_type;
	}

	const tok_t * name = nullptr;
	NEXT_VALID2( TOK_IDEN, TOK_STR );
	name = ph->peak();

	NEXT_VALID( TOK_LPAREN );

	expr_res_t arg_expr = { 0, nullptr };
	int arg_expr_end;
	int err;
	stmt_block_t * block = nullptr;

	NEXT_VALID3( TOK_RPAREN, TOK_IDEN, TOK_TDOT );

	if( ph->peak()->type == TOK_RPAREN ) goto end_args;
	err = find_next_of( ph, arg_expr_end, { TOK_RPAREN }, TOK_LPAREN );
	if( err < 0 ) {
		if( err == -1 ) {
			PARSE_FAIL( "could not find the right parenthesis for function '%s'",
				    name->data.c_str() );
		} else if( err == -2 ) {
			PARSE_FAIL( "found beginning of block (left braces) before the ')' for arguments of function '%s'",
				    name->data.c_str() );
		}
		goto fail;
	}
	arg_expr = parse_expr( src, ph, arg_expr_end, false );
	if( arg_expr.res != 0 ) goto fail;
	ph->set_tok_ctr( arg_expr_end );
end_args:
	NEXT_VALID_FAIL( TOK_LBRACE );
	parents.push_back( GRAM_FUNC );
	block = parse_block( src, ph, parents );
	parents.pop_back();
	if( block == nullptr ) goto fail;
	return new stmt_func_t( new stmt_simple_t( SIMPLE_TOKEN, name, tok_ctr + 1 ),
				arg_expr.expr, block, mem_types, tok_ctr );
fail:
	if( arg_expr.expr ) delete arg_expr.expr;
	if( block ) delete block;
	for( auto & mem_type : mem_types ) delete mem_type;
	return nullptr;
}

bool stmt_func_t::bytecode( src_t & src ) const
{
	int block_till_loc = src.bcode.size();
	src.bcode.push_back( { m_tok_ctr, m_name->m_val->line, m_name->m_val->col, IC_BLOCK_TILL, { OP_INT, "<func-block-placeholder>" } } );
	ADD_FUNC();
	m_block->bytecode( src );
	REM_FUNC();
	// not used src.bcode.size() - 1 because the exec loop runs till < end and not <= end
	src.bcode[ block_till_loc ].oper.val = std::to_string( src.bcode.size() );

	if( m_args == nullptr ) {
		src.bcode.push_back( { m_tok_ctr, m_name->m_val->line, m_name->m_val->col, IC_ARGS_TILL, { OP_INT, "-1" } } );
	} else {
		int args_till_loc = src.bcode.size();
		src.bcode.push_back( { m_tok_ctr, m_name->m_val->line, m_name->m_val->col, IC_ARGS_TILL, { OP_INT, "<func-args-placeholder>" } } );
		m_args->bytecode( src );
		src.bcode[ args_till_loc ].oper.val = std::to_string( src.bcode.size() - 1 );
	}

	if( m_mem_types.size() > 0 ) {
		for( auto & mem_type : m_mem_types ) {
			src.bcode.push_back( { m_tok_ctr, mem_type->m_val->line, mem_type->m_val->col, IC_PUSH, { OP_CONST, mem_type->m_val->data } } );
		}
		src.bcode.push_back( { m_tok_ctr, m_mem_types.front()->m_val->line, m_mem_types.front()->m_val->col, IC_PUSH, { OP_INT, std::to_string( m_mem_types.size() ) } } );
		src.bcode.push_back( { m_tok_ctr, m_mem_types.front()->m_val->line, m_mem_types.front()->m_val->col, IC_BUILD_MFN, { OP_CONST, m_name->m_val->data } } );
	} else {
		src.bcode.push_back( { m_tok_ctr, m_name->m_val->line, m_name->m_val->col, IC_BUILD_FN, { OP_CONST, m_name->m_val->data } } );
	}
	return true;
}