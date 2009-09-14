/*!
	\file PTXToLLVMTranslator.cpp
	\date Wednesday July 29, 2009
	\author Gregory Diamos <gregory.diamos@gatech.edu>
	\brief The source file for the PTXToLLVMTranslator class
	\comment : Written with subdued haste
*/

#ifndef PTX_TO_LLVM_TRANSLATOR_CPP_INCLUDED
#define PTX_TO_LLVM_TRANSLATOR_CPP_INCLUDED

#include <ocelot/translator/interface/PTXToLLVMTranslator.h>
#include <ocelot/ir/interface/LLVMInstruction.h>
#include <ocelot/ir/interface/LLVMKernel.h>
#include <ocelot/ir/interface/PTXInstruction.h>

#include <hydrazine/implementation/debug.h>

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 0

namespace translator
{
	ir::LLVMInstruction::DataType PTXToLLVMTranslator::_translate( 
		ir::PTXOperand::DataType type )
	{
		switch( type )
		{
			case ir::PTXOperand::pred:
			{
				return ir::LLVMInstruction::I1;
				break;
			}			
			case ir::PTXOperand::b8: /* fall through */
			case ir::PTXOperand::u8: /* fall through */
			case ir::PTXOperand::s8:
			{
				return ir::LLVMInstruction::I8;
				break;
			}
			case ir::PTXOperand::b16: /* fall through */
			case ir::PTXOperand::s16: /* fall through */
			case ir::PTXOperand::u16:
			{
				return ir::LLVMInstruction::I16;
				break;
			}
			case ir::PTXOperand::b32: /* fall through */
			case ir::PTXOperand::u32: /* fall through */
			case ir::PTXOperand::s32:
			{
				return ir::LLVMInstruction::I32;
				break;
			}
			case ir::PTXOperand::b64: /* fall through */
			case ir::PTXOperand::s64: /* fall through */
			case ir::PTXOperand::u64:
			{
				return ir::LLVMInstruction::I64;
				break;
			}
			case ir::PTXOperand::f32:
			{
				return ir::LLVMInstruction::F32;
				break;
			}
			case ir::PTXOperand::f64:
			{
				return ir::LLVMInstruction::F64;
				break;
			}
			default:
			{
				assertM( false, "PTXOperand datatype " 
					+ ir::PTXOperand::toString( type ) 
					+ " not supported." );
			}
		}
		return ir::LLVMInstruction::InvalidDataType;
	}

	void PTXToLLVMTranslator::_doubleWidth( ir::LLVMInstruction::DataType& t )
	{
		switch( t )
		{
			case ir::LLVMInstruction::I1:
			{
				assertM( false, "Cannot double i1" );
				break;
			}
			case ir::LLVMInstruction::I8:
			{
				t = ir::LLVMInstruction::I16;
				break;
			}
			case ir::LLVMInstruction::I16:
			{
				t = ir::LLVMInstruction::I32;
				break;
			}
			case ir::LLVMInstruction::I32:
			{
				t = ir::LLVMInstruction::I64;
				break;
			}
			case ir::LLVMInstruction::I64:
			{
				assertM( false, "Cannot double i64" );
				break;
			}
			case ir::LLVMInstruction::F32:
			{
				t = ir::LLVMInstruction::F64;
				break;
			}
			case ir::LLVMInstruction::F64:
			{
				t = ir::LLVMInstruction::F128;
				break;
			}
			case ir::LLVMInstruction::F128:
			{
				assertM( false, "Cannot double fp128" );
				break;
			}
			case ir::LLVMInstruction::InvalidDataType:
			{
				assertM( false, "Cannot double invalid data type" );
				break;
			}
		}
	}

	ir::LLVMInstruction::Comparison PTXToLLVMTranslator::_translate( 
		ir::PTXInstruction::CmpOp op, bool isInt )
	{
		if( isInt )
		{
			switch( op )
			{
				case ir::PTXInstruction::Eq: 
					return ir::LLVMInstruction::Eq; break;
				case ir::PTXInstruction::Ne: 
					return ir::LLVMInstruction::Ne; break;
				case ir::PTXInstruction::Lo: /* fall through */
				case ir::PTXInstruction::Lt: 
					return ir::LLVMInstruction::Slt; break;
				case ir::PTXInstruction::Ls: /* fall through */
				case ir::PTXInstruction::Le: 
					return ir::LLVMInstruction::Sle; break;
				case ir::PTXInstruction::Hi: /* fall through */
				case ir::PTXInstruction::Gt: 
					return ir::LLVMInstruction::Sgt; break;
				case ir::PTXInstruction::Hs: /* fall through */
				case ir::PTXInstruction::Ge: 
					return ir::LLVMInstruction::Sge; break;
				default: assertM( false, "Invalid comparison " 
					<< ir::PTXInstruction::toString( op ) 
					<< " for integer operand." );
			}
		}
		else
		{
			switch( op )
			{
				case ir::PTXInstruction::Eq: 
					return ir::LLVMInstruction::Oeq; break;
				case ir::PTXInstruction::Ne: 
					return ir::LLVMInstruction::One; break;
				case ir::PTXInstruction::Lt: 
					return ir::LLVMInstruction::Olt; break;
				case ir::PTXInstruction::Le: 
					return ir::LLVMInstruction::Ole; break;
				case ir::PTXInstruction::Gt: 
					return ir::LLVMInstruction::Ogt; break;
				case ir::PTXInstruction::Ge: 
					return ir::LLVMInstruction::Oge; break;
				case ir::PTXInstruction::Lo: 
					return ir::LLVMInstruction::Olt; break;
				case ir::PTXInstruction::Ls: 
					return ir::LLVMInstruction::Ole; break;
				case ir::PTXInstruction::Hi: 
					return ir::LLVMInstruction::Ogt; break;
				case ir::PTXInstruction::Hs: 
					return ir::LLVMInstruction::Oge; break;
				case ir::PTXInstruction::Equ: 
					return ir::LLVMInstruction::Ueq; break;
				case ir::PTXInstruction::Neu: 
					return ir::LLVMInstruction::Une; break;
				case ir::PTXInstruction::Ltu: 
					return ir::LLVMInstruction::Ult; break;
				case ir::PTXInstruction::Leu: 
					return ir::LLVMInstruction::Ule; break;
				case ir::PTXInstruction::Gtu: 
					return ir::LLVMInstruction::Ugt; break;
				case ir::PTXInstruction::Geu: 
					return ir::LLVMInstruction::Uge; break;
				case ir::PTXInstruction::Num: 
					return ir::LLVMInstruction::Ord; break;
				case ir::PTXInstruction::Nan: 
					return ir::LLVMInstruction::Uno; break;
				default: assertM( false, "Invalid comparison " 
					<< ir::PTXInstruction::toString( op ) 
					<< " for floating point operand." );
			}
		}
		
		return ir::LLVMInstruction::True;
	}

	ir::LLVMInstruction::Type PTXToLLVMTranslator::_getCtaContextType()
	{
		ir::LLVMInstruction::Type context;
		
		context.category = ir::LLVMInstruction::Type::Structure;
		context.members.resize( 12 );

		context.members[0].category = ir::LLVMInstruction::Type::Structure;
		context.members[0].label = "%Dimension";

		context.members[1] = context.members[0];
		context.members[2] = context.members[0];
		context.members[3] = context.members[0];

		context.members[4].category = ir::LLVMInstruction::Type::Pointer;
		context.members[4].type = ir::LLVMInstruction::I8;

		context.members[5] = context.members[4];
		context.members[6] = context.members[4];
		context.members[7] = context.members[4];
		
		context.members[8].category = ir::LLVMInstruction::Type::Element;
		context.members[8].type = ir::LLVMInstruction::I64;

		context.members[9] = context.members[8];
		context.members[10] = context.members[8];
		context.members[11] = context.members[8];
		
		return context;
	}

	void PTXToLLVMTranslator::_yield( unsigned int continuation )
	{
		assertM( continuation == 0, 
			"No support for yielding anywhere except the program exit" );
		ir::LLVMRet ret;
		
		ret.d.constant = true;
		ret.d.type.category = ir::LLVMInstruction::Type::Element;
		ret.d.type.type = ir::LLVMInstruction::I32;
		ret.d.i32 = continuation;
		
		_add( ret );
	}

	ir::LLVMInstruction::Operand PTXToLLVMTranslator::_translate( 
		const ir::PTXOperand& o, ir::PTXInstruction::AddressSpace space,
		ir::PTXInstruction::Vec vector )
	{
		ir::LLVMInstruction::Operand op( o.identifier, 
			o.addressMode == ir::PTXOperand::Immediate );

		op.type.type = _translate( o.type );

		if( o.vec == ir::PTXOperand::v1 )
		{
			op.type.category = ir::LLVMInstruction::Type::Element;
		}
		else
		{
			op.type.category = ir::LLVMInstruction::Type::Vector;
		}
		
		switch( o.addressMode )
		{
			case ir::PTXOperand::Register: /* fall through */
			case ir::PTXOperand::Indirect:
			{
				std::stringstream stream;
				stream << "%r" << o.reg;
				op.name = stream.str();
				break;
			}
			case ir::PTXOperand::Immediate:
			{
				switch( o.type )
				{
					case ir::PTXOperand::s8: /* fall through */
					case ir::PTXOperand::s16: /* fall through */
					case ir::PTXOperand::s32: /* fall through */
					case ir::PTXOperand::s64: /* fall through */
					case ir::PTXOperand::u8: /* fall through */
					case ir::PTXOperand::u16: /* fall through */
					case ir::PTXOperand::u32: /* fall through */
					case ir::PTXOperand::u64: /* fall through */
					case ir::PTXOperand::b8: /* fall through */
					case ir::PTXOperand::b16: /* fall through */
					case ir::PTXOperand::b32: /* fall through */
					case ir::PTXOperand::b64:
					{
						op.i64 = o.imm_uint;
						break;
					}
					case ir::PTXOperand::f32:
					{
						op.f32 = o.imm_float;
						break;
					}
					case ir::PTXOperand::f64:
					{
						op.f64 = o.imm_float;
						break;
					}
					default:
					{
						assertM( false, "PTXOperand datatype " 
							+ ir::PTXOperand::toString( o.type ) 
							+ " not supported for immediate operand." );
					}				
				}
				break;
			}
			case ir::PTXOperand::Address:
			{
				op.name = _loadMemoryBase( space, o.type, o.offset, vector );
				if( op.name == "" ) op.name = "@" + o.identifier;
				op.type.category = ir::LLVMInstruction::Type::Pointer;
				break;
			}
			case ir::PTXOperand::Label:
			{
				assertM( false, "PTXOperand datatype " 
					+ ir::PTXOperand::toString( o.type ) 
					+ " not supported." );
				break;
			}
			case ir::PTXOperand::Special:
			{
				op.name = _loadSpecialRegister( o.special );
				break;
			}
			case ir::PTXOperand::Invalid:
			{
				assertM( false, "Cannot translate invalid PTX operand." );
			}
		}
		
		op.type.vector = o.vec;
		
		return op;
	}

	void PTXToLLVMTranslator::_swapAllExceptName( 
		ir::LLVMInstruction::Operand& o, const ir::PTXOperand& i )
	{
		std::string temp = o.name;
		o = _translate( i );
		o.name = temp;
	}

	void PTXToLLVMTranslator::_convertPtxToSsa()
	{
		report( " Doing basic PTX register allocation");
		ir::Kernel::assignRegisters( _llvmKernel->instructions );
		report( " Building the dataflow graph");
		_graph = new analysis::DataflowGraph( *_llvmKernel->ptxCFG, 
			_llvmKernel->instructions );
		report( " Converting PTX to SSA form");
		_graph->toSsa();
	}

	void PTXToLLVMTranslator::_translateInstructions()
	{
		for( analysis::DataflowGraph::iterator 
			block = ++_graph->begin(); block != _graph->end(); ++block )
		{
			_newBlock( block->label() );
			report( "  Translating Phi Instructions" );
			for( analysis::DataflowGraph::PhiInstructionVector::const_iterator 
				phi = block->phis().begin(); 
				phi != block->phis().end(); ++phi )
			{
				ir::LLVMPhi p;
				analysis::DataflowGraph::RegisterVector::const_iterator 
					s = phi->s.begin();
				for( ; s != phi->s.end(); ++s )
				{			
					ir::LLVMPhi::Node node;
					
					try
					{
						node.label = "%" + block->producer( *s );
					}
					catch( analysis::DataflowGraph::NoProducerException& e )
					{
						node.label = "%$OcelotRegisterInitializerBlock";
						_uninitialized.push_back( *s );
					}
					
					node.reg = s->id;

					std::stringstream stream;
					stream << "%r" << s->id;
					
					node.operand.name = stream.str();
					node.operand.type.category 
						= ir::LLVMInstruction::Type::Element;
					node.operand.type.type = _translate( s->type );
					
					p.nodes.push_back( node );
				}

				assert( !p.nodes.empty() );
				
				std::stringstream stream;
				stream << "%r" << phi->d.id;
				p.d.name = stream.str();
				p.d.type.category = ir::LLVMInstruction::Type::Element;
				p.d.type.type = _translate( phi->d.type );
				
				_add( p );
			}
			report( "  Translating Instructions" );
			for( analysis::DataflowGraph::InstructionVector::const_iterator 
				instruction = block->instructions().begin();
				instruction != block->instructions().end(); ++instruction )
			{
				_instructionId = instruction->id;
				_translate( *instruction, *block );
			}
			
			if( block->targets().empty() )
			{
				if( block->fallthrough() != _graph->end() )
				{
					ir::LLVMBr branch;
				
					branch.iftrue = "%" + block->fallthrough()->label();
					_add( branch );
				}
				else
				{
					_yield( 0 );
				}
			}
		}
	}

	void PTXToLLVMTranslator::_newBlock( const std::string& name )
	{
		report( " Translating basic block: " << name );
		_llvmKernel->_statements.push_back( ir::LLVMStatement( name ) );
	}

	void PTXToLLVMTranslator::_translate( 
		const analysis::DataflowGraph::Instruction& i, 
		const analysis::DataflowGraph::Block& block )
	{
		assert( i.id < _llvmKernel->instructions.size() );
		_translate( _llvmKernel->instructions[ i.id ], block );
	}

	void PTXToLLVMTranslator::_translate( const ir::PTXInstruction& i, 
		const analysis::DataflowGraph::Block& block )
	{
		report( "   Translating: " << i.toString() );
		assertM( i.valid() == "", "Instruction " << i.toString() 
			<< " is not valid: " << i.valid() );
		switch( i.opcode )
		{
			case ir::PTXInstruction::Abs: _translateAbs( i ); break;
			case ir::PTXInstruction::Add: _translateAdd( i ); break;
			case ir::PTXInstruction::AddC: _translateAddC( i ); break;
			case ir::PTXInstruction::And: _translateAnd( i ); break;
			case ir::PTXInstruction::Atom: _translateAtom( i ); break;
			case ir::PTXInstruction::Bar: _translateBar( i ); break;
			case ir::PTXInstruction::Bra: _translateBra( i, block ); break;
			case ir::PTXInstruction::Brkpt: _translateBrkpt( i ); break;
			case ir::PTXInstruction::Call: _translateCall( i ); break;
			case ir::PTXInstruction::CNot: _translateCNot( i ); break;
			case ir::PTXInstruction::Cos: _translateCos( i ); break;
			case ir::PTXInstruction::Cvt: _translateCvt( i ); break;
			case ir::PTXInstruction::Div: _translateDiv( i ); break;
			case ir::PTXInstruction::Ex2: _translateEx2( i ); break;
			case ir::PTXInstruction::Exit: _translateExit( i ); break;
			case ir::PTXInstruction::Ld: _translateLd( i ); break;
			case ir::PTXInstruction::Lg2: _translateLg2( i ); break;
			case ir::PTXInstruction::Mad24: _translateMad24( i ); break;
			case ir::PTXInstruction::Mad: _translateMad( i ); break;
			case ir::PTXInstruction::Max: _translateMax( i ); break;
			case ir::PTXInstruction::Membar: _translateMembar( i ); break;
			case ir::PTXInstruction::Min: _translateMin( i ); break;
			case ir::PTXInstruction::Mov: _translateMov( i ); break;
			case ir::PTXInstruction::Mul24: _translateMul24( i ); break;
			case ir::PTXInstruction::Mul: _translateMul( i ); break;
			case ir::PTXInstruction::Neg: _translateNeg( i ); break;
			case ir::PTXInstruction::Not: _translateNot( i ); break;
			case ir::PTXInstruction::Or: _translateOr( i ); break;
			case ir::PTXInstruction::Pmevent: _translatePmevent( i ); break;
			case ir::PTXInstruction::Rcp: _translateRcp( i ); break;
			case ir::PTXInstruction::Red: _translateRed( i ); break;
			case ir::PTXInstruction::Rem: _translateRem( i ); break;
			case ir::PTXInstruction::Ret: _translateRet( i ); break;
			case ir::PTXInstruction::Rsqrt: _translateRsqrt( i ); break;
			case ir::PTXInstruction::Sad: _translateSad( i ); break;
			case ir::PTXInstruction::SelP: _translateSelP( i ); break;
			case ir::PTXInstruction::Set: _translateSet( i ); break;
			case ir::PTXInstruction::SetP: _translateSetP( i ); break;
			case ir::PTXInstruction::Shl: _translateShl( i ); break;
			case ir::PTXInstruction::Shr: _translateShr( i ); break;
			case ir::PTXInstruction::Sin: _translateSin( i ); break;
			case ir::PTXInstruction::SlCt: _translateSlCt( i ); break;
			case ir::PTXInstruction::Sqrt: _translateSqrt( i ); break;
			case ir::PTXInstruction::St: _translateSt( i ); break;
			case ir::PTXInstruction::Sub: _translateSub( i ); break;
			case ir::PTXInstruction::SubC: _translateSubC( i ); break;
			case ir::PTXInstruction::Tex: _translateTex( i ); break;
			case ir::PTXInstruction::Trap: _translateTrap( i ); break;
			case ir::PTXInstruction::Vote: _translateVote( i ); break;
			case ir::PTXInstruction::Xor: _translateXor( i ); break;
			default:
			{
				assertM( false, "Opcode " 
					<< ir::PTXInstruction::toString( i.opcode ) 
					<< " not supported." );
				break;
			}
		}
	}

	void PTXToLLVMTranslator::_translateAbs( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFcmp compare;
			
			compare.comparison = ir::LLVMInstruction::Olt;
			compare.d.type.category = ir::LLVMInstruction::Type::Element;
			compare.d.type.type = ir::LLVMInstruction::I1;
			compare.d.name = _tempRegister();
			compare.a = _translate( i.a );
			compare.b = compare.a;
			compare.b.constant = true;
			
			if( ir::PTXOperand::f64 == i.a.type )
			{
				compare.b.f64 = 0;
			}
			else
			{
				compare.b.f32 = 0;
			}
			
			_add( compare );
			
			ir::LLVMFsub sub;
			
			sub.d = destination;
			sub.d.name = _tempRegister();
			sub.a = compare.a;
			sub.a.constant = true;
			
			if( ir::PTXOperand::f64 == i.a.type )
			{
				sub.a.f64 = 0;
			}
			else
			{
				sub.a.f32 = 0;
			}			
			
			sub.b = compare.a;
			
			_add( sub );
			
			ir::LLVMSelect select;
			
			select.d = destination;
			select.condition = compare.d;
			select.a = sub.d;
			select.b = compare.a;
			
			_add( select );
		}
		else
		{
			ir::LLVMIcmp compare;
			
			compare.comparison = ir::LLVMInstruction::Slt;
			compare.d.type.category = ir::LLVMInstruction::Type::Element;
			compare.d.type.type = ir::LLVMInstruction::I1;
			compare.d.name = _tempRegister();
			compare.a = _translate( i.a );
			compare.b = compare.a;
			compare.b.constant = true;
			
			if( ir::PTXOperand::s64 == i.a.type )
			{
				compare.b.i64 = 0;
			}
			else
			{
				compare.b.i32 = 0;
			}
			
			_add( compare );
			
			ir::LLVMSub sub;
			
			sub.d = destination;
			sub.d.name = _tempRegister();
			sub.a = compare.a;
			sub.a.constant = true;
			
			if( ir::PTXOperand::s64 == i.a.type )
			{
				sub.a.i64 = 0;
			}
			else
			{
				sub.a.i32 = 0;
			}			
			
			sub.b = compare.a;
			
			_add( sub );
			
			ir::LLVMSelect select;
			
			select.d = destination;
			select.condition = compare.d;
			select.a = sub.d;
			select.b = compare.a;
			
			_add( select );
		}
		
		_predicateEpilogue( i, destination );
	}

	void PTXToLLVMTranslator::_translateAdd( const ir::PTXInstruction& i )
	{
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFadd add;
		
			add.d = _destination( i );
			add.a = _translate( i.a );
			add.b = _translate( i.b );
		
			_add( add );	
			
			if( i.modifier & ir::PTXInstruction::sat )
			{
				ir::LLVMFcmp compare;
				
				compare.d.name = _tempRegister();
				compare.d.type.type = ir::LLVMInstruction::I1;
				compare.d.type.category = ir::LLVMInstruction::Type::Element;
				compare.comparison = ir::LLVMInstruction::Ule;
				compare.a = add.d;
				compare.b.type.type = compare.a.type.type;
				compare.b.type.category = ir::LLVMInstruction::Type::Element;
				compare.b.constant = true;
				if( compare.b.type.type == ir::LLVMInstruction::F32 )
				{
					compare.b.f32 = 0;
				}
				else
				{
					compare.b.f64 = 0;
				}
				
				ir::LLVMSelect select;
				
				select.d.name = _tempRegister();
				select.d.type.type = add.d.type.type;
				select.d.type.category = add.d.type.category;
				select.condition = compare.d;
				select.a = compare.b;
				select.b = add.d;
				
				_add( compare );
				_add( select );
				_predicateEpilogue( i, select.d );
			}
			else
			{
				_predicateEpilogue( i, add.d );
			}
		}
		else
		{
			assertM( !(i.modifier & ir::PTXInstruction::sat), 
				"Saturation for ptx int add not supported." );

			ir::LLVMAdd add;
			
			add.d = _destination( i );
			add.a = _translate( i.a );
			add.b = _translate( i.b );
		
			_add( add );
			_predicateEpilogue( i, add.d );
		}
		
	}

	void PTXToLLVMTranslator::_translateAddC( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		ir::LLVMInstruction::Operand extendedA = _translate( i.a );
		ir::LLVMInstruction::Operand extendedB = _translate( i.b );
		
		extendedA.type = ir::LLVMInstruction::I64;
		extendedB.type = ir::LLVMInstruction::I64;
		
		if( ir::PTXOperand::isSigned( i.type ) )
		{
			ir::LLVMSext extend;
			
			extend.d = extendedA;
			extend.a = extendedA;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
			
			extend.d = extendedB;
			extend.a = extendedB;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
		}
		else
		{
			ir::LLVMZext extend;
			
			extend.d = extendedA;
			extend.a = extendedA;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
			
			extend.d = extendedB;
			extend.a = extendedB;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
		}
				
		ir::LLVMAdd add;
		
		add.d = extendedB;
		add.d.name = _tempRegister();
		add.a = extendedB;
		add.b = _conditionCodeRegister( i.c );
		
		_add( add );

		ir::LLVMAdd add1;
		
		add1.d = destination;
		add1.d.type.type = ir::LLVMInstruction::I64;
		add1.d.name = _tempRegister();
		add1.a = extendedA;
		add1.b = add.d;
		
		_add( add1 );

		ir::LLVMTrunc truncate;
		
		truncate.d = destination;
		truncate.a = add1.d;
		
		_add( truncate );
		_predicateEpilogue( i, truncate.d );
		
		if( i.modifier & ir::PTXInstruction::CC )
		{
			ir::LLVMLshr shift;
			
			shift.d = add1.d;
			shift.d.name = _tempRegister();
			shift.a = add1.d;
			shift.b.type.category = ir::LLVMInstruction::Type::Element;
			shift.b.type.type = ir::LLVMInstruction::I32;
			shift.b.constant = true;
			shift.b.i32 = 32;
			
			_add( shift );
		
			ir::LLVMAnd And;
			
			And.d = _destinationCC( i );
			And.a = shift.d;
			And.b.type.category = ir::LLVMInstruction::Type::Element;
			And.b.type.type = ir::LLVMInstruction::I64;
			And.b.constant = true;
			And.b.i64 = 1;
		
			_add( And );
		
			_predicateEpilogue( i, And.d );
		}
	}

	void PTXToLLVMTranslator::_translateAnd( const ir::PTXInstruction& i )
	{						
		ir::LLVMAnd And;
		
		And.d = _destination( i );
		And.a = _translate( i.a );
		And.b = _translate( i.b );

		_add( And );
		_predicateEpilogue( i, And.d );
	}

	void PTXToLLVMTranslator::_translateAtom( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@atom";
		
		call.d = _destination( i );

		call.parameters.resize( 4 );
		call.parameters[0].type.type = ir::LLVMInstruction::I32;
		call.parameters[0].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[0].i32 = i.addressSpace;

		call.parameters[1].type.type = ir::LLVMInstruction::I32;
		call.parameters[1].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[1].i32 = i.atomicOperation;


		call.parameters[2] = _translate( i.a );
		call.parameters[3] = _translate( i.b );
		
		if( i.atomicOperation == ir::PTXInstruction::AtomicCas )
		{
			call.parameters.resize( 5 );
			call.parameters[4] = _translate( i.c );
		}
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateBar( const ir::PTXInstruction& i )
	{
		_yield( _continuation++ );
	}

	void PTXToLLVMTranslator::_translateBra( const ir::PTXInstruction& i, 
		const analysis::DataflowGraph::Block& block )
	{
		ir::LLVMBr branch;
		if( ir::PTXOperand::PT != i.pg.condition 
			&& ir::PTXOperand::nPT != i.pg.condition )
		{
			branch.condition = _translate( i.pg );
		}
		else
		{
			branch.condition.type.category = ir::LLVMInstruction::Type::Element;
			branch.condition.type.type = ir::LLVMInstruction::I1;
			branch.condition.constant = true;
			
			if( ir::PTXOperand::PT == i.pg.condition )
			{
				branch.condition.i1 = true;
			}
			else
			{
				branch.condition.i1 = false;
			}
		}
		
		if( block.targets().empty() )
		{
			branch.iftrue = "%" + i.d.identifier;
		}
		else
		{
			branch.iftrue = "%" + (*block.targets().begin())->label();
		}
		branch.iffalse = "%" + block.fallthrough()->label();
		_add( branch );
	}

	void PTXToLLVMTranslator::_translateBrkpt( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@breakpoint";
		
		_add( call );
	}

	void PTXToLLVMTranslator::_translateCall( const ir::PTXInstruction& i )
	{
		assertM( false, "Opcode " 
			<< ir::PTXInstruction::toString( i.opcode ) 
			<< " not supported." );
	}

	void PTXToLLVMTranslator::_translateCNot( const ir::PTXInstruction& i )
	{
		ir::LLVMSelect select;
		select.d = _destination( i );
		select.condition = _translate( i.a );
		select.a = select.condition;
		select.a.constant = true;
		select.a.i64 = 0;
		select.b = select.a;		
		select.b.i64 = -1;
	
		_add( select );
		_predicateEpilogue( i, select.d );
	}

	void PTXToLLVMTranslator::_translateCos( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@cosFtz";
		}
		else
		{
			call.name = "@cos";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateCvt( const ir::PTXInstruction& i )
	{
		switch( i.a.type )
		{
			case ir::PTXOperand::s8:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::s64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMSext sext;
						sext.d = _destination( i );
						sext.a = _translate( i.a );
	
						_add( sext );
						_predicateEpilogue( i, sext.d );
						break;
					}
					case ir::PTXOperand::s8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::b8:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMSitofp sitofp;
						sitofp.d = _destination( i );
						sitofp.a = _translate( i.a );
						
						_add( sitofp );
						_predicateEpilogue( i, sitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::s16:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::s8:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
						
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					case ir::PTXOperand::s64:
					{
						ir::LLVMSext sext;
						sext.d = _destination( i );
						sext.a = _translate( i.a );
						
						_add( sext );
						_predicateEpilogue( i, sext.d );
						break;
					}
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::s16:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMSitofp sitofp;
						sitofp.d = _destination( i );
						sitofp.a = _translate( i.a );
						
						_add( sitofp );
						_predicateEpilogue( i, sitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::s32:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::s8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::s16:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::s32:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::s64:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMSext sext;
						sext.d = _destination( i );
						sext.a = _translate( i.a );
						
						_add( sext );
						_predicateEpilogue( i, sext.d );
						break;
					}
					case ir::PTXOperand::f16:
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMSitofp sitofp;
						sitofp.d = _destination( i );
						sitofp.a = _translate( i.a );
						
						_add( sitofp );
						_predicateEpilogue( i, sitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::s64:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::s8:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::s64:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMSitofp sitofp;
						sitofp.d = _destination( i );
						sitofp.a = _translate( i.a );
						
						_add( sitofp );
						_predicateEpilogue( i, sitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::pred:
			case ir::PTXOperand::b8:
			case ir::PTXOperand::u8:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::s64:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMZext zext;
						zext.d = _destination( i );
						zext.a = _translate( i.a );
						
						_add( zext );
						_predicateEpilogue( i, zext.d );
						break;
					}
					case ir::PTXOperand::s8:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMUitofp uitofp;
						uitofp.d = _destination( i );
						uitofp.a = _translate( i.a );
						
						_add( uitofp );
						_predicateEpilogue( i, uitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::b16:
			case ir::PTXOperand::u16:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::s8:
					case ir::PTXOperand::pred:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::s32:
					case ir::PTXOperand::s64:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMZext zext;
						zext.d = _destination( i );
						zext.a = _translate( i.a );
						
						_add( zext );
						_predicateEpilogue( i, zext.d );
						break;
					}
					case ir::PTXOperand::s16:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					{
						assertM( false, "F16 type not supported" );
						break;
					}
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMUitofp uitofp;
						uitofp.d = _destination( i );
						uitofp.a = _translate( i.a );
						
						_add( uitofp );
						_predicateEpilogue( i, uitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::b32:
			case ir::PTXOperand::u32:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::s8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::s16:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					case ir::PTXOperand::s64:
					{
						ir::LLVMZext zext;
						zext.d = _destination( i );
						zext.a = _translate( i.a );
						
						_add( zext );
						_predicateEpilogue( i, zext.d );
						break;
					}
					case ir::PTXOperand::b32:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::u32:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					{
						assertM( false, "F16 type not supported" );
						break;
					}
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMUitofp uitofp;
						uitofp.d = _destination( i );
						uitofp.a = _translate( i.a );
						
						_add( uitofp );
						_predicateEpilogue( i, uitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::b64:
			case ir::PTXOperand::u64:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::pred:
					case ir::PTXOperand::s8:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::s32:
					{
						ir::LLVMBitcast bitcast;
						bitcast.d = _destination( i );
						bitcast.a = _translate( i.a );
	
						_add( bitcast );
						_predicateEpilogue( i, bitcast.d );
						break;
					}
					case ir::PTXOperand::b64:
					case ir::PTXOperand::s64:
					case ir::PTXOperand::u64:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f16:
					{
						assertM( false, "F16 type not supported" );
						break;
					}
					case ir::PTXOperand::f64:
					case ir::PTXOperand::f32:
					{
						ir::LLVMUitofp uitofp;
						uitofp.d = _destination( i );
						uitofp.a = _translate( i.a );
						
						_add( uitofp );
						_predicateEpilogue( i, uitofp.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::f16:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::s8:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::s64:
					{
						ir::LLVMFptosi fptosi;
						fptosi.d = _destination( i );
						fptosi.a = _translate( i.a );
						
						_add( fptosi );
						_predicateEpilogue( i, fptosi.d );
						break;
					}
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMFptoui fptoui;
						fptoui.d = _destination( i );
						fptoui.a = _translate( i.a );
						
						_add( fptoui );
						_predicateEpilogue( i, fptoui.d );
						break;
					}
					case ir::PTXOperand::f16:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f32:
					case ir::PTXOperand::f64:
					{
						ir::LLVMFpext fpext;
						fpext.d = _destination( i );
						fpext.a = _translate( i.a );
						
						_add( fpext );
						_predicateEpilogue( i, fpext.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::f32:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::s8:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::s64:
					{
						ir::LLVMFptosi fptosi;
						fptosi.d = _destination( i );
						fptosi.a = _translate( i.a );
						
						_add( fptosi );
						_predicateEpilogue( i, fptosi.d );
						break;
					}
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMFptoui fptoui;
						fptoui.d = _destination( i );
						fptoui.a = _translate( i.a );
						
						_add( fptoui );
						_predicateEpilogue( i, fptoui.d );
						break;
					}
					case ir::PTXOperand::f16:
					{
						ir::LLVMFptrunc fptrunc;
						fptrunc.d = _destination( i );
						fptrunc.a = _translate( i.a );
						
						_add( fptrunc );
						_predicateEpilogue( i, fptrunc.d );
						break;
					}
					case ir::PTXOperand::f32:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::f64:
					{
						ir::LLVMFpext fpext;
						fpext.d = _destination( i );
						fpext.a = _translate( i.a );
						
						_add( fpext );
						_predicateEpilogue( i, fpext.d );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::f64:
			{
				switch( i.d.type )
				{
					case ir::PTXOperand::s8:
					case ir::PTXOperand::s16:
					case ir::PTXOperand::s32:
					case ir::PTXOperand::s64:
					{
						ir::LLVMFptosi fptosi;
						fptosi.d = _destination( i );
						fptosi.a = _translate( i.a );
						
						_add( fptosi );
						_predicateEpilogue( i, fptosi.d );
						break;
					}
					case ir::PTXOperand::pred:
					case ir::PTXOperand::b8:
					case ir::PTXOperand::u8:
					case ir::PTXOperand::b16:
					case ir::PTXOperand::u16:
					case ir::PTXOperand::b32:
					case ir::PTXOperand::u32:
					case ir::PTXOperand::b64:
					case ir::PTXOperand::u64:
					{
						ir::LLVMFptoui fptoui;
						fptoui.d = _destination( i );
						fptoui.a = _translate( i.a );
						
						_add( fptoui );
						_predicateEpilogue( i, fptoui.d );
						break;
					}
					case ir::PTXOperand::f16:
					{
						ir::LLVMFptrunc fptrunc;
						fptrunc.d = _destination( i );
						fptrunc.a = _translate( i.a );
						
						_add( fptrunc );
						_predicateEpilogue( i, fptrunc.d );
						break;
					}
					case ir::PTXOperand::f32:
					{
						ir::LLVMFptrunc fptrunc;
						fptrunc.d = _destination( i );
						fptrunc.a = _translate( i.a );
						
						_add( fptrunc );
						_predicateEpilogue( i, fptrunc.d );
						break;
					}
					case ir::PTXOperand::f64:
					{
						_bitcast( i );
						break;
					}
					case ir::PTXOperand::TypeSpecifier_invalid:
					{
						assertM( false, "Convert " << i.toString() 
							<< " not implemented." );
						break;
					}
				}
				break;
			}
			case ir::PTXOperand::TypeSpecifier_invalid:
			{
				assertM( false, "Convert " << i.toString() 
					<< " not implemented." );
				break;
			}
		}
	}

	void PTXToLLVMTranslator::_translateDiv( const ir::PTXInstruction& i )
	{
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFdiv div;
			
			div.d = _destination( i );
			div.a = _translate( i.a );
			div.b = _translate( i.b );
			
			_add( div );
			_predicateEpilogue( i, div.d );		
		}
		else if( ir::PTXOperand::isSigned( i.type ) )
		{
			ir::LLVMSdiv div;
			
			div.d = _destination( i );
			div.a = _translate( i.a );
			div.b = _translate( i.b );
			
			_add( div );
			_predicateEpilogue( i, div.d );		
		}
		else
		{
			ir::LLVMUdiv div;
			
			div.d = _destination( i );
			div.a = _translate( i.a );
			div.b = _translate( i.b );
			
			_add( div );
			_predicateEpilogue( i, div.d );				
		}
	}

	void PTXToLLVMTranslator::_translateEx2( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@ex2Ftz";
		}
		else
		{
			call.name = "@ex2";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateExit( const ir::PTXInstruction& i )
	{
		_yield( 0 );
	}

	void PTXToLLVMTranslator::_translateLd( const ir::PTXInstruction& i )
	{		
		ir::LLVMLoad load;
		
		if( i.d.vec != ir::PTXOperand::v1 )
		{
			load.d = _translate( i.d.array.front() );
			load.d.type.category = ir::LLVMInstruction::Type::Vector;
			load.d.type.vector = i.d.vec;
			load.d.name = _tempRegister();
		}
		else
		{
			load.d = _destination( i );
		}

		if( i.a.addressMode == ir::PTXOperand::Address )
		{
			load.a = _translate( i.a, i.addressSpace );
		}
		else
		{
			ir::LLVMInttoptr cast;
			
			cast.a = _translate( i.a );
			cast.d.type.members.push_back( load.d.type );
			cast.d.type.category = ir::LLVMInstruction::Type::Pointer;
			cast.d.name = _tempRegister();
			
			_add( cast );
			
			load.a = cast.d;
		}
				
		if( i.volatility == ir::PTXInstruction::Volatile )
		{
			load.isVolatile = true;
		}
		
		load.alignment = i.d.bytes();
				
		_add( load );
		
		for( ir::PTXOperand::Array::const_iterator 
			destination = i.d.array.begin(); 
			destination != i.d.array.end(); ++destination )
		{
			ir::LLVMInstruction::Operand target = _translate( *destination );
			
			if( i.pg.condition != ir::PTXOperand::PT )
			{
				target.name = _tempRegister();			
			}
			
			ir::LLVMExtractelement extract;
			
			extract.d = target;
			extract.d.type.type = load.d.type.type;
			extract.a = load.d;
			extract.b.type.type = ir::LLVMInstruction::I32;
			extract.b.type.category = ir::LLVMInstruction::Type::Element;
			extract.b.constant = true;
			extract.b.i32 = std::distance( i.d.array.begin(), destination );
			
			_add( extract );
			if( i.pg.condition == ir::PTXOperand::PT ) continue;
		
			ir::LLVMSelect select;
			select.condition = _translate( i.pg );
			select.d = _translate( *destination );
	
			if( i.pg.condition == ir::PTXOperand::nPT )
			{
				select.a = select.d;
				select.b = select.d;
			}
			else if( i.pg.condition == ir::PTXOperand::Pred )
			{
				select.a = extract.d;
				select.b = select.d;
			}
			else
			{
				select.a = select.d;
				select.b = extract.d;
			}
		
			_add( select );
		}
		
		if( i.d.vec == ir::PTXOperand::v1 )
		{
			_predicateEpilogue( i, load.d );
		}
	}

	void PTXToLLVMTranslator::_translateLg2( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@lg2Ftz";
		}
		else
		{
			call.name = "@lg2";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateMad24( const ir::PTXInstruction& i )
	{
		assertM( !( i.modifier & ir::PTXInstruction::sat ), 
			"No support for saturation in mad24" );
		assertM( !( i.modifier & ir::PTXInstruction::hi ), 
			"No support for hi multiply in mad24" );
		
		ir::LLVMInstruction::Operand destination = _destination( i );
		
		ir::LLVMMul multiply;
		
		multiply.d = destination;
		multiply.d.type.type = ir::LLVMInstruction::I64;
		multiply.d.name = _tempRegister();
		multiply.a = _translate( i.a );
		multiply.b = _translate( i.b );
		
		_add( multiply );
		
		ir::LLVMInstruction::Operand c = _translate( i.c );
		
		if( ir::PTXOperand::isSigned( i.c.type ) )
		{
			ir::LLVMSext extend;
			
			extend.d = c;
			extend.d.name = _tempRegister();
			extend.d.type = ir::LLVMInstruction::I64;
			extend.a = c;
			
			c = extend.d;
			_add( extend );
		}
		else
		{
			ir::LLVMZext extend;
			
			extend.d = c;
			extend.d.name = _tempRegister();
			extend.d.type = ir::LLVMInstruction::I64;
			extend.a = c;
			
			c = extend.d;
			_add( extend );
		}
		
		ir::LLVMAdd add;
		
		add.d = destination;
		add.d.name = _tempRegister();
		add.a = multiply.d;
		add.b = c;
		
		ir::LLVMTrunc truncate;
		
		truncate.d = destination;
		truncate.a = add.d;

		_add( truncate );
		_predicateEpilogue( i, truncate.d );		
	}

	void PTXToLLVMTranslator::_translateMad( const ir::PTXInstruction& i )
	{
		assertM( !( i.modifier & ir::PTXInstruction::sat ), 
			"No support for saturation in mad" );
		assertM( !( i.modifier & ir::PTXInstruction::hi ), 
			"No support for hi multiply in mad" );
		
		ir::LLVMInstruction::Operand destination = _destination( i );
		
		if( !ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMMul multiply;
		
			multiply.d = destination;
			multiply.d.type.type = ir::LLVMInstruction::I64;
			multiply.d.name = _tempRegister();
			multiply.a = _translate( i.a );
			multiply.b = _translate( i.b );
		
			_add( multiply );
		
			ir::LLVMInstruction::Operand c = _translate( i.c );
		
			if( ir::PTXOperand::isSigned( i.c.type ) )
			{
				ir::LLVMSext extend;
			
				extend.d = c;
				extend.d.name = _tempRegister();
				extend.d.type = ir::LLVMInstruction::I64;
				extend.a = c;
			
				c = extend.d;
				_add( extend );
			}
			else
			{
				ir::LLVMZext extend;
			
				extend.d = c;
				extend.d.name = _tempRegister();
				extend.d.type = ir::LLVMInstruction::I64;
				extend.a = c;
			
				c = extend.d;
				_add( extend );
			}
		
			ir::LLVMAdd add;
		
			add.d = destination;
			add.d.name = _tempRegister();
			add.a = multiply.d;
			add.b = c;
		
			_add( add );
		
			ir::LLVMTrunc truncate;
		
			truncate.d = destination;
			truncate.d.name = _tempRegister();
			truncate.a = add.d;

			_add( truncate );
			_predicateEpilogue( i, truncate.d );
		}
		else
		{
			ir::LLVMFpext extend;
			
			extend.a = _translate( i.a );
			
			ir::LLVMInstruction::Operand extendedA;
			
			extendedA = extend.a;
			extendedA.name = _tempRegister();
			_doubleWidth( extendedA.type.type );
			
			extend.d = extendedA;
			
			_add( extend );
			
			extend.a = _translate( i.b );
			
			ir::LLVMInstruction::Operand extendedB;
			
			extendedB = extend.a;
			extendedB.name = _tempRegister();
			_doubleWidth( extendedB.type.type );
			
			extend.d = extendedB;
			
			_add( extend );
			
			ir::LLVMFmul multiply;
		
			multiply.d = destination;
			multiply.d.type.type = extendedA.type.type;
			multiply.d.name = _tempRegister();
			multiply.a = extendedA;
			multiply.b = extendedB;
		
			_add( multiply );
		
			extend.a = _translate( i.c );
			
			ir::LLVMInstruction::Operand extendedC;
			
			extendedC = extend.a;
			extendedC.name = _tempRegister();
			_doubleWidth( extendedC.type.type );
			
			extend.d = extendedC;
			
			_add( extend );
		
			ir::LLVMFadd add;
		
			add.d = destination;
			add.d.name = _tempRegister();
			add.d.type = extendedC.type;
			add.a = multiply.d;
			add.b = extendedC;
		
			_add( add );
		
			ir::LLVMFptrunc truncate;
		
			truncate.d = destination;
			truncate.a = add.d;

			_add( truncate );
			_predicateEpilogue( i, truncate.d );
		}		
	}

	void PTXToLLVMTranslator::_translateMax( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		ir::LLVMInstruction::Operand comparison;
		
		comparison.type.category = ir::LLVMInstruction::Type::Element;
		comparison.type.type = ir::LLVMInstruction::I1;
		comparison.name = _tempRegister();
		
		if( ir::PTXOperand::isFloat( i.a.type ) )
		{
			ir::LLVMFcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			compare.comparison = ir::LLVMInstruction::Ogt;
			
			ir::LLVMSelect select; 
			
			select.condition = compare.d;
			select.a = compare.a;
			select.b = compare.b;
			select.d = destination;
			
			_add( compare );
			_add( select );
		}
		else
		{
			ir::LLVMIcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			
			if( ir::PTXOperand::isSigned( i.a.type ) )
			{
				compare.comparison = ir::LLVMInstruction::Sgt;
			}
			else
			{
				compare.comparison = ir::LLVMInstruction::Ugt;
			}
			
			ir::LLVMSelect select; 
			
			select.condition = compare.d;
			select.a = compare.a;
			select.b = compare.b;
			select.d = destination;
			
			_add( compare );
			_add( select );
		}
		
		_predicateEpilogue( i, destination );
	}

	void PTXToLLVMTranslator::_translateMembar( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.level == ir::PTXInstruction::CtaLevel )
		{
			call.name = "@membarCta";
		}
		else
		{
			call.name = "@membarGlobal";
		}
		
		_add( call );
	}

	void PTXToLLVMTranslator::_translateMin( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		ir::LLVMInstruction::Operand comparison;
		
		comparison.type.category = ir::LLVMInstruction::Type::Element;
		comparison.type.type = ir::LLVMInstruction::I1;
		comparison.name = _tempRegister();
		
		if( ir::PTXOperand::isFloat( i.a.type ) )
		{
			ir::LLVMFcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			compare.comparison = ir::LLVMInstruction::Olt;
			
			ir::LLVMSelect select; 
			
			select.condition = compare.d;
			select.a = compare.a;
			select.b = compare.b;
			select.d = destination;
			
			_add( compare );
			_add( select );
		}
		else
		{
			ir::LLVMIcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			
			if( ir::PTXOperand::isSigned( i.a.type ) )
			{
				compare.comparison = ir::LLVMInstruction::Slt;
			}
			else
			{
				compare.comparison = ir::LLVMInstruction::Ult;
			}
			
			ir::LLVMSelect select; 
			
			select.condition = compare.d;
			select.a = compare.a;
			select.b = compare.b;
			select.d = destination;
			
			_add( compare );
			_add( select );
		}
		
		_predicateEpilogue( i, destination );
	}

	void PTXToLLVMTranslator::_translateMov( const ir::PTXInstruction& i )
	{
		assertM( i.vec == ir::PTXOperand::v1, "No support for vector moves" );		
		
		if( i.a.addressMode == ir::PTXOperand::Address )
		{
			ir::LLVMPtrtoint cast;
			
			cast.d = _destination( i );
			cast.a = _translate( i.a, i.addressSpace );
			
			_add( cast );
			_predicateEpilogue( i, cast.d );
		}
		else if( i.d.type == i.a.type )
		{
			_bitcast( i );
		}
		else
		{
			_translateCvt( i );
		}
	}

	void PTXToLLVMTranslator::_translateMul24( const ir::PTXInstruction& i )
	{
		if( i.modifier & ir::PTXInstruction::lo )
		{
			// 24-bit lo is the same as 32-bit lo
			ir::LLVMMul mul;
					
			mul.d = _destination( i );
			mul.a = _translate( i.a );
			mul.b = _translate( i.b );
		
			_add( mul );
			_predicateEpilogue( i, mul.d );		
		}
		else
		{
			assertM( false, "No support for hi multiply" );
		}	
	}

	void PTXToLLVMTranslator::_translateMul( const ir::PTXInstruction& i )
	{
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFmul mul;
			
			_setFloatingPointRoundingMode( i );
			
			mul.d = _destination( i );
			mul.a = _translate( i.a );
			mul.b = _translate( i.b );

			if( i.modifier & ir::PTXInstruction::sat )
			{
				assertM( false, "Saturate not supported for " << i.toString() );
			}

			_add( mul );
			_predicateEpilogue( i, mul.d );
		}
		else
		{
			if( i.modifier & ir::PTXInstruction::wide )
			{
				ir::LLVMInstruction::Operand extendedA = _translate( i.a );
				ir::LLVMInstruction::Operand extendedB = _translate( i.b );
				
				if( ir::PTXOperand::isSigned( i.a.type ) )
				{
					ir::LLVMSext sextA;
					
					sextA.a = extendedA;
					_doubleWidth( extendedA.type.type );
					extendedA.name = _tempRegister();
					sextA.d = extendedA;
					
					_add( sextA );
				
					ir::LLVMSext sextB;
					
					sextB.a = extendedB;
					_doubleWidth( extendedB.type.type );
					extendedB.name = _tempRegister();
					sextB.d = extendedB;
					
					_add( sextB );
				}
				else
				{
					ir::LLVMZext sextA;
					
					sextA.a = extendedA;
					_doubleWidth( extendedA.type.type );
					extendedA.name = _tempRegister();
					sextA.d = extendedA;
					
					_add( sextA );
				
					ir::LLVMZext sextB;
					
					sextB.a = extendedB;
					_doubleWidth( extendedB.type.type );
					extendedB.name = _tempRegister();
					sextB.d = extendedB;
					
					_add( sextB );
				}
				
				ir::LLVMMul mul;
						
				mul.d = _destination( i );
				mul.a = extendedA;
				mul.b = extendedB;
			
				_add( mul );
				_predicateEpilogue( i, mul.d );
			}
			else if( i.modifier & ir::PTXInstruction::lo )
			{
				ir::LLVMMul mul;
						
				mul.d = _destination( i );
				mul.a = _translate( i.a );
				mul.b = _translate( i.b );
			
				_add( mul );
				_predicateEpilogue( i, mul.d );		
			}
			else
			{
				ir::LLVMInstruction::Operand destination = _destination( i );
				ir::LLVMInstruction::Operand extendedA = _translate( i.a );
				ir::LLVMInstruction::Operand extendedB = _translate( i.b );
				
				if( ir::PTXOperand::isSigned( i.a.type ) )
				{
					ir::LLVMSext sextA;
					
					sextA.a = extendedA;
					_doubleWidth( extendedA.type.type );
					extendedA.name = _tempRegister();
					sextA.d = extendedA;
					
					_add( sextA );
				
					ir::LLVMSext sextB;
					
					sextB.a = extendedB;
					_doubleWidth( extendedB.type.type );
					extendedB.name = _tempRegister();
					sextB.d = extendedB;
					
					_add( sextB );
				}
				else
				{
					ir::LLVMZext sextA;
					
					sextA.a = extendedA;
					_doubleWidth( extendedA.type.type );
					extendedA.name = _tempRegister();
					sextA.d = extendedA;
					
					_add( sextA );
				
					ir::LLVMZext sextB;
					
					sextB.a = extendedB;
					_doubleWidth( extendedB.type.type );
					extendedB.name = _tempRegister();
					sextB.d = extendedB;
					
					_add( sextB );
				}
				
				ir::LLVMMul mul;
						
				mul.d = extendedA;
				mul.d.name = _tempRegister();
				mul.a = extendedA;
				mul.b = extendedB;
			
				_add( mul );
				
				ir::LLVMInstruction::Operand shiftedDestination = destination;
				shiftedDestination.name = _tempRegister();
				_doubleWidth( shiftedDestination.type.type );
				
				if( ir::PTXOperand::isSigned( i.a.type ) )
				{
					ir::LLVMAshr shift;
					
					shift.d = shiftedDestination;
					shift.a = mul.d;
					shift.b.constant = true;
					shift.b.type.category = ir::LLVMInstruction::Type::Element;
					shift.b.type.type = ir::LLVMInstruction::I32;
					shift.b.i32 = ir::PTXOperand::bytes( i.a.type ) * 8;
					
					_add( shift );
				
				}
				else
				{
					ir::LLVMLshr shift;
					
					shift.d = shiftedDestination;
					shift.a = mul.d;
					shift.b.constant = true;
					shift.b.type.category = ir::LLVMInstruction::Type::Element;
					shift.b.type.type = ir::LLVMInstruction::I32;
					shift.b.i32 = ir::PTXOperand::bytes( i.a.type ) * 8;
					
					_add( shift );
				}
				
				ir::LLVMTrunc truncate;
				
				truncate.d = destination;
				truncate.a = shiftedDestination;
				
				_add( truncate );
				
				_predicateEpilogue( i, destination );
			}
		}
	}

	void PTXToLLVMTranslator::_translateNeg( const ir::PTXInstruction& i )
	{
		ir::LLVMSub sub;
		
		sub.d = _destination( i );
		sub.b = _translate( i.a );
		sub.a = sub.b;
		sub.a.constant = true;
		sub.a.i64 = 0;
		
		_add( sub );
		_predicateEpilogue( i, sub.d );
	}

	void PTXToLLVMTranslator::_translateNot( const ir::PTXInstruction& i )
	{
		ir::LLVMXor Not;
		
		Not.d = _destination( i );
		Not.a = _translate( i.a );
		Not.b = Not.a;
		Not.b.constant = true;
		Not.b.i64 = -1;
		
		_add( Not );
		_predicateEpilogue( i, Not.d );
	}

	void PTXToLLVMTranslator::_translateOr( const ir::PTXInstruction& i )
	{
		ir::LLVMOr Or;
		
		Or.d = _destination( i );
		Or.a = _translate( i.a );
		Or.b = _translate( i.b );

		_add( Or );
		_predicateEpilogue( i, Or.d );
	}

	void PTXToLLVMTranslator::_translatePmevent( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@pmevent";
		
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
	}

	void PTXToLLVMTranslator::_translateRcp( const ir::PTXInstruction& i )
	{
		ir::LLVMFdiv div;
		
		div.d = _destination( i );
		div.b = _translate( i.a );
		div.a = div.b;
		div.a.constant = true;
		
		if( i.a.type == ir::PTXOperand::f32 )
		{
			div.a.f32 = 1.0;
		}
		else
		{
			div.a.f64 = 1.0;
		}
		
		_add( div );
		_predicateEpilogue( i, div.d );
	}

	void PTXToLLVMTranslator::_translateRed( const ir::PTXInstruction& i )
	{
		assertM( i.pg.condition == ir::PTXOperand::PT, 
			"Predicated red instructions not supported." );
	
		ir::LLVMCall call;
		
		call.name = "@reduction";
		
		call.parameters.resize( 4 );
		call.parameters[0].type.type = ir::LLVMInstruction::I32;
		call.parameters[0].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[0].i32 = i.addressSpace;

		call.parameters[1].type.type = ir::LLVMInstruction::I32;
		call.parameters[1].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[1].i32 = i.reductionOperation;


		call.parameters[2] = _translate( i.a );
		call.parameters[3] = _translate( i.b );
		
		_add( call );
	}

	void PTXToLLVMTranslator::_translateRem( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		
		if( ir::PTXOperand::isSigned( i.type ) )
		{
			ir::LLVMSrem rem;
			
			rem.d = destination;
			rem.a = _translate( i.a );
			rem.b = _translate( i.b );
			
			_add( rem );
		}
		else
		{
			ir::LLVMUrem rem;
			
			rem.d = destination;
			rem.a = _translate( i.a );
			rem.b = _translate( i.b );
			
			_add( rem );			
		}
		_predicateEpilogue( i, destination );
	}

	void PTXToLLVMTranslator::_translateRet( const ir::PTXInstruction& i )
	{
		assertM( false, "Return not supported" );
	}

	void PTXToLLVMTranslator::_translateRsqrt( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@rsqrtFtz";
		}
		else
		{
			call.name = "@rsqrt";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateSad( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination = _destination( i );
		
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFcmp compare;
			
			compare.d.name = _tempRegister();
			compare.d.type.category = ir::LLVMInstruction::Type::Element;
			compare.d.type.type = ir::LLVMInstruction::I1;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			compare.comparison = ir::LLVMInstruction::Olt;
			
			_add( compare );
			
			ir::LLVMSub subtract;
			
			subtract.d = destination;
			subtract.d.name = _tempRegister();
			subtract.a = compare.b;
			subtract.b = compare.a;
			
			_add( subtract );

			ir::LLVMSub subtract1;
			
			subtract1.d = destination;
			subtract1.d.name = _tempRegister();
			subtract1.a = compare.a;
			subtract1.b = compare.b;
			
			_add( subtract1 );
			
			ir::LLVMSelect select;
			
			select.condition = compare.d;
			select.d = destination;
			select.d.name = _tempRegister();
			select.a = subtract.d;
			select.b = subtract1.d;
			
			_add( select );
			
			ir::LLVMFadd add;
			
			add.d = destination;
			add.a = _translate( i.c );
			add.b = select.d;
			
			_add( add );
		}
		else
		{
			ir::LLVMIcmp compare;
			
			compare.d.name = _tempRegister();
			compare.d.type.category = ir::LLVMInstruction::Type::Element;
			compare.d.type.type = ir::LLVMInstruction::I1;
			compare.a = _translate( i.a );
			compare.b = _translate( i.b );
			
			if( ir::PTXOperand::isSigned( i.type ) )
			{
				compare.comparison = ir::LLVMInstruction::Slt;
			}
			else
			{
				compare.comparison = ir::LLVMInstruction::Ult;			
			}
			
			_add( compare );
			
			ir::LLVMSub subtract;
			
			subtract.d = destination;
			subtract.d.name = _tempRegister();
			subtract.a = compare.b;
			subtract.b = compare.a;
			
			_add( subtract );

			ir::LLVMSub subtract1;
			
			subtract1.d = destination;
			subtract1.d.name = _tempRegister();
			subtract1.a = compare.a;
			subtract1.b = compare.b;
			
			_add( subtract1 );
			
			ir::LLVMSelect select;
			
			select.condition = compare.d;
			select.d = destination;
			select.d.name = _tempRegister();
			select.a = subtract.d;
			select.b = subtract1.d;
			
			_add( select );
			
			ir::LLVMAdd add;
			
			add.d = destination;
			add.a = _translate( i.c );
			add.b = select.d;
			
			_add( add );		
		}
		_predicateEpilogue( i, destination );
	}

	void PTXToLLVMTranslator::_translateSelP( const ir::PTXInstruction& i )
	{
		ir::LLVMSelect select;
		
		select.d = _destination( i );
		select.a = _translate( i.a );
		select.b = _translate( i.b );
		select.condition = _translate( i.c );
		
		_add( select );
		_predicateEpilogue( i, select.d );
	}

	void PTXToLLVMTranslator::_translateSet( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand d = _destination( i );

		ir::LLVMInstruction::Operand comparison; 
		comparison.name = _tempRegister();
		comparison.type.category = ir::LLVMInstruction::Type::Element;
		comparison.type.type = ir::LLVMInstruction::I1;

		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFcmp fcmp;
			
			fcmp.d = comparison;
			fcmp.a = _translate( i.a );
			fcmp.b = _translate( i.b );
			fcmp.comparison = _translate( i.comparisonOperator, false );
			
			_add( fcmp );
		}
		else
		{
			ir::LLVMIcmp icmp;
					
			icmp.d = comparison;
			icmp.a = _translate( i.a );
			icmp.b = _translate( i.b );
			icmp.comparison = _translate( i.comparisonOperator, false );
			
			_add( icmp );		
		}
		
		if( i.c.addressMode == ir::PTXOperand::Register )
		{
			ir::LLVMInstruction::Operand c = _translate( i.c );
		
			if( i.c.condition == ir::PTXOperand::InvPred )
			{
				ir::LLVMXor Not;
				
				Not.d = c;
				Not.d.name = _tempRegister();
				Not.a = c;
				Not.b = c;
				Not.b.constant = true;
				Not.b.i1 = 1;
			
				_add( Not );

				c = Not.d;
			}
					
			switch( i.booleanOperator )
			{
				case ir::PTXInstruction::BoolAnd:
				{
					ir::LLVMAnd And;
					
					And.d = comparison;
					And.d.name = _tempRegister();
					And.a = c;
					And.b = comparison;
					
					_add( And );
					
					comparison.name = And.d.name;
					
					break;
				}
				case ir::PTXInstruction::BoolOr:
				{
					ir::LLVMOr Or;
					
					Or.d = comparison;
					Or.d.name = _tempRegister();
					Or.a = c;
					Or.b = comparison;
					
					_add( Or );
					
					comparison.name = Or.d.name;
					
					break;
				}
				case ir::PTXInstruction::BoolXor:
				{
					ir::LLVMXor Xor;
					
					Xor.d = d;
					Xor.d.name = _tempRegister();
					Xor.a = c;
					Xor.b = comparison;
					
					_add( Xor );
					
					comparison.name = Xor.d.name;

					break;
				}
				default:
				{
					break;
				}
			}
		}

		ir::LLVMSelect select;
		
		select.condition = comparison;
		select.d = d;
		select.a = d;
		select.b = d;
		select.a.constant = true;
		select.b.constant = true;
		
		if( ir::PTXOperand::f64 == i.type )
		{
			select.a.f64 = 1.0;
			select.b.f64 = 0.0;
		}
		else if( ir::PTXOperand::f32 == i.type )
		{
			select.a.f32 = 1.0;
			select.b.f32 = 0.0;		
		}
		else
		{
			select.a.i64 = -1;
			select.b.f64 = 0;		
		}
		
		_add( select );
		_predicateEpilogue( i, d );
	}

	void PTXToLLVMTranslator::_translateSetP( const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand d = _destination( i );
		ir::LLVMInstruction::Operand tempD;

		if( i.c.addressMode == ir::PTXOperand::Register )
		{
			tempD = _destination( i, true );
		}
		else
		{
			tempD = d;
		}

		if( ir::PTXOperand::isFloat( i.a.type ) )
		{
			ir::LLVMFcmp fcmp;
			
			fcmp.d = tempD;
			fcmp.a = _translate( i.a );
			fcmp.b = _translate( i.b );
			fcmp.comparison = _translate( i.comparisonOperator, false );
			
			_add( fcmp );
		}
		else
		{
			ir::LLVMIcmp icmp;
					
			icmp.d = tempD;
			icmp.a = _translate( i.a );
			icmp.b = _translate( i.b );
			icmp.comparison = _translate( i.comparisonOperator, true );
			
			_add( icmp );		
		}
		
		ir::LLVMInstruction::Operand pd = d;
		ir::LLVMInstruction::Operand pq;
		
		if( i.pq.addressMode != ir::PTXOperand::Invalid )
		{
			pq = _translate( i.pq );
		}
		
		if( i.c.addressMode == ir::PTXOperand::Register )
		{
			ir::LLVMXor Not;

			if( i.pq.addressMode != ir::PTXOperand::Invalid )
			{
				Not.d = tempD;
				Not.d.name = _tempRegister();
				Not.a = tempD;
				Not.b.type.category = ir::LLVMInstruction::Type::Element;
				Not.b.type.type = ir::LLVMInstruction::I1;
				Not.b.constant = true;
				Not.b.i1 = true;
			
				_add( Not );
			}
					
			switch( i.booleanOperator )
			{
				case ir::PTXInstruction::BoolAnd:
				{
					ir::LLVMAnd And;
					
					And.d = pd;
					And.a = _translate( i.c );
					And.b = tempD;
					
					_add( And );
					
					if( i.pq.addressMode != ir::PTXOperand::Invalid )
					{
						And.d = pq;
						And.b = Not.d;
					
						_add( And );
					}
					break;
				}
				case ir::PTXInstruction::BoolOr:
				{
					ir::LLVMOr Or;
					
					Or.d = pd;
					Or.a = _translate( i.c );
					Or.b = tempD;
					
					_add( Or );
					
					if( i.pq.addressMode != ir::PTXOperand::Invalid )
					{
						Or.d = pq;
						Or.b = Not.d;
					
						_add( Or );
					}
					break;
				}
				case ir::PTXInstruction::BoolXor:
				{
					ir::LLVMXor Xor;
					
					Xor.d = pd;
					Xor.a = _translate( i.c );
					Xor.b = tempD;
					
					_add( Xor );

					if( i.pq.addressMode != ir::PTXOperand::Invalid )
					{
						Xor.d = pq;
						Xor.b = Not.d;
					
						_add( Xor );
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
		
		_predicateEpilogue( i, d );
		if( i.pq.addressMode != ir::PTXOperand::Invalid )
		{
			_predicateEpilogue( i, tempD );
		}
	}

	void PTXToLLVMTranslator::_translateShl( const ir::PTXInstruction& i )
	{
		ir::LLVMShl shift;
		
		shift.d = _destination( i );
		shift.a = _translate( i.a );
		shift.b = _translate( i.b );
		
		_add( shift );
		_predicateEpilogue( i, shift.d );
	}

	void PTXToLLVMTranslator::_translateShr( const ir::PTXInstruction& i )
	{
		if( ir::PTXOperand::isSigned( i.type ) )
		{
			ir::LLVMLshr shift;
			
			shift.d = _destination( i );
			shift.a = _translate( i.a );
			shift.b = _translate( i.b );
			
			_add( shift );
			_predicateEpilogue( i, shift.d );
		}
		else
		{
			ir::LLVMAshr shift;
			
			shift.d = _destination( i );
			shift.a = _translate( i.a );
			shift.b = _translate( i.b );
			
			_add( shift );
			_predicateEpilogue( i, shift.d );		
		}
	}

	void PTXToLLVMTranslator::_translateSin( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@sinFtz";
		}
		else
		{
			call.name = "@sin";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateSlCt( const ir::PTXInstruction& i )
	{
	
		ir::LLVMInstruction::Operand comparison;
		
		comparison.type.category = ir::LLVMInstruction::Type::Element;
		comparison.type.type = ir::LLVMInstruction::I1;
		comparison.name = _tempRegister();
	
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.c );
			compare.b = compare.a;
			compare.b.constant = true;
			compare.b.f32 = 0;
			compare.comparison = ir::LLVMInstruction::Oge;
			
			_add( compare );
		}
		else
		{
			ir::LLVMIcmp compare;
			
			compare.d = comparison;
			compare.a = _translate( i.c );
			compare.b = compare.a;
			compare.b.constant = true;
			compare.b.i32 = 0;
			compare.comparison = ir::LLVMInstruction::Oge;
			
			_add( compare );		
		}
		
		ir::LLVMSelect select;
		select.d = _destination( i );
		select.condition = comparison;
		select.a = _translate( i.a );
		select.b = _translate( i.b );
		
		_add( select );
		_predicateEpilogue( i, select.d );
	}

	void PTXToLLVMTranslator::_translateSqrt( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		if( i.modifier & ir::PTXInstruction::ftz )
		{
			call.name = "@sqrtFtz";
		}
		else
		{
			call.name = "@sqrt";
		}
		
		call.d = _destination( i );
		call.parameters.resize( 1 );
		call.parameters[0] = _translate( i.a );
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateSt( const ir::PTXInstruction& i )
	{
		assertM( i.pg.condition == ir::PTXOperand::PT, 
			"Predicated store instructions not supported." );
		
		ir::LLVMStore store;

		store.d = _translate( i.d );
		
		if( i.vec == ir::PTXOperand::v1 )
		{
			store.a = _translate( i.a );
		}
		else
		{
			store.a = _translate( i.a.array.front() );
			store.a.type.vector = i.vec;
			store.a.type.category = ir::LLVMInstruction::Type::Vector;
		}

		ir::LLVMInttoptr inttoptr;
		
		if( i.d.offset != 0 )
		{
			ir::LLVMAdd add;
		
			add.a = store.d;
			add.b = add.a;
			add.b.constant = true;
			add.b.i64 = i.d.offset;
			add.d = add.a;
			add.d.name = _tempRegister();
	
			_add( add );
			
			inttoptr.a = add.d;
		}
		else
		{
			inttoptr.a = store.d;
		}		

		inttoptr.d = store.a;
		inttoptr.d.type.category = ir::LLVMInstruction::Type::Pointer;
		inttoptr.d.name = _tempRegister();		

		_add( inttoptr );
						
		if( i.volatility == ir::PTXInstruction::Volatile )
		{
			store.isVolatile = true;
		}
		
		store.d = inttoptr.d;
		store.alignment = i.a.bytes();

		if( i.vec == ir::PTXOperand::v1 )
		{
			store.a = _translate( i.a );
		}
		else
		{
			store.a = _translate( i.a.array.front() );
			store.a.type.vector = i.vec;
			store.a.type.category = ir::LLVMInstruction::Type::Vector;

			ir::PTXOperand::Array::const_iterator 
				source = i.a.array.begin();

			ir::LLVMInsertelement insertOne;
		
			insertOne.d = store.a;
			insertOne.d.name = _tempRegister();
			insertOne.a = store.a;
			insertOne.b = _translate( *source );
			insertOne.c.type.category = ir::LLVMInstruction::Type::Element;
			insertOne.c.type.type = ir::LLVMInstruction::I32;
			insertOne.c.constant = true;
			insertOne.c.i32 = 0;
			
			_add( insertOne );
			
			std::string currentSource = insertOne.d.name;
			
			for( ++source; source != i.a.array.end(); ++source )
			{
				ir::LLVMInsertelement insert;

				insert.d = store.a;
				if( ++ir::PTXOperand::Array::const_iterator( source ) 
					!= i.a.array.end() )
				{
					insert.d.name = _tempRegister();
				}
				insert.a = store.a;
				insert.a.name = currentSource;
				insert.b = _translate( *source );
				insert.c.type.category = ir::LLVMInstruction::Type::Element;
				insert.c.type.type = ir::LLVMInstruction::I32;
				insert.c.constant = true;
				insert.c.i32 = std::distance( i.a.array.begin(), source );
				
				_add( insert );
				currentSource = insert.d.name;
			}
		}
		
		_add( store );
	}

	void PTXToLLVMTranslator::_translateSub( const ir::PTXInstruction& i )
	{
		if( ir::PTXOperand::isFloat( i.type ) )
		{
			ir::LLVMFsub sub;
		
			sub.d = _destination( i );
			sub.a = _translate( i.a );
			sub.b = _translate( i.b );
		
			_add( sub );
			
			if( i.modifier & ir::PTXInstruction::sat )
			{
				ir::LLVMFcmp compare;
				
				compare.d.name = _tempRegister();
				compare.d.type.type = ir::LLVMInstruction::I1;
				compare.d.type.category = ir::LLVMInstruction::Type::Element;
				compare.comparison = ir::LLVMInstruction::Ule;
				compare.a = sub.d;
				compare.b.type.type = compare.a.type.type;
				compare.b.type.category = ir::LLVMInstruction::Type::Element;
				compare.b.constant = true;
				if( compare.b.type.type == ir::LLVMInstruction::F32 )
				{
					compare.b.f32 = 0;
				}
				else
				{
					compare.b.f64 = 0;
				}
				
				ir::LLVMSelect select;
				
				select.d.name = _tempRegister();
				select.d.type.type = sub.d.type.type;
				select.d.type.category = sub.d.type.category;
				select.condition = compare.d;
				select.a = compare.b;
				select.b = sub.d;
				
				_add( compare );
				_add( select );
				_predicateEpilogue( i, select.d );
			}
			else
			{
				_predicateEpilogue( i, sub.d );
			}
		}
		else
		{
			assertM( !(i.modifier & ir::PTXInstruction::sat), 
				"Saturation for ptx int sub not supported." );
		
			ir::LLVMSub sub;
			
			sub.d = _destination( i );
			sub.a = _translate( i.a );
			sub.b = _translate( i.b );
		
			_add( sub );
			_predicateEpilogue( i, sub.d );
		}
	}

	void PTXToLLVMTranslator::_translateSubC( const ir::PTXInstruction& i )
	{	
		ir::LLVMInstruction::Operand destination = _destination( i );
		ir::LLVMInstruction::Operand extendedA = _translate( i.a );
		ir::LLVMInstruction::Operand extendedB = _translate( i.b );
		
		extendedA.type = ir::LLVMInstruction::I64;
		extendedB.type = ir::LLVMInstruction::I64;
		
		if( ir::PTXOperand::isSigned( i.type ) )
		{
			ir::LLVMSext extend;
			
			extend.d = extendedA;
			extend.a = extendedA;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
			
			extend.d = extendedB;
			extend.a = extendedB;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
		}
		else
		{
			ir::LLVMZext extend;
			
			extend.d = extendedA;
			extend.a = extendedA;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
			
			extend.d = extendedB;
			extend.a = extendedB;
			extend.a.type = ir::LLVMInstruction::I32;
			
			_add( extend );
		}
				
		ir::LLVMAdd add;
		
		add.d = extendedB;
		add.d.name = _tempRegister();
		add.a = extendedB;
		add.b = _conditionCodeRegister( i.c );
		
		_add( add );

		ir::LLVMSub sub;
		
		sub.d = destination;
		sub.d.type.type = ir::LLVMInstruction::I64;
		sub.d.name = _tempRegister();
		sub.a = extendedA;
		sub.b = add.d;
		
		_add( sub );

		ir::LLVMTrunc truncate;
		
		truncate.d = destination;
		truncate.a = sub.d;
		
		_add( truncate );
		_predicateEpilogue( i, truncate.d );
		
		if( i.modifier & ir::PTXInstruction::CC )
		{
			ir::LLVMLshr shift;
			
			shift.d = sub.d;
			shift.d.name = _tempRegister();
			shift.a = sub.d;
			shift.b.type.category = ir::LLVMInstruction::Type::Element;
			shift.b.type.type = ir::LLVMInstruction::I32;
			shift.b.constant = true;
			shift.b.i32 = 32;
			
			_add( shift );
		
			ir::LLVMAnd And;
			
			And.d = _destinationCC( i );
			And.a = shift.d;
			And.b.type.category = ir::LLVMInstruction::Type::Element;
			And.b.type.type = ir::LLVMInstruction::I64;
			And.b.constant = true;
			And.b.i64 = 1;
		
			_add( And );
		
			_predicateEpilogue( i, And.d );
		}
	}

	void PTXToLLVMTranslator::_translateTex( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@tex";
				
		ir::LLVMInstruction::Operand d1 = _translate( i.d.array[0] );
		ir::LLVMInstruction::Operand d2 = _translate( i.d.array[1] );
		ir::LLVMInstruction::Operand d3 = _translate( i.d.array[2] );
		ir::LLVMInstruction::Operand d4 = _translate( i.d.array[3] );
		
		if( i.pg.condition != ir::PTXOperand::PT )
		{
			d1.name = _tempRegister();			
			d2.name = _tempRegister();			
			d3.name = _tempRegister();			
			d4.name = _tempRegister();			
		}
		
		call.d = d1;
		call.d.type.category = ir::LLVMInstruction::Type::Vector;
		call.d.type.vector = 4;
		
		switch( i.geometry )
		{
			case ir::PTXInstruction::_1d:
			{
				call.parameters.resize( 2 );
				call.parameters[0] = _translate( i.a );
				call.parameters[1] = _translate( i.c );
				break;
			}
			case ir::PTXInstruction::_2d:
				call.parameters.resize( 3 );
				call.parameters[0] = _translate( i.a );
				call.parameters[1] = _translate( i.c.array[0] );
				call.parameters[2] = _translate( i.c.array[1] );
				break;
			case ir::PTXInstruction::_3d:
				call.parameters.resize( 5 );
				call.parameters[0] = _translate( i.a );
				call.parameters[1] = _translate( i.c.array[0] );
				call.parameters[2] = _translate( i.c.array[1] );
				call.parameters[3] = _translate( i.c.array[2] );
				call.parameters[4] = _translate( i.c.array[3] );
				break;
			default: assertM( false, "Invalid texture geometry" );
		}
		
		_add( call );
		
		ir::LLVMExtractelement extract;
			
		extract.d = d1;
		extract.a = call.d;
		extract.b.type.type = ir::LLVMInstruction::I32;
		extract.b.type.category = ir::LLVMInstruction::Type::Element;
		extract.b.constant = true;
		extract.b.i32 = 0;
	
		_add( extract );
	
		extract.d = d2;
		extract.b.i32 = 1;
	
		_add( extract );
		
		extract.d = d3;
		extract.b.i32 = 2;
	
		_add( extract );

		extract.d = d4;
		extract.b.i32 = 3;
	
		_add( extract );
		
		_predicateEpilogue( i, d1 );
		_predicateEpilogue( i, d2 );
		_predicateEpilogue( i, d3 );
		_predicateEpilogue( i, d4 );
	}

	void PTXToLLVMTranslator::_translateTrap( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@trap";
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateVote( const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@vote";
		
		call.d = _destination( i );
		call.parameters.resize( 3 );
		call.parameters[0] = _translate( i.a );
		call.parameters[1].type.type = ir::LLVMInstruction::I32;
		call.parameters[1].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[1].i32 = i.vote;
		call.parameters[2].type.type = ir::LLVMInstruction::I1;
		call.parameters[2].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[2].i1 = i.b.condition == ir::PTXOperand::InvPred;
		
		_add( call );
		_predicateEpilogue( i, call.d );
	}

	void PTXToLLVMTranslator::_translateXor( const ir::PTXInstruction& i )
	{
		ir::LLVMXor Xor;
		
		Xor.d = _destination( i );
		Xor.a = _translate( i.a );
		Xor.b = _translate( i.b );

		_add( Xor );
		_predicateEpilogue( i, Xor.d );
	}
	
	void PTXToLLVMTranslator::_bitcast( const ir::PTXInstruction& i )
	{
		ir::LLVMBitcast cast;
		cast.d = _destination( i );
		cast.a = _translate( i.a );
		
		_add( cast );
		_predicateEpilogue( i, cast.d );
	}	

	std::string PTXToLLVMTranslator::_tempRegister()
	{
		std::stringstream stream;
		stream << "%rt" << _tempRegisterCount++;
		return stream.str();
	}

	std::string PTXToLLVMTranslator::_loadSpecialRegister( 
		ir::PTXOperand::SpecialRegister s )
	{
		std::string reg;

		ir::LLVMGetelementptr get;
			
		get.d.type.category = ir::LLVMInstruction::Type::Pointer;
		get.d.type.type = ir::LLVMInstruction::I16;
		get.a.type.category = ir::LLVMInstruction::Type::Pointer;
		get.a.type.label = "%LLVMContext";
		get.a.name = "%__ctaContext";
		get.indices.push_back( 0 );
		
		switch( s )
		{
			case ir::PTXOperand::tidX:
			{
				get.indices.push_back( 0 );
				get.indices.push_back( 0 );
				break;
			}
			case ir::PTXOperand::tidY:
			{
				get.indices.push_back( 0 );
				get.indices.push_back( 1 );
				break;
			}
			case ir::PTXOperand::tidZ:
			{
				get.indices.push_back( 0 );
				get.indices.push_back( 2 );
				break;
			}
			case ir::PTXOperand::ntidX:
			{
				get.indices.push_back( 1 );
				get.indices.push_back( 0 );
				break;
			}
			case ir::PTXOperand::ntidY:
			{
				get.indices.push_back( 1 );
				get.indices.push_back( 1 );
				break;
			}
			case ir::PTXOperand::ntidZ:
			{
				get.indices.push_back( 1 );
				get.indices.push_back( 2 );
				break;
			}
			case ir::PTXOperand::laneId:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::warpId:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::warpSize:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::ctaIdX:
			{
				get.indices.push_back( 2 );
				get.indices.push_back( 0 );
				break;
			}
			case ir::PTXOperand::ctaIdY:
			{
				get.indices.push_back( 2 );
				get.indices.push_back( 1 );
				break;
			}
			case ir::PTXOperand::ctaIdZ:
			{
				get.indices.push_back( 2 );
				get.indices.push_back( 2 );
				break;
			}
			case ir::PTXOperand::nctaIdX:
			{
				get.indices.push_back( 3 );
				get.indices.push_back( 0 );
				break;
			}
			case ir::PTXOperand::nctaIdY:
			{
				get.indices.push_back( 3 );
				get.indices.push_back( 1 );
				break;
			}
			case ir::PTXOperand::nctaIdZ:
			{
				get.indices.push_back( 3 );
				get.indices.push_back( 2 );
				break;
			}
			case ir::PTXOperand::smId:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::nsmId:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::gridId:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::clock:
			{
				ir::LLVMCall call;
				
				call.name = "@clock";
				call.d.type.category = ir::LLVMInstruction::Type::Element;
				call.d.type.type = ir::LLVMInstruction::I32;
				call.d.name = _tempRegister();
				
				_add( call );
				
				return call.d.name;
				break;
			}
			case ir::PTXOperand::pm0:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::pm1:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::pm2:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			case ir::PTXOperand::pm3:
			{
				assertM( false, "Special register " 
					<< ir::PTXOperand::toString( s ) << " not supported." );
				break;
			}
			default: break;
		}
		
		get.d.name = _tempRegister();
		
		_add( get );
		
		ir::LLVMLoad load;
		
		load.d.name = _tempRegister();;
		load.d.type.category = ir::LLVMInstruction::Type::Element;
		load.d.type.type = ir::LLVMInstruction::I16;
		load.a = get.d;
		
		_add( load );
		
		return load.d.name;
	}
	
	std::string PTXToLLVMTranslator::_loadMemoryBase( 
		ir::PTXInstruction::AddressSpace space, ir::PTXOperand::DataType type, 
		size_t offset, ir::PTXInstruction::Vec vector )
	{
		ir::LLVMGetelementptr get;
		
		get.d.name = _tempRegister();
		get.d.type.category = ir::LLVMInstruction::Type::Pointer;
		get.d.type.members.resize( 1 );
		get.d.type.members[0].category = ir::LLVMInstruction::Type::Pointer;
		get.d.type.members[0].type = ir::LLVMInstruction::I8;
		
		get.a.type.label = "%LLVMContext";
		get.a.type.category = ir::LLVMInstruction::Type::Pointer;
		get.a.name = "%__ctaContext";
		get.indices.push_back( 0 );
		
		switch( space )
		{
			case ir::PTXInstruction::Const:
			{
				get.indices.push_back( 6 );
				break;
			}
			case ir::PTXInstruction::Shared:
			{
				get.indices.push_back( 5 );
				break;
			}
			case ir::PTXInstruction::Local:
			{
				get.indices.push_back( 4 );
				break;
			}
			case ir::PTXInstruction::Param:
			{
				get.indices.push_back( 7 );
				break;
			}
			default:
			{
				return "";				
				break;
			}			
		}
				
		_add( get );
		
		ir::LLVMLoad load;
		
		load.d.name = _tempRegister();
		load.d.type.category = ir::LLVMInstruction::Type::Pointer;
		load.d.type.type = ir::LLVMInstruction::I8;
		
		load.a = get.d;
		
		_add( load );
		
		ir::LLVMGetelementptr getIndex;
		
		getIndex.d.name = _tempRegister();
		getIndex.d.type.category = ir::LLVMInstruction::Type::Pointer;
		getIndex.d.type.type = ir::LLVMInstruction::I8;
		
		getIndex.a = load.d;
		getIndex.indices.push_back( offset );
		
		_add( getIndex );
		
		ir::LLVMBitcast cast;
		
		cast.d.type.category = ir::LLVMInstruction::Type::Pointer;
			
		if( vector == ir::PTXOperand::v1 )
		{
			cast.d.type.type = _translate( type );
		}
		else
		{
			cast.d.type.members.resize( 1 );
			cast.d.type.members[ 0 ].category 
				= ir::LLVMInstruction::Type::Vector;
			cast.d.type.members[ 0 ].type = _translate( type );
			cast.d.type.members[ 0 ].vector = vector;		
		}

		cast.d.name = _tempRegister();
		cast.a = getIndex.d;
		
		_add( cast );
		
		return cast.d.name;
	}
	
	void PTXToLLVMTranslator::_setFloatingPointRoundingMode( 
		const ir::PTXInstruction& i )
	{
		ir::LLVMCall call;
		
		call.name = "@setRoundingMode";
		
		call.parameters.resize( 1 );
		call.parameters[0].type.type = ir::LLVMInstruction::I32;
		call.parameters[0].type.category = ir::LLVMInstruction::Type::Element;
		call.parameters[0].i32 = i.modifier;
		call.parameters[0].constant = true;

		if( ir::PTXOperand::PT != i.pg.condition )
		{
			call.parameters.resize( 2 );
			call.parameters[1] = _translate( i.pg );
		}
		
		_add( call );
	}

	ir::LLVMInstruction::Operand PTXToLLVMTranslator::_destination( 
		const ir::PTXInstruction& i, bool pq )
	{
		ir::LLVMInstruction::Operand destination;
		if( pq )
		{
			destination = _translate( i.pq );
		}
		else
		{
			destination = _translate( i.d );
		}
		if( i.pg.condition != ir::PTXOperand::PT )
		{
			destination.name = _tempRegister();			
		}
		return destination;
	}
	
	ir::LLVMInstruction::Operand PTXToLLVMTranslator::_destinationCC( 
		const ir::PTXInstruction& i )
	{
		ir::LLVMInstruction::Operand destination;
		destination.type.category = ir::LLVMInstruction::Type::Element;
		destination.type.type = ir::LLVMInstruction::I64;
		
		if( i.pg.condition != ir::PTXOperand::PT )
		{
			destination.name = _tempRegister();			
		}
		else
		{
			std::stringstream stream;
			stream << "rcc" << _tempCCRegisterCount++;
			destination.name = stream.str();
		}
		
		return destination;
	}
	
	ir::LLVMInstruction::Operand PTXToLLVMTranslator::_conditionCodeRegister( 
		const ir::PTXOperand& op )
	{
		ir::LLVMInstruction::Operand cc;

		std::stringstream stream;
		stream << "rcc" << op.reg;
		
		cc.name = stream.str();
		cc.type.category = ir::LLVMInstruction::Type::Element;
		cc.type.type = ir::LLVMInstruction::I64;
	
		return cc;
	}
	
	void PTXToLLVMTranslator::_predicateEpilogue( const ir::PTXInstruction& i,
		const ir::LLVMInstruction::Operand& temp )
	{
		if( i.pg.condition == ir::PTXOperand::PT ) return;
		
		ir::LLVMSelect select;
		select.condition = _translate( i.pg );
		select.d = _translate( i.d );
	
		if( i.pg.condition == ir::PTXOperand::nPT )
		{
			select.a = select.d;
			select.b = select.d;
		}
		else if( i.pg.condition == ir::PTXOperand::Pred )
		{
			select.a = temp;
			select.b = select.d;
		}
		else
		{
			select.a = select.d;
			select.b = temp;
		}
		
		_add( select );
	}

	void PTXToLLVMTranslator::_add( const ir::LLVMInstruction& i )
	{
		assertM( i.valid() == "", "Instruction " << i.toString() 
			<< " is not valid: " << i.valid() );
		report( "    Added instruction '" << i.toString() << "'" );
		_llvmKernel->_statements.push_back( ir::LLVMStatement( i ) );	
	}

	void PTXToLLVMTranslator::_initializeRegisters()
	{
		report( " Adding initialization instructions." );
		for( RegisterVector::const_iterator reg = _uninitialized.begin(); 
			reg != _uninitialized.end(); ++reg )
		{
			ir::LLVMSelect select;
			
			std::stringstream stream;
			
			stream << "r" << reg->id;
			
			select.d.name = stream.str();
			select.d.type.category = ir::LLVMInstruction::Type::Element;
			select.d.type.type = _translate( reg->type );
			
			select.condition.type.category = ir::LLVMInstruction::Type::Element;
			select.condition.type.type = ir::LLVMInstruction::I1;
			select.condition.constant = true;
			select.condition.i1 = true;
		
			select.a = select.d;
			select.a.constant = true;
			select.a.i64 = 0;
			select.b = select.a;
			
			report( "  Adding instruction '" << select.toString() << "'" );		
			
			_llvmKernel->_statements.push_front( ir::LLVMStatement( select ) );			
		}
		
		if( !_uninitialized.empty() )
		{
			_llvmKernel->_statements.push_front( 
				ir::LLVMStatement( "$OcelotRegisterInitializerBlock" ) );
		}
	}

	void PTXToLLVMTranslator::_addGlobalDeclarations()
	{
		
	}

	void PTXToLLVMTranslator::_addKernelPrefix()
	{
		_llvmKernel->_statements.push_front( 
			ir::LLVMStatement( ir::LLVMStatement::BeginFunctionBody ) );

		ir::LLVMStatement kernel( ir::LLVMStatement::FunctionDefinition );

		kernel.label = "_Z_ocelotTranslated_" + _llvmKernel->name;
		kernel.linkage = ir::LLVMStatement::InvalidLinkage;
		kernel.convention = ir::LLVMInstruction::DefaultCallingConvention;
		kernel.visibility = ir::LLVMStatement::Default;
		kernel.functionAttributes = ir::LLVMInstruction::NoUnwind;
	
		kernel.operand.type.category = ir::LLVMInstruction::Type::Element;
		kernel.operand.type.type = ir::LLVMInstruction::I32;
		
		kernel.parameters.resize( 1 );
		kernel.parameters[ 0 ].attribute = ir::LLVMInstruction::NoAlias;
		kernel.parameters[ 0 ].type.label = "%LLVMContext";
		kernel.parameters[ 0 ].type.category 
			= ir::LLVMInstruction::Type::Pointer;
		kernel.parameters[ 0 ].name = "%__ctaContext";
		
		_llvmKernel->_statements.push_front( kernel );

		ir::LLVMStatement dim3( ir::LLVMStatement::TypeDeclaration );
		
		dim3.label = "%Dimension";
		dim3.operand.type.category = ir::LLVMInstruction::Type::Structure;
		dim3.operand.type.members.resize( 3 );
		dim3.operand.type.members[0].category 
			= ir::LLVMInstruction::Type::Element;
		dim3.operand.type.members[0].type = ir::LLVMInstruction::I16;
		dim3.operand.type.members[1] = dim3.operand.type.members[0];
		dim3.operand.type.members[2] = dim3.operand.type.members[0];
		
		_llvmKernel->_statements.push_front( dim3 );

		_llvmKernel->_statements.push_front( 
			ir::LLVMStatement( ir::LLVMStatement::NewLine ) );		

		ir::LLVMStatement cos( ir::LLVMStatement::FunctionDeclaration );

		cos.label = "cos";
		cos.linkage = ir::LLVMStatement::InvalidLinkage;
		cos.convention = ir::LLVMInstruction::DefaultCallingConvention;
		cos.visibility = ir::LLVMStatement::Default;
		
		cos.operand.type.category = ir::LLVMInstruction::Type::Element;
		cos.operand.type.type = ir::LLVMInstruction::F32;
		
		cos.parameters.resize( 1 );
		cos.parameters[0].type.category = ir::LLVMInstruction::Type::Element;
		cos.parameters[0].type.type = ir::LLVMInstruction::F32;
	
		_llvmKernel->_statements.push_front( cos );		

		ir::LLVMStatement sin( ir::LLVMStatement::FunctionDeclaration );

		sin.label = "sin";
		sin.linkage = ir::LLVMStatement::InvalidLinkage;
		sin.convention = ir::LLVMInstruction::DefaultCallingConvention;
		sin.visibility = ir::LLVMStatement::Default;
		
		sin.operand.type.category = ir::LLVMInstruction::Type::Element;
		sin.operand.type.type = ir::LLVMInstruction::F32;
		
		sin.parameters.resize( 1 );
		sin.parameters[0].type.category = ir::LLVMInstruction::Type::Element;
		sin.parameters[0].type.type = ir::LLVMInstruction::F32;
	
		_llvmKernel->_statements.push_front( sin );		

		ir::LLVMStatement setRoundingMode( 
			ir::LLVMStatement::FunctionDeclaration );

		setRoundingMode.label = "setRoundingMode";
		setRoundingMode.linkage = ir::LLVMStatement::InvalidLinkage;
		setRoundingMode.convention 
			= ir::LLVMInstruction::DefaultCallingConvention;
		setRoundingMode.visibility = ir::LLVMStatement::Default;
		
		setRoundingMode.parameters.resize( 1 );
		setRoundingMode.parameters[0].type.category 
			= ir::LLVMInstruction::Type::Element;
		setRoundingMode.parameters[0].type.type = ir::LLVMInstruction::I32;
	
		_llvmKernel->_statements.push_front( setRoundingMode );

		_llvmKernel->_statements.push_back( 
			ir::LLVMStatement( ir::LLVMStatement::NewLine ) );
		
		ir::LLVMStatement contextType( ir::LLVMStatement::TypeDeclaration );
		
		contextType.label = "%LLVMContext";
		contextType.operand.type = _getCtaContextType();
		
		_llvmKernel->_statements.push_front( contextType );

		_llvmKernel->_statements.push_front( 
			ir::LLVMStatement( ir::LLVMStatement::NewLine ) );
		
		ir::LLVMStatement context( ir::LLVMStatement::VariableDeclaration );

	}
	
	void PTXToLLVMTranslator::_addKernelSuffix()
	{
		_llvmKernel->_statements.push_back( 
			ir::LLVMStatement( ir::LLVMStatement::EndFunctionBody ) );	
	}

	PTXToLLVMTranslator::PTXToLLVMTranslator( OptimizationLevel l ) 
		: Translator( ir::Instruction::PTX, ir::Instruction::LLVM, l ),
		_tempRegisterCount( 0 ), _tempCCRegisterCount( 0 ),
		_tempBlockCount( 0 ), _continuation( 1 )
	{
	
	}
	
	PTXToLLVMTranslator::~PTXToLLVMTranslator()
	{
	
	}
	
	ir::Kernel* PTXToLLVMTranslator::translate( const ir::Kernel* k )
	{
		report( "Translating PTX kernel " << k->name );
		_llvmKernel = new ir::LLVMKernel( *k );	
		
		_addGlobalDeclarations();
		_convertPtxToSsa();
		_translateInstructions();
		_initializeRegisters();
		_addKernelPrefix();
		_addKernelSuffix();
		
		_tempRegisterCount = 0;
		_tempCCRegisterCount = 0;
		_tempBlockCount = 0;
		_continuation = 1;
		_uninitialized.clear();
		delete _graph;
		
		return _llvmKernel;
	}
	
	void PTXToLLVMTranslator::addProfile( const ProfilingData& d )
	{
		// profiling not yet implemented
	}
}

#endif

