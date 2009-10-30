/*	======================
 *	FSTree_sys_mac_gdev.cc
 *	======================
 */

#include "Genie/FS/FSTree_sys_mac_gdev.hh"

// Iota
#include "iota/decimal.hh"
#include "iota/hexidecimal.hh"

// plus
#include "plus/hexidecimal.hh"

// Nucleus
#include "Nucleus/LinkedListContainer.h"

// Nitrogen
#include "Nitrogen/QuickDraw.h"

// Genie
#include "Genie/FS/FSTree_Directory.hh"
#include "Genie/FS/FSTree_Property.hh"
#include "Genie/FS/FSTree_Virtual_Link.hh"
#include "Genie/FS/Scribes.hh"


namespace Nitrogen
{
	
	class DeviceList_ContainerSpecifics
	{
		public:
			typedef GDHandle  value_type;
			typedef UInt32    size_type;
			typedef SInt32    difference_type;
			
			value_type GetNextValue( value_type value )
			{
				return GetNextDevice( value );
			}
			
			static value_type begin_value()  { return GetDeviceList(); }
	};
	
	class DeviceList_Container: public Nucleus::LinkedListContainer< ::Nitrogen::DeviceList_ContainerSpecifics >
	{
		friend DeviceList_Container DeviceList();
		
		private:
			DeviceList_Container()
			: Nucleus::LinkedListContainer< ::Nitrogen::DeviceList_ContainerSpecifics >( ::Nitrogen::DeviceList_ContainerSpecifics() )
			{
			}
	};
	
	inline DeviceList_Container DeviceList()
	{
		return DeviceList_Container();
	}
	
}

namespace Genie
{
	
	namespace N = Nitrogen;
	
	
	struct GDHandle_KeyName_Traits : public Pointer_KeyName_Traits< Nitrogen::GDHandle >  {};
	
	struct sys_mac_gdev_Details : public GDHandle_KeyName_Traits
	{
		typedef Nitrogen::DeviceList_Container Sequence;
		
		static Sequence ItemSequence()  { return Nitrogen::DeviceList(); }
		
		static Key KeyFromValue( const Sequence::value_type& value )  { return value; }
		
		static bool KeyIsValid( const Key& key );
		
		static FSTreePtr GetChildNode( const FSTreePtr&    parent,
		                               const std::string&  name,
		                               const Key&          key );
	};
	
	typedef FSTree_Sequence< sys_mac_gdev_Details > FSTree_sys_mac_gdev;
	
	
	static N::GDHandle GetKeyFromParent( const FSTreePtr& parent )
	{
		return (N::GDHandle) plus::decode_32_bit_hex( parent->Name() );
	}
	
	static N::GDHandle GetKey( const FSTree* that )
	{
		return GetKeyFromParent( that->ParentRef() );
	}
	
	
	bool sys_mac_gdev_Details::KeyIsValid( const Key& key )
	{
		Sequence sequence = ItemSequence();
		
		return std::find( sequence.begin(), sequence.end(), key ) != sequence.end();
	}
	
	
	extern const FSTree_Premapped::Mapping sys_mac_gdev_H_Mappings[];
	
	FSTreePtr sys_mac_gdev_Details::GetChildNode( const FSTreePtr&    parent,
		                                          const std::string&  name,
		                                          const Key&          key )
	{
		return Premapped_Factory< sys_mac_gdev_H_Mappings >( parent, name );
	}
	
	
	struct GetGDBounds
	{
		typedef const Rect& Result;
		
		typedef Rect_Scribe Scribe;
		
		static Result Get( N::GDHandle gdevice )
		{
			return gdevice[0]->gdRect;
		}
	};
	
	struct GetGDSize
	{
		typedef Point Result;
		
		typedef Point_Scribe< 'x' > Scribe;
		
		static Result Get( N::GDHandle gdevice )
		{
			const Rect& bounds = gdevice[0]->gdRect;
			
			const Point size = { bounds.bottom - bounds.top,
			                     bounds.right - bounds.left };
			
			return size;
		}
	};
	
	template < class Accessor >
	struct sys_mac_gdev_N_Property
	{
		typedef N::GDHandle Key;
		
		static std::string Read( const FSTree* that, bool binary )
		{
			Key key = GetKey( that );
			
			const typename Accessor::Result data = Accessor::Get( key );
			
			return Freeze< typename Accessor::Scribe >( data, binary );
		}
	};
	
	template < class Accessor >
	static FSTreePtr Property_Factory( const FSTreePtr&    parent,
	                                   const std::string&  name )
	{
		typedef sys_mac_gdev_N_Property< Accessor > Property;
		
		return New_FSTree_Property( parent,
		                            name,
		                            &Property::Read );
	}
	
	static FSTreePtr Driver_Link_Factory( const FSTreePtr&    parent,
	                                      const std::string&  name )
	{
		GDHandle_KeyName_Traits::Key key = GetKeyFromParent( parent );
		
		std::string unit = iota::inscribe_decimal( ~key[0]->gdRefNum );
		
		return New_FSTree_Virtual_Link( parent, name, "/sys/mac/unit/" + unit );
	}
	
	const FSTree_Premapped::Mapping sys_mac_gdev_H_Mappings[] =
	{
		{ "driver", &Driver_Link_Factory },
		
		{ "bounds", &Property_Factory< GetGDBounds > },
		
		{ NULL, NULL }
	};
	
	FSTreePtr New_FSTree_sys_mac_gdev( const FSTreePtr& parent, const std::string& name )
	{
		return FSTreePtr( new FSTree_sys_mac_gdev( parent, name ) );
	}
	
}

