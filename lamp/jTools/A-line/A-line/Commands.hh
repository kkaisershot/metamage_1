/*	===========
 *	Commands.hh
 *	===========
 */

#pragma once

// Standard C++
#include <string>

// Nitrogen Extras / Templates
#include "Templates/FunctionalExtensions.h"

// BitsAndBytes
#include "StringFilters.hh"

// CompileDriver
#include "CompileDriver/Platform.hh"


namespace ALine
{
	
	//namespace N = Nitrogen;
	namespace CD = CompileDriver;
	
	namespace ext = N::STLExtensions;
	
	
	using BitsAndBytes::q;
	using BitsAndBytes::qq;
	
	static const std::string space = " ";
	
	enum PathnameType
	{
		kMacPathnames,
		kPOSIXPathnames
	};
	
	enum EnvironmentType
	{
		kGenieEnvironment,
		kMPWEnvironment,
		kNativePOSIXEnvironment
	};
	
	enum ToolChainType
	{
		kCodeWarriorTools,
		kGNUTools
	};
	
	enum OperationType
	{
		kPrecompile,
		kCompile,
		kLinkLibrary,
		kLinkProgram
	};
	
	#if 0
	template < ArchitectureType  arch,
	           RuntimeType       runtime,
	           APIType           api,
	           BuildType         build,
	           ToolChainType     tools,
	           OperationType     op >  struct CommandTraits;
	#endif
	
	
	// Environment refers to the environment of the tool we wish to run,
	// not the environment A-line is running in.
	
	template < EnvironmentType type > struct EnvironmentTraits;
	
	template <>
	struct EnvironmentTraits< kGenieEnvironment >
	{
		static const PathnameType pathnameType = kPOSIXPathnames;
		
		static std::string MakeNativeCommand( const std::string& command )
		{
		#if TARGET_RT_MAC_MACHO
			
			// A-line is a native Mach-O tool running in the Terminal.
			// Not supported.
			// Besides, any tool that runs in Genie should build as Mach-O anyway.
			
			// Missing return value warning
			
		#else
			
			// A-line is a CFM Genie plugin.
			return command;
			
		#endif
		}
	};
	
	template <>
	struct EnvironmentTraits< kMPWEnvironment >
	{
		static const PathnameType pathnameType = kMacPathnames;
		
		static std::string MakeNativeCommand( const std::string& command )
		{
			// This works either within Genie or in native POSIX.
			return "tlsrvr --switch --escape -- " + command;
		}
	};
	
	template <>
	struct EnvironmentTraits< kNativePOSIXEnvironment >
	{
		static const PathnameType pathnameType = kPOSIXPathnames;
		
		static std::string MakeNativeCommand( const std::string& command )
		{
		#if TARGET_RT_MAC_MACHO
			
			// A-line is a native Mach-O tool running in the Terminal.
			return command;
			
		#else
			
			// A-line is a Carbon CFM Genie plugin.  This is a hack.
			return std::string( "dss " ) + qq( command + " && true" );
			
		#endif
		}
	};
	
	
	struct CommandGenerator
	{
		TargetInfo target;
		
		bool m68k, ppc, x86;
		bool a4, a5, cfm, machO;
		bool classic, carbon;
		bool debug;
		bool gnu;
		
		CommandGenerator( const TargetInfo& target )
		:
			target( target ),
			m68k( target.platform.arch == CD::arch68K ),
			ppc ( target.platform.arch == CD::archPPC ),
			x86 ( target.platform.arch == CD::archX86 ),
			a4( target.platform.runtime == CD::runtimeA4CodeResource ),
			a5( target.platform.runtime == CD::runtimeA5CodeSegments ),
			cfm( target.platform.runtime == CD::runtimeCodeFragments ),
			machO( target.platform.runtime == CD::runtimeMachO ),
			classic( target.platform.api == CD::apiMacToolbox ),
			carbon( target.platform.api == CD::apiMacCarbon ),
			debug( target.build == buildDebug ),
			gnu( target.toolkit == toolkitGNU )
		{}
		
		std::string MakeNativeCommand( const std::string& command )
		{
			return ( gnu ? EnvironmentTraits< kNativePOSIXEnvironment >::MakeNativeCommand
			             : EnvironmentTraits< kMPWEnvironment         >::MakeNativeCommand )( command );
		}
		
		// CodeWarrior only
		std::string MWToolSuffix() const  { return m68k ? "68K" : "PPC"; }
		
		// CodeWarrior only
		std::string MWLinkerName() const  { return "MWLink" + MWToolSuffix(); }
		
		std::string CompilerName() const
		{
			return gnu ? "gcc -c"
			           : "MWC" + MWToolSuffix();
		}
		
		std::string LinkerName() const
		{
			return gnu ? "g++"
			           : MWLinkerName();
		}
		
		std::string LibraryMakerName() const
		{
			return gnu ? "libtool"
			           : MWLinkerName() + " -xm l";
		}
		
		std::string TargetArchitecture() const
		{
			return gnu ? ppc ? "-arch ppc"
			                 : "-arch i386"
			           : "";
		}
		
		// CodeWarrior only
		std::string MWMiscCompilerOptions() const
		{
			return "-nosyspath "
			       "-w all,noemptydecl,nopossible,noimplicit,nounusedarg,nonotinlined "
			       "-ext o "
			       "-maxerrors 8";
		}
		
		// This means that we pass the precompiled output, not the header source.
		// Required for CodeWarrior (i.e. otherwise you lose precompile benefit).
		// Not supported by gcc.
		bool PrecompiledHeaderIsImage() const  { return !gnu; }
		
		// This means that the prefix header is given by name only
		// and the parent directory of the precompiled header image
		// is in the search path.
		// Required for gcc unless the precompiled header image is in the same
		// directory as the precompiled header source.
		// Not supported by CodeWarrior.
		bool PrecompiledHeaderIsSearched() const  { return gnu; }
		
		std::string Prefix( const std::string& pathname ) const
		{
			return std::string( gnu ? "-include" : "-prefix" ) + " " + q( pathname );
		}
		
		std::string ConvertPaths() const { return gnu ? "" : "-convertpaths"; }
		
		std::string Newlines() const  { return gnu ? "" : "-nomapcr"; }
		
		std::string PreprocessorOptions() const
		{
			std::string result;
			
			result = ConvertPaths()
			       + " " + Newlines();
			
			return result;
		}
		
		
		std::string PascalStrings() const  { return gnu ? "-fpascal-strings" : ""; }
		
		std::string StrictPrototypes() const  { return gnu ? "" : "-proto strict"; }
		
		std::string NoRTTI() const  { return gnu ? "-fno-rtti" : "-RTTI off"; }
		
		std::string LanguageOptions() const
		{
			std::string result;
			
			// gcc won't compile shared_ptr without RTTI
			
			result = PascalStrings()
			       + " " + StrictPrototypes()
			       + (gnu ? ""
			              : " " + NoRTTI());
			
			return result;
		}
		
		
		std::string DebugSymbols() const
		{
			return debug ? "-g" : "";
		}
		
		// CodeWarrior only
		std::string MWNoMacsBugSymbols() const  { return m68k && !debug ? "-mbg off" : ""; }
		
		// CodeWarrior only
		std::string CFMTracebackTables() const  { return carbon && cfm && debug ? "-tb on" : ""; }
		
		// CodeWarrior only
		std::string MWCodeModel() const
		{
			return m68k ? std::string() + "-model far" + " " + (cfm ? "-model CFMflatdf" : "")
			            : "";
		}
		
		// CodeWarrior only
		std::string MW68KGlobals() const  { return a4 ? "-a4" : ""; }
		
		std::string Optimization() const
		{
			return debug ? gnu ? "-O2"
			                   : "-O4"
			             : "-O0";
		}
		
		std::string CodeGenOptions() const
		{
			std::string dependent = gnu ? ""
			                            : ( MWNoMacsBugSymbols()
			                        + " " + CFMTracebackTables()
			                        + " " + MWCodeModel()
			                        + " " + MW68KGlobals() );
			
			return Optimization() + " " + DebugSymbols() + " " + dependent;
		}
		
		std::string AllCompilerOptions() const
		{
			std::string result;
			
			result =         TargetArchitecture()
			         + " " + ( gnu ? "" : MWMiscCompilerOptions() )
			         + " " + PreprocessorOptions()
			         + " " + LanguageOptions()
			         + " " + CodeGenOptions();
			
			return result;
		}
		
		std::string Output( const std::string& pathname ) const
		{
			return "-o " + q( pathname );
		}
		
		std::string PrecompiledOutput( const std::string& pathname ) const
		{
			return std::string( gnu ? "-o" : "-precompile" ) + " " + q( pathname );
		}
		
		std::string Input( const std::string& pathname ) const
		{
			return q( pathname );
		}
		
		std::string OutputType( const std::string& type ) const
		{
			return std::string( "-t " ) + type;
		}
		
		std::string OutputCreator( const std::string& creator ) const
		{
			return std::string( "-c " ) + creator;
		}
		
		std::string ResourceTypeAndID( const std::string& type, const std::string& id ) const
		{
			return std::string( "-rt " ) + type + "=" + id;
		}
		
		std::string ResourceName( const std::string& name ) const
		{
			return "-rsrcname " + name;
		}
		
		std::string CustomDriverHeader() const  { return "-custom"; }
		
		std::string AppHeapSize() const
		{
			return "-sizemin 4096 -sizemax 8192";
		}
		
		std::string TargetApplication() const
		{
			return gnu ? "" : "-xm a -dead " + std::string( m68k ? "code" : "off" ) + space + AppHeapSize();
		}
		
		// CodeWarrior only
		std::string MWTargetCodeResource() const
		{
			return "-xm c -rsrcfar -rsrcflags system";
		}
		
		// CodeWarrior only
		std::string MWTargetSharedLibrary() const
		{
			return "-xm s -init __initialize -term __terminate -export pragma";
		}
		
		// CodeWarrior only
		std::string MWTargetKeroseneShLib() const
		{
			return "-xm s -init InitializeFragment -term TerminateFragment -export pragma -name " + std::string( carbon ? "Carbon" : "classic" ) + space + AppHeapSize();
		}
		
		// CodeWarrior only
		std::string MWTargetKeroseneRsrc() const
		{
			return MWTargetCodeResource() + space + ResourceTypeAndID( "Wish", "0" );
		}
		
		// CodeWarrior only
		std::string MWTargetKerosene() const
		{
			return cfm ? MWTargetKeroseneShLib() : MWTargetKeroseneRsrc();
		}
		
		std::string TargetCommandLineTool() const
		{
			return gnu ? ""
			           : MWTargetKerosene() + " " + OutputType   ( "Wish" )
			                                + " " + OutputCreator( "Poof" );
		}
		
		std::string MWLinkerOptions() const
		{
			return DebugSymbols() + " " + MWCodeModel() + " " + CFMTracebackTables();
		}
		
		std::string LinkerOptions() const
		{
			return gnu ? ""
			           : MWLinkerOptions();
		}
		
	};
	
	
	template < class F, class Iter >
	std::string join( Iter begin, Iter end, const std::string& glue = "", F f = F() )
	{
		if ( begin == end )
		{
			return "";
		}
		
		std::string result = f( *begin++ );
		
		while ( begin != end )
		{
			result += glue;
			result += f( *begin++ );
		}
		
		return result;
	}
	
	template < class Iter >
	std::string join( Iter begin, Iter end, const std::string& glue = "" )
	{
		return join( begin, end, glue, ext::identity< std::string >() );
	}
	
}

