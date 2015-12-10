/*
	precedence.cc
	-------------
*/

#include "vlib/precedence.hh"


#define ARRAY_LEN( a ) (sizeof (a) / sizeof (a)[0])
#define ARRAY_END( a ) ((a) + ARRAY_LEN(a))


namespace vlib
{
	
	enum precedence_level
	{
		Precedence_none = 0,
		
		Precedence_juxtaposition,   // . f(x)
		Precedence_exponentiation,  // ^
		Precedence_unary_math,      // + - (f x)
		Precedence_multiplication,  // * /
		Precedence_addition,        // + -
		Precedence_repeat,          // (*)
		Precedence_map,             // map
		Precedence_inequality,      // < <= > >=
		Precedence_equality,        // == !=
		Precedence_list,            // ,
		Precedence_assignment,      // =
		Precedence_conditional,     // if then else while do
		Precedence_end,             // ;
		Precedence_group,           // (
	};
	
	struct precedence_mapping
	{
		precedence_level  precedence;
		op_type           op;
	};
	
	static const precedence_mapping precedence_table[] =
	{
		{ Precedence_juxtaposition, Op_preinc   },
		{ Precedence_juxtaposition, Op_predec   },
		{ Precedence_juxtaposition, Op_postinc  },
		{ Precedence_juxtaposition, Op_postdec  },
		{ Precedence_juxtaposition, Op_function },
		{ Precedence_juxtaposition, Op_member   },
		{ Precedence_juxtaposition, Op_denote   },
		
		{ Precedence_exponentiation, Op_empower },
		
		{ Precedence_unary_math, Op_named_unary },
		{ Precedence_unary_math, Op_const       },
		{ Precedence_unary_math, Op_var         },
		{ Precedence_unary_math, Op_unary_plus  },
		{ Precedence_unary_math, Op_unary_minus },
		{ Precedence_unary_math, Op_unary_count },
		{ Precedence_unary_math, Op_unary_deref },
		
		{ Precedence_multiplication, Op_multiply },
		{ Precedence_multiplication, Op_divide   },
		{ Precedence_multiplication, Op_percent  },
		{ Precedence_multiplication, Op_modulo   },
		
		{ Precedence_addition, Op_add      },
		{ Precedence_addition, Op_subtract },
		
		{ Precedence_repeat, Op_repeat },
		
		{ Precedence_map, Op_map },
		
		{ Precedence_inequality, Op_lt  },
		{ Precedence_inequality, Op_lte },
		{ Precedence_inequality, Op_gt  },
		{ Precedence_inequality, Op_gte },
		
		{ Precedence_equality, Op_isa     },
		{ Precedence_equality, Op_equal   },
		{ Precedence_equality, Op_unequal },
		
		{ Precedence_list, Op_list },
		
		{ Precedence_assignment, Op_duplicate   },
		{ Precedence_assignment, Op_approximate },
		{ Precedence_assignment, Op_increase_by },
		{ Precedence_assignment, Op_decrease_by },
		{ Precedence_assignment, Op_multiply_by },
		{ Precedence_assignment, Op_divide_by   },
		{ Precedence_assignment, Op_percent_by  },
		
		{ Precedence_conditional, Op_if   },
		{ Precedence_conditional, Op_then },
		{ Precedence_conditional, Op_else },
		
		{ Precedence_conditional, Op_while },
		{ Precedence_conditional, Op_do    },
		
		{ Precedence_end, Op_end },
		
		{ Precedence_group, Op_parens   },
		{ Precedence_group, Op_brackets },
		{ Precedence_group, Op_braces   },
	};
	
	static precedence_level op_precedence( op_type op )
	{
		const precedence_mapping* begin = precedence_table;
		const precedence_mapping* end   = ARRAY_END( precedence_table );
		
		for ( const precedence_mapping* it = begin;  it < end;  ++it )
		{
			if ( it->op == op )
			{
				return it->precedence;
			}
		}
		
		return Precedence_none;
	}
	
	static bool is_right_associative( op_type op )
	{
		return op <= Op_empower  ||  (op >= Op_list  &&  op < Op_end);
	}
	
	bool decreasing_op_precedence( op_type left, op_type right )
	{
		const precedence_level pr_left  = op_precedence( left  );
		const precedence_level pr_right = op_precedence( right );
		
		/*
			Precedence levels start at 1 and *increase* numerically with
			*lower* precedence, so decreasing precedence means the right
			is greater than the left.
			
			If the operator is right-associative, then fudge the level so
			the left is lower-precedence than the right.
		*/
		
		return pr_right >= pr_left + is_right_associative( right );
	}
	
}