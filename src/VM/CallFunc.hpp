/*
	Copyright (c) 2019, Electrux
	All rights reserved.
	Using the GNU GPL 3.0 license for the project,
	main LICENSE file resides in project's root directory.
	Please read that file and understand the license terms
	before using or altering the project.
*/

#ifndef VM_CALL_FUNC_HPP
#define VM_CALL_FUNC_HPP

#include "Core.hpp"

int CallFunc( vm_state_t & vm, func_call_data_t & fd, const int ins_ctr );

#endif // VM_CALL_FUNC_HPP